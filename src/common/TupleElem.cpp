#include "TupleElem.h"


TupleElem::TupleElem() {}

TupleElem::TupleElem(std::string value, ElemType type) {
    strncpy(m_value, value.c_str(), MAX_VALUE_SIZE);
    m_type = type;
}

TupleElem::~TupleElem() {}

std::string TupleElem::getValue() const {
    std::string out;
    int i = 0;
    while (m_value[i] != '\0') {
        out += m_value[i];
        i += 1;
    }

    return out;
}

int TupleElem::getOffset() const {
    return m_offset;
}

ElemType TupleElem::getElemType() const {
    return m_type;
}

std::string TupleElem::serialize() {
    std::string serialized;
    serialized += this->m_value;
    for (int i =0; i < MAX_VALUE_SIZE-serialized.length()-1; i++) {
        serialized += " ";
    }
    serialized += std::to_string(this->m_type);
    serialized += std::to_string(this->m_offset);
    return serialized;

}

// Tupla sklada sie ze stringa MAX_VALUE_SIZE+1 elementowego
void TupleElem::deserialize(std::string serialized) {
    strncpy(m_value, serialized.substr(0, MAX_VALUE_SIZE-1).c_str(), MAX_VALUE_SIZE);
    m_value[MAX_VALUE_SIZE-1] = '\0';
    m_type = static_cast<ElemType>(atoi(serialized.substr(MAX_VALUE_SIZE-1, 1).c_str()));
    m_offset = atoi(serialized.substr(MAX_VALUE_SIZE, 1).c_str());
}

bool operator== (const TupleElem& te1, const TupleElem& te2) {
    return
    (te1.getValue()==te2.getValue()) &
    (te1.m_type == te2.m_type);
}


std::ostream& operator<< (std::ostream& out, const TupleElem& elem) {
    
    out << "value:" << elem.getValue() << " type:" << elem.getElemType() << " offset: " << elem.getOffset();
    return out;
}

