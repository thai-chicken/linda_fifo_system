#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include "Constants.h"
#include "Enums.h"
// #define MSG_SIZE 80

// Sama wiadomosc zajmowac bedzie 7B
class Message
{
private:
  pid_t pid;
  Command cmd;
  MessageType type;
public:
  Message() {};
  virtual ~Message() {};
  pid_t getPid();
  Command getCommand();
  MessageType getType();
  void setPid(pid_t p);
  void setCommand(Command cmd);
  void setType(MessageType t);

  virtual std::string serialize() = 0;
  virtual void deserialize(std::string serialized) = 0;

};

#endif // MESSAGE_H