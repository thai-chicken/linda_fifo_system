#ifndef TUPLEPATTERNMESSAGE_H
#define TUPLEPATTERNMESSAGE_H

#include "Enums.h"
#include "Message.h"
#include "TuplePattern.h"

class TuplePatternMessage : public Message {
private:
    TuplePattern pattern;
public:
    TuplePatternMessage() {};
    virtual ~TuplePatternMessage() {};
    TuplePattern getTuplePattern();
    void setTuplePattern(TuplePattern t);

    virtual std::string serialize() {
        std::string serialized;
        serialized += std::to_string(this->getPid());
        for (int i =0; i < PID_MAX_DIGITS-serialized.length(); i++) {
            serialized += " ";
        }
        serialized += std::to_string(this->getType());
        serialized += std::to_string(this->getCommand());
        serialized += this->pattern.serialize();
        return serialized;
    };
    virtual void deserialize(std::string serialized) {
        this->setPid( atoi(serialized.substr(0, PID_MAX_DIGITS).c_str()));
        this->setType( static_cast<MessageType>(serialized[PID_MAX_DIGITS]));
        this->setCommand( static_cast<Command>(serialized[PID_MAX_DIGITS+1]));
        // Wzorzec zajmuje MAX_N_OF_ELEMENTS*(MAX_VALUE_SIZE+1) + 1 bajty
        this->pattern.deserialize(serialized.substr(PID_MAX_DIGITS+2, MAX_N_OF_ELEMENTS*(MAX_VALUE_SIZE+1) + 1).c_str());
    };
};

#endif