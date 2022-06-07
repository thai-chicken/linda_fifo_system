#ifndef CONSTANTS_H
#define CONSTANTS_H
#include "Enums.h"

#include <map>
#include <string>

const int MAX_N_OF_ELEMENTS = 5;
const int MAX_VALUE_SIZE = 32;
const int PID_MAX_DIGITS = 5;
const int MESSAGE_SIZE = (MAX_N_OF_ELEMENTS * (MAX_VALUE_SIZE + 1) + 1 + 11);

const int MAX_TIMEOUT_LENGTH = 3;
const int MAX_TIMEOUT = 999;

const std::map<std::string, Command> cmd_map{
    {"input", Command::INPUT},
    {"output", Command::OUTPUT},
    {"read", Command::READ},
};

const std::map<std::string, ElemType> type_map{
    {"int", ElemType::INT},
    {"float", ElemType::FLOAT},
    {"str", ElemType::STRING},
    {"empty", ElemType::EMPTY},
};

const std::map<ElemType, std::string> type_str_map{
    {ElemType::INT, "int"},
    {ElemType::FLOAT, "float"},
    {ElemType::STRING, "str"},
    {ElemType::EMPTY, ""},
};

const std::map<std::string, MatchCondition> cond_map{{"*", MatchCondition::ANY},
                                                     {"=", MatchCondition::EQUAL},
                                                     {"<", MatchCondition::LESS},
                                                     {">", MatchCondition::GREATER},
                                                     {"<=", MatchCondition::LEQ},
                                                     {">=", MatchCondition::GEQ}};

const std::map<MatchCondition, std::string> cond_str_map{{MatchCondition::ANY, "*"},
                                                         {MatchCondition::EQUAL, ""},
                                                         {MatchCondition::LESS, "<"},
                                                         {MatchCondition::GREATER, ">"},
                                                         {MatchCondition::LEQ, "<="},
                                                         {MatchCondition::GEQ, ">="}};

#endif