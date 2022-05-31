#include <iostream>

#include "../../include/tuple_structures/TuplePatternElem.h"
#include "../../include/tuple_structures/TuplePattern.h"
#include "../../include/tuple_structures/TupleElem.h"
#include "../../include/tuple_structures/Tuple.h"
#include "../../include/Enums.h"
#include "../../include/Constants.h"
#include "../../include/tuple_structures/tuple_utils.h"

// Krotki z tresci zadania
Tuple tup1 = createTuple(
    {"1", "abc", "3.1415", "d", "*"}, 
    {ElemType::INT, ElemType::STRING, ElemType::FLOAT, ElemType::STRING, ElemType::EMPTY}, 4);
Tuple tup2 = createTuple(
    {"10", "abc", "3.1415", "*", "*"}, 
    {ElemType::INT, ElemType::STRING, ElemType::FLOAT, ElemType::EMPTY, ElemType::EMPTY}, 3);
Tuple tup3 = createTuple(
    {"2", "3", "1", "Ala ma kota", "*"}, 
    {ElemType::INT, ElemType::INT, ElemType::INT, ElemType::STRING, ElemType::EMPTY}, 4);

// Wzorce z tresci zadania
TuplePattern tp1 = createTuplePattern(
    {"1", "*", "*", "d", "*"}, 
    {ElemType::INT, ElemType::STRING, ElemType::FLOAT, ElemType::STRING, ElemType::EMPTY}, 
    {MatchCondition::EQUAL, MatchCondition::ANY, MatchCondition::ANY, MatchCondition::EQUAL, MatchCondition::ANY}, 4);
TuplePattern tp2 = createTuplePattern(
    {"0", "abc", "*", "*", "*"}, 
    {ElemType::INT, ElemType::STRING, ElemType::FLOAT, ElemType::EMPTY, ElemType::EMPTY}, 
    {MatchCondition::GREATER, MatchCondition::EQUAL, MatchCondition::ANY, MatchCondition::ANY, MatchCondition::ANY}, 3);

// Pomocnicza funkcja do tworzenia serializowanych tupli
std::string createSerialized(Tuple& tup) {
    std::string serialized;
    for (int i = 0; i < MAX_N_OF_ELEMENTS; i++){
        std::string val = tup.getElements()[i].getValue();
        serialized += val;
        for (int i = 0; i < MAX_VALUE_SIZE-val.length()-1;i++) {
            serialized += " ";
        }
        serialized += std::to_string(tup.getElements()[i].getElemType());
        serialized += std::to_string(tup.getElements()[i].getOffset());     
    }
    serialized += std::to_string(tup.getSize());
    return serialized;
}

// Pomocnicza funkcja do tworzenia serializowanych wzorcow
std::string createSerialized(TuplePattern& tup) {
    std::string serialized;
    for (int i = 0; i < MAX_N_OF_ELEMENTS; i++){
        std::string val = tup.getElements()[i].getValue();
        serialized += val;
        for (int i = 0; i < MAX_VALUE_SIZE-val.length()-1;i++) {
            serialized += " ";
        }
        serialized += std::to_string(tup.getElements()[i].getElemType());
        serialized += std::to_string(tup.getElements()[i].getMatchCond());     
    }
    serialized += std::to_string(tup.getSize());
    return serialized;
}

//TODO: Pewnie do przepisania na ladne testy:)
bool testMatching() {
    bool match1 = tp1.matches(tup1);
    bool match2 = tp2.matches(tup2);
    return match1 & match2;
}

bool testSerialitationTuple() {
    std::string target = createSerialized(tup1);
    std::string serialized = tup1.serialize();
    bool match1 = (target==serialized);
    return match1;
}

bool testDeserializationTuple() {
    Tuple tmp = Tuple();
    std::string target = createSerialized(tup1);
    tmp.deserialize(target);
    bool match1 = (tmp == tup1);
    return match1;
}

bool testDoubleSerializationTuple() {
    Tuple tmp = Tuple();
    Tuple tmp2 = Tuple();
    std::string target = createSerialized(tup1);
    tmp.deserialize(target);
    tmp2.deserialize(tmp.serialize());
    bool match1 = (tmp == tmp2);
    return match1;
}

bool testSerialitationTuplePattern() {
    std::string target = createSerialized(tp1);
    std::string serialized = tp1.serialize();
    bool match1 = (target==serialized);
    return match1;
}

bool testDeserializationTuplePattern() {
    TuplePattern tmp = TuplePattern();
    std::string target = createSerialized(tp1);
    tmp.deserialize(target);
    bool match1 = (tmp == tp1);
    return match1;
}

bool testDoubleSerializationTuplePattern() {
    TuplePattern tmp = TuplePattern();
    TuplePattern tmp2 = TuplePattern();
    std::string target = createSerialized(tp1);
    tmp.deserialize(target);
    tmp2.deserialize(tmp.serialize());
    bool match1 = (tmp == tmp2);
    return match1;
}

int main () {
    if (!testMatching()) {
        std::cout << "Test tuple matching failed!" << std::endl;
    } else {
        std::cout << "Test tuple matching passed!" << std::endl;
    }
    if (!testSerialitationTuple()) {
        std::cout << "Test tuple serialization failed!" << std::endl;
    } else {
        std::cout << "Test tuple serialization passed!" << std::endl;
    }
    if (!testDeserializationTuple()) {
        std::cout << "Test tuple deserialization failed!" << std::endl;
    } else {
        std::cout << "Test tuple deserialization passed!" << std::endl;
    }
    if (!testDoubleSerializationTuple()) {
        std::cout << "Test tuple double deserialization failed!" << std::endl;
    } else {
        std::cout << "Test tuple double deserialization passed!" << std::endl;
    }
    if (!testSerialitationTuplePattern()) {
        std::cout << "Test tuple pattern serialization failed!" << std::endl;
    } else {
        std::cout << "Test tuple pattern serialization passed!" << std::endl;
    }
    if (!testDeserializationTuplePattern()) {
        std::cout << "Test tuple pattern deserialization failed!" << std::endl;
    } else {
        std::cout << "Test tuple pattern deserialization passed!" << std::endl;
    }
    if (!testDoubleSerializationTuplePattern()) {
        std::cout << "Test tuple pattern double deserialization failed!" << std::endl;
    } else {
        std::cout << "Test tuple pattern double deserialization passed!" << std::endl;
    }
    return 0;
}