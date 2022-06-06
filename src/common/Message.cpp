#include "Message.h"

pid_t Message::getPid() const
{
  return this->pid;
}

Command Message::getCommand() const
{
  return this->cmd;
}

MessageType Message::getType() const
{
  return this->type;
}

int Message::getTimeout() const
{
  return this->timeout;
}


void Message::setPid(pid_t p)
{
  this->pid = p;
}

void Message::setCommand(Command cmd)
{
  this->cmd = cmd;
}

void Message::setType(MessageType t)
{
  this->type = t;
}

void Message::setTimeout(int timeout)
{
  this->timeout = timeout;
}