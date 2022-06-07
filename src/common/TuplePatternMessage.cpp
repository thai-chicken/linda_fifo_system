#include "TuplePatternMessage.h"

TuplePattern TuplePatternMessage::getTuplePattern()
{
  return this->pattern;
}

void TuplePatternMessage::setTuplePattern(TuplePattern t)
{
  this->pattern = t;
}
