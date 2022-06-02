#include <src/server/Request.h>

Request::Request()
{
  this->request_tuple = "";
  this->request_pid = 0;
  this->timeout_pid = 0;
}

Request::~Request() {}

Request::Request(std::string request_tuple, pid_t request_pid, std::string command)
{
  this->request_tuple = request_tuple;
  this->request_pid = request_pid;
  this->timeout_pid = 0;
  this->command = command;
}

Request::Request(std::string request_tuple, pid_t request_pid, pid_t timeout_pid, std::string command)
{
  this->request_tuple = request_tuple;
  this->request_pid = request_pid;
  this->timeout_pid = timeout_pid;
  this->command = command;
}

std::string Request::get_request() const
{
  return this->request_tuple;
}

pid_t Request::get_request_pid() const
{
  return this->request_pid;
}

pid_t Request::get_timeout_pid() const
{
  return this->timeout_pid;
}

void Request::set_request(const std::string request)
{
  this->request_tuple = request;
}

void Request::set_request_pid(const pid_t request_pid)
{
  this->request_pid = request_pid;
}

void Request::set_timeout_pid(const pid_t timeout_pid)
{
  this->timeout_pid = timeout_pid;
}

void Request::set_command(const std::string command)
{
  this->command = command;
}

std::string Request::get_command() const
{
  return this->command;
}
