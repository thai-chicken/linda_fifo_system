#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include "../common/TuplePatternMessage.h"

class Request
{
  public:
  Request();
  Request(TuplePatternMessage* m);
  ~Request();
  // void set_request(const std::string request);
  // std::string get_request() const;
  // void set_request_pid(const pid_t request_pid);
  // int get_request_pid() const;
  // void set_timeout_pid(const pid_t timeout_pid);
  // int get_timeout_pid() const;
  // void set_command(const std::string command);
  // std::string get_command() const;
  TuplePatternMessage* getMessage() const;
  int getId() const;
  void setMessage(TuplePatternMessage* m);
  void setId(int Id);


  private:
  TuplePatternMessage* msg;
  int id;
};

#endif