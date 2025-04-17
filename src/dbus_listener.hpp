#pragma once
#include <string>


struct Notification {
    std::string app_name;
    std::string summary;
    std::string body;
    int timeout;
};

void listen_for_notifications();
