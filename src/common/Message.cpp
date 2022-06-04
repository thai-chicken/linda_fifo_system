#include "Message.h"

pid_t Message::getPid() {
    return this->pid;
}

Command Message::getCommand() {
    return this->cmd;
}

MessageType Message::getType() {
    return this->type;
}


void Message::setPid(pid_t p) {
    this->pid = p;
}

void Message::setCommand(Command cmd) {
    this->cmd = cmd;
}

void Message::setType(MessageType t) {
    this->type = t;
}





// std::string Message::serialize() {
//     std::string serialized;
//     serialized += std::to_string(this->pid);
//     for (int i =0; i < PID_MAX_DIGITS-serialized.length(); i++) {
//         serialized += " ";
//     }
//     serialized += std::to_string(this->type);
//     serialized += std::to_string(this->cmd);
//     return serialized;
// }

// void Message::deserialize(std::string serialized) {
//     this->pid = atoi(serialized.substr(0, PID_MAX_DIGITS).c_str());
//     this->type = static_cast<MessageType>(serialized[PID_MAX_DIGITS]);
//     this->cmd = static_cast<Command>(serialized[PID_MAX_DIGITS+1]);
// }