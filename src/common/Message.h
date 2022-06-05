#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include "Constants.h"
#include "Enums.h"
#include "TuplePattern.h"
#include "Tuple.h"
// #define MSG_SIZE 80

// Sama wiadomosc zajmowac bedzie 7B + int to 11?
class Message
{
private:
  pid_t pid;
  Command cmd;
  MessageType type;
  int timeout=0;

public:
  Message() {};
  virtual ~Message() {};
  pid_t getPid() const;
  Command getCommand() const;
  MessageType getType() const;
  int getTimeout() const;
  void setPid(pid_t p);
  void setCommand(Command cmd);
  void setType(MessageType t);
  void setTimeout(int timeout);


  virtual std::string serialize() = 0;
  virtual void deserialize(std::string serialized) = 0;

};

#endif // MESSAGE_H