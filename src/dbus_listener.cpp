#include "dbus_listener.hpp"
#include <cerrno>
#include <dbus/dbus.h>
#include <iostream>
#include <thread>

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
  dbus_bus_request_name(connection, "org.freedesktop.notifications",
                        DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
  if (dbus_error_is_set(&err)) {
    std::cerr << "Name Error: " << err.message << std::endl;
    dbus_error_free(&err);
    return;
  }

  while (true) {
    dbus_connection_read_write(connection, 0);
    DBusMessage *msg = dbus_connection_pop_message(
        connection); // retrieves and removes the next message from the incoming
                     // message queue of a DBusConnection
    if (!msg) {
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
      continue;
    }

    // Get the Notification from the App and its details
    if(dbus_message_is_method_call(msg, "org.freedesktop.Notifications","Notify")){
        const char* application_name;
        const char* summary;
        const char* body;
        int32_t replaces_id;
        const char* application_icon;
        int32_t expire_timeout;

        // Store in args
        DBusMessageIter args;
        dbus_message_iter_init(msg, &args);

        // Go through one after the other
        dbus_message_iter_get_basic(&args, &application_name);
        dbus_message_iter_next(&args);

        dbus_message_iter_get_basic(&args, &replaces_id);


    }
  }
}
