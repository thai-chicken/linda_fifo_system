#ifndef TUPLE_H
#define TUPLE_H

#include "../Constants.h"
#include "../Enums.h"
#include "./TupleElem.h"
#include <iostream>
#include <vector>
#include <string.h>

// Tupla ma MAX_N_OF_ELEMENTS*(MAX_VALUE_SIZE+1) + 1 bajty
class Tuple
{
private:
    std::vector<TupleElem> m_elements;
    int m_size;

public:
    Tuple() {};
    Tuple(std::vector<TupleElem>& elements, int size);
    ~Tuple();

    std::vector<TupleElem> getElements() const {return m_elements;} ;
    std::string serialize() const;

    void setElements(std::vector<TupleElem> v);
    void setSize(int s);
    void deserialize(std::string serialized);

    int getSize() {return m_size;};
    friend std::ostream& operator<< (std::ostream& out, const Tuple& tuple);
    friend bool operator== (const Tuple& t1, const Tuple& t2);
};

Tuple::Tuple(std::vector<TupleElem>& elements, int size) {
    m_elements = elements;
    m_size = size;
}

Tuple::~Tuple() {}

std::ostream& operator<< (std::ostream& out, const Tuple& tuple) {
    out << "(";
    for (TupleElem t: tuple.m_elements) {
        out << t << ";";
    }
    out << ")";
    return out;
}

std::string Tuple::serialize() const {
    std::string serialized;
    for (TupleElem t: this->m_elements) {
        serialized += t.serialize();
    }
    serialized += std::to_string(this->m_size);
    return serialized;
}

void Tuple::setElements(std::vector<TupleElem> v ) {
    m_elements = v;
}

void Tuple::setSize(int s) {
    m_size = s;
}

void Tuple::deserialize(std::string serialized) { 
    std::vector<TupleElem> v;
    for (int i = 0; i < MAX_N_OF_ELEMENTS; i++) {
        TupleElem elem = TupleElem();
        elem.deserialize(serialized.substr(i*(MAX_VALUE_SIZE+1), (i+1)*(MAX_VALUE_SIZE+1)));
        v.push_back(elem);
    }
    const char* s = &serialized[MAX_N_OF_ELEMENTS*(MAX_VALUE_SIZE+1)];
    setSize(atoi(s));
    setElements(v);
}

bool operator== (const Tuple& t1, const Tuple& t2) {
    if (t1.m_size != t2.m_size) {
        return false;
    }
    bool isEqual = true;
    for (int i = 0; i < t1.m_size; i++) {
        if (!(t1.getElements()[i] == t2.getElements()[i])) {
            isEqual = false;
        }
    }
    return isEqual;
}

#endif