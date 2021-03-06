#ifndef ENUMS_H
#define ENUMS_H

enum ElemType
{
  STRING,
  INT,
  FLOAT,
  EMPTY
};

enum MessageType
{
  TUPLE,
  PATTERN
};

enum Command
{
  INPUT,
  OUTPUT,
  READ,
  TIMEOUT
};

enum MatchCondition
{
  ANY,
  EQUAL,
  LESS,
  LEQ,
  GREATER,
  GEQ
};

#endif