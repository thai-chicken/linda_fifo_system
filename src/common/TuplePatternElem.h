#ifndef TUPLEPATTERNELEM_H
#define TUPLEPATTERNELEM_H

#include "Constants.h"
#include "Enums.h"
#include "TupleElem.h"

#include <string>

class TuplePatternElem
{
  private:
  char m_value[MAX_VALUE_SIZE];
  ElemType m_type;
  MatchCondition m_cond;

  bool intMatches(TupleElem& tupleElem);
  bool floatMatches(TupleElem& tupleElem);
  bool stringMatches(TupleElem& tupleElem);


  public:
  TuplePatternElem();
  TuplePatternElem(std::string value, ElemType type, MatchCondition cond);
  ~TuplePatternElem();

  bool matches(TupleElem& elem);

  std::string getValue() const;
  ElemType getElemType() const;
  MatchCondition getMatchCond() const;

  std::string serialize();
  void deserialize(std::string serialized);

  friend std::ostream& operator<<(std::ostream& out, const TuplePatternElem& elem);
  friend bool operator==(const TuplePatternElem& te1, const TuplePatternElem& te2);
};

#endif