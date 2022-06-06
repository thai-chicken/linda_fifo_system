#ifndef TUPLEELEM_H
#define TUPLEELEM_H

#include "Constants.h"
#include "Enums.h"

#include <boost/algorithm/string.hpp>
#include <iostream>
#include <string.h>

class TupleElem
{
  private:
  char m_value[MAX_VALUE_SIZE];
  ElemType m_type;
  int m_offset = 0;

  public:
  TupleElem();
  TupleElem(std::string value, ElemType type);
  ~TupleElem();

  std::string getValue() const;
  ElemType getElemType() const;
  int getOffset() const;

  std::string serialize();
  void deserialize(std::string serialized);


  friend std::ostream& operator<<(std::ostream& out, const TupleElem& elem);
  friend bool operator==(const TupleElem& te1, const TupleElem& te2);
};

#endif