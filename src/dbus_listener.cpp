#include "dbus_listener.hpp"
#include "dbus/dbus-shared.h"
#include "notification_handler.hpp"
#include <cerrno>
#include <cstddef>
#include <dbus/dbus.h>
#include <iostream>
#include <string.h>
#include <thread>

void myLOG(std::string message) { std::cout << message << std::endl; }

//  TODO: parse args, render popup, etc
DBusHandlerResult message_handler(DBusConnection *connection, DBusMessage *msg,
                                  void *user_data) {
  const char *member = dbus_message_get_member(msg);
  const char *path = dbus_message_get_path(msg);

  if (!member || !path)
    return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;

  const char *name = "waynotify";
  const char *vendor = "linux";
  const char *version = "1.0";
  const char *spec_version = "1.2";

  // Handle GetServerInformation
  if (strcmp(path, "/org/freedesktop/Notifications") == 0 &&
      strcmp(member, "GetServerInformation") == 0) {

    DBusMessage *reply = dbus_message_new_method_return(msg);
    dbus_message_append_args(reply, DBUS_TYPE_STRING, &name, DBUS_TYPE_STRING,
                             &vendor, DBUS_TYPE_STRING, &version,
                             DBUS_TYPE_STRING, &spec_version,
                             DBUS_TYPE_INVALID);
    dbus_connection_send(connection, reply, nullptr);
    dbus_message_unref(reply);

    return DBUS_HANDLER_RESULT_HANDLED;
  }

  // Handling the Notification
  if (strcmp(path, "/org/freedesktop/Notifications") == 0 &&
      strcmp(member, "Notify") == 0) {

    // Store in args
    DBusMessageIter args;
    if (!dbus_message_iter_init(msg, &args)) {
      std::cerr << "Message has no arguments!" << std::endl;
      return DBUS_HANDLER_RESULT_HANDLED;
    }

    const char *application_name;
    const char *summary;
    const char *body;
    int32_t replaces_id;
    const char *application_icon;
    int32_t expire_timeout;

    // Go through one after the other
    dbus_message_iter_get_basic(&args, &application_name);
    dbus_message_iter_next(&args);
    dbus_message_iter_get_basic(&args, &replaces_id);
    dbus_message_iter_next(&args);
    dbus_message_iter_get_basic(&args, &application_icon);
    dbus_message_iter_next(&args);
    dbus_message_iter_get_basic(&args, &summary);
    dbus_message_iter_next(&args);
    dbus_message_iter_get_basic(&args, &body);
    dbus_message_iter_next(&args);

    // Skipping actions and hints
    dbus_message_iter_next(&args); // skip actions
    dbus_message_iter_next(&args); // skip hints

    dbus_message_iter_get_basic(&args, &expire_timeout);

    if(expire_timeout < 1000) expire_timeout = 4000;

    std::cout << "\n[Notification]\n"
              << "Application: " << application_name << "\n"
              << "Title: " << summary << "\n"
              << "body:" << body << "\n"
              << "App ICON: " << application_icon << "\n"
              << "TimeOut: " << expire_timeout << "ms\n"
              << std::endl;

    // Leaving it to notification handler_function
    handle_notification(application_name, summary, body, application_icon, expire_timeout);


    DBusMessage *reply = dbus_message_new_method_return(msg);
    uint32_t fake_id = 1; // to increment for each message
    dbus_message_append_args(reply, DBUS_TYPE_UINT32, &fake_id,
                             DBUS_TYPE_INVALID);
    dbus_connection_send(connection, reply, nullptr);
    dbus_message_unref(reply);

    return DBUS_HANDLER_RESULT_HANDLED;
  }

  return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

void listen_for_notifications() {
  DBusError err;
  dbus_error_init(&err);

  DBusConnection *connection = dbus_bus_get(DBUS_BUS_SESSION, &err);

  // check if error exists at init.
  if (dbus_error_is_set(&err)) {
    std::cerr << "DBus Error: " << err.message << std::endl;
    dbus_error_free(&err);
    return;
  }

  // Enable the connection to org.freedesktop.notifications
  // org.freedesktop.Notifications is a D-Bus service specification that defines
  // how applications on Linux desktop environments can send notifications to
  // users.
  dbus_bus_request_name(connection, "org.freedesktop.Notifications",
                        DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
  if (dbus_error_is_set(&err)) {
    std::cerr << "Name Error: " << err.message << std::endl;
    dbus_error_free(&err);
    return;
  } else {
    std::cout << "Registered successfully. Listening for notifications..."
              << std::endl;
  }

  static DBusObjectPathVTable vtable = {nullptr, message_handler, nullptr,
                                        nullptr, nullptr};

  if (!dbus_connection_register_object_path(
          connection, "/org/freedesktop/Notifications", &vtable, nullptr)) {
    std::cerr << "Failed to register object path." << std::endl;
    return;
  }

  myLOG("Ready to receive Notifications");

  // Let the D-bus know that I'm listening for  notifications that are sent to
  // `org.freedesktop.Notifications` and the Method `Notify`
  // dbus_bus_add_match(
  //     connection,
  //     "type='method_call',interface='org.freedesktop.Notifications'", &err);
  // dbus_connection_flush(connection);

  // if (dbus_error_is_set(&err)) {
  //   std::cerr << "Match rule error: " << err.message << std::endl;
  //   dbus_error_free(&err);
  // } else {
  //   std::cout << "Match rule added. Waiting for notifications..." <<
  //   std::endl;
  // }

  while (true) {
    dbus_connection_read_write_dispatch(connection, 100);
  }
}
