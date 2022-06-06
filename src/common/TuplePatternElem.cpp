#include "TuplePatternElem.h"

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
    std::cout << "realTupleValue: " << realTupleValue << std::endl;
    std::cout << "realPatternValue: " <<realPatternValue << std::endl;
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
        std::cout << "VAL1: " << this->m_value << " size: " << strlen(this->m_value) << std::endl;
        std::cout << "VAL2: " << tupleElem.getValue() << " size: " << tupleElem.getValue().length() << std::endl;
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
    std::string v;
    if (elem.getElemType() == ElemType::STRING) {
        v = elem.getValue();
        boost::algorithm::trim_right(v);
        v = "\""+v+"\"";
    }
    else {
        v = elem.getValue();
    }
    boost::algorithm::trim_right(v);
    out << type_str_map.find(elem.getElemType())->second << ":" << cond_str_map.find(elem.getMatchCond())->second << v;
    return out;
}