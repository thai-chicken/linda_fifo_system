#ifndef REQUEST_H
#define REQUEST_H

#include <string>

class Request
{
  public:
  Request();
  Request(std::string request_tuple, pid_t request_pid, std::string command);
  Request(std::string request_tuple, pid_t request_pid, pid_t timeout_pid, std::string command);
  ~Request();
  void set_request(const std::string request);
  std::string get_request() const;
  void set_request_pid(const pid_t request_pid);
  int get_request_pid() const;
  void set_timeout_pid(const pid_t timeout_pid);
  int get_timeout_pid() const;
  void set_command(const std::string command);
  std::string get_command() const;

  private:
  std::string request_tuple;
  pid_t request_pid;
  pid_t timeout_pid;
  std::string command;
};

#endif