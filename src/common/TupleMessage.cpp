#include "TupleMessage.h"


Tuple TupleMessage::getTuple()
{
  return this->tuple;
}

void TupleMessage::setTuple(Tuple t)
{
  this->tuple = t;
}
