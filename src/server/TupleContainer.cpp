#include <src/server/TupleContainer.h>

TupleContainer::TupleContainer()
{
  tuples = {};
}

TupleContainer::~TupleContainer()
{
  tuples.clear();
}

void TupleContainer::add(Tuple* tuple)
{
  std::cout <<"MAKARENA " << (*tuple) << std::endl;
  tuples.push_back(tuple);
  std::cout <<"arivederci roma " << tuples << std::endl;
}

Tuple TupleContainer::get(int index) const
{
  if (tuples.size() > index)
  {
    std::list<Tuple*>::const_iterator it = tuples.begin();
    std::advance(it, index);
    return *(*it);
  }
  else
  {
    return Tuple();
  }
}

void TupleContainer::remove(int index)
{
  if (tuples.size() > index)
  {
    std::list<Tuple*>::const_iterator it = tuples.begin();
    std::advance(it, index);
    tuples.erase(it);
  }
}

bool TupleContainer::is_in(const Tuple* tuple) const
{
  for (std::list<Tuple*>::const_iterator it = tuples.begin(); it != tuples.end(); ++it)
  {
    if (*it == tuple)
    {
      return true;
    }
  }
  return false;
}

int TupleContainer::size() const
{
  return tuples.size();
}

void TupleContainer::show_elems() const
{
  
  if (tuples.size() > 0)
  {
    for (std::list<Tuple*>::const_iterator it = tuples.begin(); it != tuples.end(); ++it)
    {
      std::cout << *(*it) << std::endl;
    }
  }
  else
  {
    std::cout << "No tuples in container" << std::endl;
  }
}

void TupleContainer::clear()
{
  tuples.clear();
}

int TupleContainer::find(TuplePattern* pattern) const
{
  int index = 0;
  for (std::list<Tuple*>::const_iterator it = tuples.begin(); it != tuples.end(); ++it)
  {
    if (pattern->matches(*(*it)))
    {
      return index;
    }
    index++;
  }
  return -1;
}