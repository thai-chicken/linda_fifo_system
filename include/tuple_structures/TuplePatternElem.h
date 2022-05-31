#ifndef TUPLEPATTERNELEM_H
#define TUPLEPATTERNELEM_H

#include "../Enums.h"
#include "../Constants.h"
#include "../tuple_structures/TupleElem.h"
#include <string>

// Element wzorca krotki ma rozmiar MAX_VALUE_SIZE+1 B
class TuplePatternElem
{
private:
    char m_value [MAX_VALUE_SIZE];
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

    friend std::ostream& operator<< (std::ostream& out, const TuplePatternElem& elem);
    friend bool operator== (const TuplePatternElem& te1, const TuplePatternElem& te2);

};

TuplePatternElem::TuplePatternElem() {}

TuplePatternElem::TuplePatternElem(std::string value, ElemType type, MatchCondition cond) {
    strncpy(m_value, value.c_str(), MAX_VALUE_SIZE);
    m_type = type;
    m_cond = cond;
}


TuplePatternElem::~TuplePatternElem() {}

std::string TuplePatternElem::getValue() const {
    std::string out;
    int i = 0;
    while (m_value[i] != '\0') {
        out += m_value[i];
        i += 1;
    }

    return out;
}

ElemType TuplePatternElem::getElemType() const {
    return m_type;
}

MatchCondition TuplePatternElem::getMatchCond() const {
    return m_cond;
}

std::string TuplePatternElem::serialize() {
    std::string serialized;
    serialized += this->m_value;
    for (int i =0; i < MAX_VALUE_SIZE-serialized.length()-1; i++) {
        serialized += " ";
    }
    serialized += std::to_string(this->m_type);
    serialized += std::to_string(this->m_cond);
    return serialized;
}

void TuplePatternElem::deserialize(std::string serialized) {
    strncpy(m_value, serialized.substr(0, MAX_VALUE_SIZE-1).c_str(), MAX_VALUE_SIZE);
    m_value[MAX_VALUE_SIZE-1] = '\0';
    m_type = static_cast<ElemType>(atoi(serialized.substr(MAX_VALUE_SIZE-1, 1).c_str()));
    m_cond = static_cast<MatchCondition>(atoi(serialized.substr(MAX_VALUE_SIZE, 1).c_str()));
}

bool TuplePatternElem::matches(TupleElem& tupleElem) {
    if (this->m_type != tupleElem.getElemType() || this->m_type == ElemType::EMPTY || tupleElem.getElemType() == ElemType::EMPTY) {
        return false;
    }

    if (this->m_cond == MatchCondition::ANY) {
        return true;
    }

    switch (this->m_type)
    {
    case ElemType::INT:
        return this->intMatches(tupleElem);
    case ElemType::FLOAT:
        return this->floatMatches(tupleElem);
    case ElemType::STRING:
        return this->stringMatches(tupleElem);
    
    default:
        return false;
    }
};



bool TuplePatternElem::intMatches(TupleElem& tupleElem) {
    int realTupleValue = atoi(tupleElem.getValue().c_str());
    int realPatternValue = atoi(this->m_value);
    switch (this->m_cond)
    {
    case MatchCondition::EQUAL:
        return realPatternValue == realTupleValue;
        
    case MatchCondition::GREATER:
        return realTupleValue > realPatternValue;

    case MatchCondition::GEQ:
        return realTupleValue >= realPatternValue;
    
    case MatchCondition::LEQ:
        return realTupleValue <= realPatternValue;

    case MatchCondition::LESS:
        return realTupleValue < realPatternValue;
    
    default:
        break;
    }
    return false;
}

bool TuplePatternElem::floatMatches(TupleElem& tupleElem) {
    int realTupleValue = atof(tupleElem.getValue().c_str());
    int realPatternValue = atof(this->m_value);
    switch (this->m_cond)
    {
    case MatchCondition::EQUAL:
        return realPatternValue == realTupleValue;
        
    case MatchCondition::GREATER:
        return realTupleValue > realPatternValue;

    case MatchCondition::GEQ:
        return realTupleValue >= realPatternValue;
    
    case MatchCondition::LEQ:
        return realTupleValue <= realPatternValue;

    case MatchCondition::LESS:
        return realTupleValue < realPatternValue;
    
    default:
        break;
    }
    return false;
}

bool TuplePatternElem::stringMatches(TupleElem& tupleElem) {
    switch (this->m_cond)
    {
    case MatchCondition::EQUAL:
        return this->m_value == tupleElem.getValue();
        
    case MatchCondition::GREATER:
        return tupleElem.getValue() > this->m_value;

    case MatchCondition::GEQ:
        return tupleElem.getValue() >= this->m_value;
    
    case MatchCondition::LEQ:
        return tupleElem.getValue() <= this->m_value;

    case MatchCondition::LESS:
        return tupleElem.getValue() < this->m_value;
    
    default:
        break;
    }
    return false;
}

bool operator== (const TuplePatternElem& te1, const TuplePatternElem& te2) {
    return
    (te1.getValue()==te2.getValue()) &
    (te1.m_type == te2.m_type) &
    (te1.m_cond == te2.m_cond);
}


std::ostream& operator<< (std::ostream& out, const TuplePatternElem& elem) {
    
    out << "value:" << elem.getValue() << " type:" << elem.getElemType() << " matchCondition: " << elem.getMatchCond();
    return out;
}

#endif