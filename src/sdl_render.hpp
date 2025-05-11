#pragma once
#include <string>

class Notificationwindow {
public:
  Notificationwindow(const std::string &title, const std::string &body, const std::string &icon,
                     int timeout_ms);
  ~Notificationwindow();
  void show();
  std::string iconSanitizer(const std::string& icon_path);

private:
  std::string title_;
  std::string body_;
  std::string icon_;
  int timeout_ms_;
};
