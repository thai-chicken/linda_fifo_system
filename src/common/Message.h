#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#define MSG_SIZE 80

struct Message
{
  pid_t pid;
  std::string command;
  std::string msg;
  int timeout=0;
};

#endif // MESSAGE_H