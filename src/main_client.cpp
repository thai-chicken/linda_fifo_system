#include "client/Client.h"
#include "common/TupleMessage.h"
#include "common/TupleElem.h"
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

int main(int argc, char const* argv[])
{
  // if (argc != 3)
  // {
  //   printf("CLIENT | Usage: ./client <command> <message>\n");
  //   exit(1);
  // }
  // std::string msg = argv[2];
  // std::string command = argv[1];
  TupleMessage tupleMessage;
  tupleMessage.setType(MessageType::TUPLE);
  Tuple tup1 = createTuple(
    {"1", "abc", "3.1415", "d", "*"}, 
    {ElemType::INT, ElemType::STRING, ElemType::FLOAT, ElemType::STRING, ElemType::EMPTY}, 4);
  tupleMessage.setTuple(tup1);
  tupleMessage.setCommand(Command::INPUT);
  Client client = Client();
  client.action(&tupleMessage);
  return 0;
}