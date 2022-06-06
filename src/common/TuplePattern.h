#ifndef TUPLEPATTERN_H
#define TUPLEPATTERN_H

#include "Constants.h"
#include "Enums.h"
#include "Tuple.h"
#include "TuplePatternElem.h"

#include <iostream>
#include <string.h>
#include <vector>

class TuplePattern
{
  private:
  std::vector<TuplePatternElem> m_elements;
  int m_size;

  public:
  TuplePattern(){};
  TuplePattern(std::vector<TuplePatternElem> elements, int size);
  ~TuplePattern();

  bool matches(Tuple tuple);

  std::string serialize() const;
  void deserialize(std::string serialized);

  void setElements(std::vector<TuplePatternElem> v);
  void setSize(int s);

  int getSize()
  {
    return m_size;
  };
  std::vector<TuplePatternElem> getElements() const
  {
    return m_elements;
  };
  friend std::ostream& operator<<(std::ostream& out, const TuplePattern& tuple);
  friend bool operator==(const TuplePattern& t1, const TuplePattern& t2);
};


#endif