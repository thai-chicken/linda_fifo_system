#ifndef TUPLEMESSAGE_H
#define TUPLEMESSAGE_H

#include "Constants.h"
#include "Enums.h"
#include "Message.h"
#include "Tuple.h"

class TupleMessage : public Message
{
  private:
  Tuple tuple;

  public:
  TupleMessage(){};
  virtual ~TupleMessage(){};

  Tuple getTuple();
  void setTuple(Tuple t);

  virtual std::string serialize()
  {
    std::string serialized;
    serialized += std::to_string(this->getPid());
    for (int i = 0; i < PID_MAX_DIGITS - serialized.length(); i++)
    {
      serialized += " ";
    }
    serialized += std::to_string(this->getType());
    serialized += std::to_string(this->getCommand());
    serialized += this->tuple.serialize();
    return serialized;
  };
  virtual void deserialize(std::string serialized)
  {
    // Wiadomosc zajmuje 7B
    this->setPid(atoi(serialized.substr(0, PID_MAX_DIGITS).c_str()));
    this->setType(static_cast<MessageType>(serialized[PID_MAX_DIGITS] - 48));
    this->setCommand(static_cast<Command>(serialized[PID_MAX_DIGITS + 1] - 48));
    // Tupla zajmuje MAX_N_OF_ELEMENTS*(MAX_VALUE_SIZE+1) + 1 bajty
    this->tuple.deserialize(serialized.substr(PID_MAX_DIGITS + 2, MAX_N_OF_ELEMENTS * (MAX_VALUE_SIZE + 1) + 1).c_str());
  };
};

#endif