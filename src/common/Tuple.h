#ifndef TUPLE_H
#define TUPLE_H

#include "Constants.h"
#include "Enums.h"
#include "TupleElem.h"

#include <iostream>
#include <string.h>
#include <vector>


class Tuple
{
  private:
  std::vector<TupleElem> m_elements;
  int m_size;

  public:
  Tuple(){};
  Tuple(std::vector<TupleElem>& elements, int size);
  ~Tuple();

  std::vector<TupleElem> getElements() const
  {
    return m_elements;
  };
  std::string serialize() const;
  void deserialize(std::string serialized);

  void setElements(std::vector<TupleElem> v);
  void setSize(int s);


  int getSize() const
  {
    return m_size;
  };
  friend std::ostream& operator<<(std::ostream& out, const Tuple& tuple);
  friend bool operator==(const Tuple& t1, const Tuple& t2);
};


#endif