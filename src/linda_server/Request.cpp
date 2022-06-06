#include "Request.h"

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