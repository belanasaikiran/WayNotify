#include "notification_handler.hpp"
#include "dbus_listener.hpp"
#include "sdl_render.hpp"
#include <thread>

void handle_notification(const std::string &app_name, const std::string &summary, const std::string &body, int timeout_ms){
    // Log this somewhere - for debugging
    std::cout << "[HANDLE] " << app_name << " - " << summary << " - " << timeout_ms << "\n";

    std::thread([=](){
        Notificationwindow popup(summary, body, timeout_ms);
        popup.show();
    }).detach();

}
