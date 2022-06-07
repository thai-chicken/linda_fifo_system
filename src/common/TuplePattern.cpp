#include "TuplePattern.h"

TuplePattern::TuplePattern(std::vector<TuplePatternElem> elements, int size)
{
  m_elements = elements;
  m_size = size;
}

TuplePattern::~TuplePattern() {}

bool TuplePattern::matches(Tuple tuple)
{
  if (this->m_size != tuple.getSize())
  {
    return false;
  }
  bool ifMatch = true;
  for (int i = 0; i < this->m_size; i++)
  {
    if (!this->m_elements[i].matches(tuple.getElements()[i]))
    {
      ifMatch = false;
    }
  }
  return ifMatch;
}

std::string TuplePattern::serialize() const
{
  std::string serialized;
  for (TuplePatternElem t : this->m_elements)
  {
    serialized += t.serialize();
  }
  serialized += std::to_string(this->m_size);
  return serialized;
}

void TuplePattern::setElements(std::vector<TuplePatternElem> v)
{
  m_elements = v;
}

void TuplePattern::setSize(int s)
{
  m_size = s;
}

void TuplePattern::deserialize(std::string serialized)
{
  std::vector<TuplePatternElem> v;
  for (int i = 0; i < MAX_N_OF_ELEMENTS; i++)
  {
    TuplePatternElem elem = TuplePatternElem();
    elem.deserialize(serialized.substr(i * (MAX_VALUE_SIZE + 1), (i + 1) * (MAX_VALUE_SIZE + 1)));
    v.push_back(elem);
  }
  const char* s = &serialized[MAX_N_OF_ELEMENTS * (MAX_VALUE_SIZE + 1)];
  setSize(atoi(s));
  setElements(v);
}

bool operator==(const TuplePattern& t1, const TuplePattern& t2)
{
  if (t1.m_size != t2.m_size)
  {
    return false;
  }
  bool isEqual = true;
  for (int i = 0; i < t1.m_size; i++)
  {
    if (!(t1.getElements()[i] == t2.getElements()[i]))
    {
      isEqual = false;
    }
  }
  return isEqual;
}

std::ostream& operator<<(std::ostream& out, const TuplePattern& tuple)
{
  out << "(";
  for (int i = 0; i < tuple.m_size; i++)
  {
    out << tuple.getElements()[i];
    if ((i + 1) < tuple.m_size)
      out << ";";
  }
  out << ")";
  return out;
}
