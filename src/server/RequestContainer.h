#ifndef REQUESTCONTAINER_H
#define REQUESTCONTAINER_H

#include "Request.h"

#include <list>

class RequestContainer
{
  public:
  RequestContainer();
  ~RequestContainer();
  void add(const Request request);
  Request get(int index) const;
  void remove(int index);
  int size() const;
  bool is_in(const Request request) const;
  void show_elems() const;
  void clear();
  int find(std::string request) const;
  int find_id(long request_id) const;

  private:
  std::list<Request*> requests;
};

#endif