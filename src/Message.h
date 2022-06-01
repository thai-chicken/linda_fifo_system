#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

struct Message {
    pid_t pid;
    std::string command;
    std::string msg;
};

#endif // MESSAGE_H