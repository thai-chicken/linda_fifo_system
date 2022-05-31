#ifndef ENUMS_H
#define ENUMS_H

enum ElemType {
    STRING,
    INT,
    FLOAT,
    EMPTY
};

enum Command {
    INPUT,
    OUTPUT,
    READ
};

enum MatchCondition {
    ANY,
    EQUAL,
    LESS,
    LEQ,
    GREATER,
    GEQ
};

#endif