#ifndef REQUEST_H
#define REQUEST_H

#include "../common/TuplePatternMessage.h"

#include <string>

class Request
{
  public:
  Request();
  Request(TuplePatternMessage* m);
  ~Request();
  TuplePatternMessage* getMessage() const;
  int getId() const;
  void setMessage(TuplePatternMessage* m);
  void setId(int Id);


  private:
  TuplePatternMessage* msg;
  int id;
};

#endif