#include "client/Client.h"
#include "common/TupleMessage.h"
#include "common/TupleElem.h"
// #include "common/Constants.h"
// #include "common/tuple_utils.h"

Tuple createTuple(std::vector<std::string> values, std::vector<ElemType> types, int size) 
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

TuplePattern createTuplePattern(std::vector<std::string> values, std::vector<ElemType> types, const std::vector<MatchCondition> conditions, int size) 
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


std::string getElemValue(int num) {
    bool valid = false;
    std::string elemValue;
    while (!valid) {
        std::string value;
        std::cout << "Enter value of element number "<<num<<": " << std::endl;
        std::getline(std::cin, value);
        if (value == "\n") {
            elemValue = "";
            valid = true;
        }
        else if (value.size() > MAX_VALUE_SIZE) {
            std::cout << "Invalid size of value. Maximum value size: " << MAX_VALUE_SIZE << std::endl;
        }
        else {
            elemValue = value;
            valid = true;
        }
    }
    return elemValue;
}

ElemType getElemType(int num) {
    ElemType elemType;
    bool valid = false;
    while (!valid) {
        std::string type;
        std::cout << "Enter type (int, str, float) of element number "<<num<<": " << std::endl;
        std::getline(std::cin, type);

        if (type == "\n") {
           elemType = ElemType::EMPTY;
           valid = true;
        }
        else if (type_map.find(type) == type_map.end()) {
            std::cout << "Invalid type. Try again " << std::endl;
        }
        else {
            valid = true;
            elemType = type_map.find(type)->second;
        }
    }
    return elemType;
}

MatchCondition getElemCondition(int num) {
    MatchCondition condition;
    bool valid = false;
    while (!valid) {
        std::string cond;
        std::cout << "Enter match condition (*,=,<,<=,>,>=) of element number "<<num<<": " << std::endl;
        std::getline(std::cin, cond);
        if (cond == "\n") {
            valid = true;
            condition = MatchCondition::ANY;
        }
        else if (cond_map.find(cond) == cond_map.end()) {
            std::cout << "Invalid match condition. Try again " << std::endl;
        }
        else {
            valid = true;
            condition = cond_map.find(cond)->second;
        }
    }

    return condition;
}


TuplePatternMessage createTuplePatternMessage(Command cmd) {
    std::string inp;
    do {
        std::cout << "Enter size of tuple pattern: " << std::endl;
        std::getline(std::cin, inp);
    } while (atoi(inp.c_str()) < 1 || atoi(inp.c_str()) > MAX_N_OF_ELEMENTS);
    int size = atoi(inp.c_str()) ;
    TuplePatternMessage patternMessage;
    patternMessage.setType(MessageType::PATTERN);
    std::vector<std::string> values;
    std::vector<ElemType> types;
    std::vector<MatchCondition> conditions;

    for (int i = 0; i < size; i++) {
        std::string value = getElemValue(i+1);
        values.push_back(value);
        ElemType type = getElemType(i+1);
        types.push_back(type);
        if (value != "") {
            MatchCondition cond = getElemCondition(i+1);
            conditions.push_back(cond);
        }
        else {
            conditions.push_back(MatchCondition::ANY);
        }
    }

    for (int i = 0; i < MAX_N_OF_ELEMENTS-size; i++) {
        values.push_back("");
        types.push_back(ElemType::EMPTY);
        conditions.push_back(MatchCondition::ANY);
    }

    TuplePattern tp = createTuplePattern(values, types, conditions, size);
    std::cout << "CREATED PATTERN: "<< tp << std::endl;
    patternMessage.setTuplePattern(tp);
    patternMessage.setCommand(cmd);
    patternMessage.setTimeout(9);
    return patternMessage;
}

TupleMessage createTupleMessage() {
    std::string inp;
    do {
        std::cout << "Enter size of tuple: " << std::endl;
        std::getline(std::cin, inp);
    } while (atoi(inp.c_str()) < 1 || atoi(inp.c_str()) > MAX_N_OF_ELEMENTS);
    int size = atoi(inp.c_str()) ;

    TupleMessage tupleMessage;
    tupleMessage.setType(MessageType::TUPLE);
    std::vector<std::string> values;
    std::vector<ElemType> types;

    for (int i = 0; i < size; i++) {
        std::string value = getElemValue(i+1);
        if (value != "") {
            ElemType type = getElemType(i+1);
            values.push_back(value);
            types.push_back(type);
        }
        else {
            values.push_back("");
            types.push_back(ElemType::EMPTY);
        }
    }

    for (int i = 0; i < MAX_N_OF_ELEMENTS-size; i++) {
        values.push_back("");
        types.push_back(ElemType::EMPTY);
    }
    

    Tuple t = createTuple(values, types, size);
    std::cout << "CREATED TUPLE: "<< t << std::endl;
    tupleMessage.setTuple(t);
    tupleMessage.setCommand(Command::OUTPUT);
    return tupleMessage;
}


int main(int argc, char const* argv[])
{
    std::cout << "Available commands are: input, output, read, exit" << std::endl;
    bool isEnd = false;
    while (!isEnd) {
        Client client = Client();
        std::string cmd;
        std::cout << "Siema, enter command: " << std::endl;
        std::getline(std::cin, cmd);
        boost::algorithm::to_lower(cmd);
        if (cmd == "exit") {
            isEnd = true;
            break;
        }

        if (cmd_map.find(cmd) == cmd_map.end()) {
            std::cout << "Invalid command, try again." << std::endl;
        }
        else {
            if (cmd == "input") {
                TuplePatternMessage msg =createTuplePatternMessage(Command::INPUT);
                client.action(&msg);
            }
            else if (cmd == "output") {
                TupleMessage msg = createTupleMessage();
                client.action(&msg);
            }
            else if (cmd == "read") {
                TuplePatternMessage msg =createTuplePatternMessage(Command::READ);
                client.action(&msg);
            }
        }
    }

  return 0;
}