#pragma once
#include <string>

class Notificationwindow {
public:
  Notificationwindow(const std::string &title, const std::string &body,
                     int timeout_ms);
  ~Notificationwindow();
  void show();

private:
  std::string title_;
  std::string body_;
  int timeout_ms_;
};
