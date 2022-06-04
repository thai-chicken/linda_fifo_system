#ifndef TUPLECONTAINER_H
#define TUPLECONTAINER_H

#include <iostream>
#include <list>
#include <string>
#include "../common/TuplePattern.h"
#include "../common/Tuple.h"

class TupleContainer
{
  public:
  TupleContainer();
  ~TupleContainer();
  void add(Tuple* tuple);
  Tuple get(int index) const;
  void remove(int index);
  int size() const;
  bool is_in(const Tuple* tuple) const;
  void show_elems() const;
  void clear();
  int find(TuplePattern* pattern) const;

  private:
  std::list<Tuple*> tuples;
};

#endif