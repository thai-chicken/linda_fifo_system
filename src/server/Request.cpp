#include <src/server/Request.h>

Request::Request()
{
  this->msg = nullptr;
}

Request::~Request() {}

Request::Request(TuplePatternMessage* msg)
{
  this->msg = msg;
}

TuplePatternMessage* Request::getMessage() const 
{
  return this->msg;
}

int Request::getId() const
{
  return this->id;
}

void Request::setMessage(TuplePatternMessage* msg) 
{
  this->msg = msg;
}

void Request::setId(int id)
{
  this->id = id;
}
// std::string Request::get_request() const
// {
//   return this->request_tuple;
// }

// pid_t Request::get_request_pid() const
// {
//   return this->request_pid;
// }

// pid_t Request::get_timeout_pid() const
// {
//   return this->timeout_pid;
// }

// void Request::set_request(const std::string request)
// {
//   this->request_tuple = request;
// }

// void Request::set_request_pid(const pid_t request_pid)
// {
//   this->request_pid = request_pid;
// }

// void Request::set_timeout_pid(const pid_t timeout_pid)
// {
//   this->timeout_pid = timeout_pid;
// }

// void Request::set_command(const std::string command)
// {
//   this->command = command;
// }

// std::string Request::get_command() const
// {
//   return this->command;
// }
