#ifndef TUPLECONTAINER_H
#define TUPLECONTAINER_H

#include <iostream>
#include <list>
#include <string>

class TupleContainer
{
  public:
  TupleContainer();
  ~TupleContainer();
  void add(const std::string tuple);
  std::string get(int index) const;
  void remove(int index);
  int size() const;
  bool is_in(const std::string tuple) const;
  void show_elems() const;
  void clear();
  int find(const std::string tuple) const;

  private:
  std::list<std::string> tuples;
};

#endif