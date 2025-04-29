#pragma once
#include<iostream>
#include <string>

void handle_notification(
    const std::string& app_name,
    const std::string& summary,
    const std::string& body,
    int timeout_ms
);
