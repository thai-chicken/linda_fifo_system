#include "RequestContainer.h"

#include <iostream>

RequestContainer::RequestContainer()
{
  requests = {};
}

RequestContainer::~RequestContainer()
{
  for (std::list<Request*>::iterator it = requests.begin(); it != requests.end(); ++it)
  {
    delete *it;
  }
}

void RequestContainer::add(Request request)
{
  requests.push_back(new Request(request.getMessage()));
}

Request RequestContainer::get(int index) const
{
  if (requests.size() > index)
  {
    std::list<Request*>::const_iterator it = requests.begin();
    std::advance(it, index);
    return *(*it);
  }
  else
  {
    return Request();
  }
}

void RequestContainer::remove(int index)
{
  if (requests.size() > index)
  {
    std::list<Request*>::const_iterator it = requests.begin();
    std::advance(it, index);
    delete *it;
    requests.erase(it);
  }
}

int RequestContainer::size() const
{
  return requests.size();
}

bool RequestContainer::is_in(Request request) const
{
  for (std::list<Request*>::const_iterator it = requests.begin(); it != requests.end(); ++it)
  {
    if ((*it)->getMessage()->getTuplePattern() == request.getMessage()->getTuplePattern())
    {
      return true;
    }
  }
  return false;
}

void RequestContainer::show_elems() const
{
  if (requests.size() > 0)
  {
    for (std::list<Request*>::const_iterator it = requests.begin(); it != requests.end(); ++it)
    {
      std::cout << "Request: " << (*it)->getMessage()->getTuplePattern() << std::endl;
    }
  }
  else
  {
    std::cout << "No requests in container!" << std::endl;
  }
}

void RequestContainer::clear()
{
  for (std::list<Request*>::iterator it = requests.begin(); it != requests.end(); ++it)
  {
    delete *it;
  }
  requests.clear();
}

int RequestContainer::find(Tuple* tuple) const
{
  int i = 0;
  for (std::list<Request*>::const_iterator it = requests.begin(); it != requests.end(); ++it)
  {
    if ((*it)->getMessage()->getTuplePattern().matches(*tuple))
    {
      return i;
    }
    i++;
  }
  return -1;
}
