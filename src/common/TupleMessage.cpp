#include "TupleMessage.h"


Tuple TupleMessage::getTuple() {
    return this->tuple;
}

void TupleMessage::setTuple(Tuple t) {
    this->tuple = t;
}

// std::string TupleMessage::serialize() {
//     std::string serialized;
//     serialized += std::to_string(this->getPid());
//     for (int i =0; i < PID_MAX_DIGITS-serialized.length(); i++) {
//         serialized += " ";
//     }
//     serialized += std::to_string(this->getType());
//     serialized += std::to_string(this->getCommand());
//     serialized += this->tuple.serialize();
//     return serialized;
// }

//1234511
// void TupleMessage::deserialize(std::string serialized) {
//     // Wiadomosc zajmuje 7B
//     this->setPid( atoi(serialized.substr(0, PID_MAX_DIGITS).c_str()));
//     this->setType( static_cast<MessageType>(serialized[PID_MAX_DIGITS]));
//     this->setCommand( static_cast<Command>(serialized[PID_MAX_DIGITS+1]));
//     // Tupla zajmuje MAX_N_OF_ELEMENTS*(MAX_VALUE_SIZE+1) + 1 bajty
//     this->tuple.deserialize(serialized.substr(PID_MAX_DIGITS+2, MAX_N_OF_ELEMENTS*(MAX_VALUE_SIZE+1) + 1).c_str());

// }