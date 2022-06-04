#include <iostream>
#include "Tuple.h"
#include "TupleElem.h"
#include "TuplePattern.h"
#include "TuplePatternElem.h"
#include "Constants.h"

Tuple createTuple(const std::vector<std::string> values, const std::vector<ElemType> types, int size) 
{
    std::vector<TupleElem> v;
    for (int i = 0; i < MAX_N_OF_ELEMENTS; i++){
        std::string val = values[i];
        for (int i = 0; i < MAX_VALUE_SIZE-values[i].length()-1;i++) {
            val += " ";
        }
        val[MAX_VALUE_SIZE-1] = '\0';
        TupleElem elem = TupleElem(val, types[i]);
        v.push_back(elem);
    }
    Tuple tup = Tuple(v, size);
    return tup;
}

TuplePattern createTuplePattern(const std::vector<std::string> values, const std::vector<ElemType> types, const std::vector<MatchCondition> conditions, int size) 
{
    std::vector<TuplePatternElem> v;
    for (int i = 0; i < MAX_N_OF_ELEMENTS; i++){
        std::string val = values[i];
        for (int i = 0; i < MAX_VALUE_SIZE-values[i].length()-1;i++) {
            val += " ";
        }
        val[MAX_VALUE_SIZE-1] = '\0';
        TuplePatternElem elem = TuplePatternElem(val, types[i], conditions[i]);
        v.push_back(elem);
    }
    TuplePattern tup = TuplePattern(v, size);
    return tup;
}