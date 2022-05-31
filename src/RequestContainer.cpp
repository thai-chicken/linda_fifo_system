#include "RequestContainer.h"
#include <iostream>

RequestContainer::RequestContainer(){
    requests={};
}

RequestContainer::~RequestContainer(){
    for (std::list<Request*>::iterator it = requests.begin(); it != requests.end(); ++it){
        delete *it;
    }
}

void RequestContainer::add(const Request request){
    requests.push_back(new Request(request.get_request(), request.get_request_pid(), request.get_timeout_pid()));
}

Request RequestContainer::get(int index) const{
    if (requests.size() > index){
        std::list<Request*>::const_iterator it = requests.begin();
        std::advance(it, index);
        return *(*it);
    }
    else{
        return Request();
    }
}

void RequestContainer::remove(int index){
    if (requests.size() > index){
        std::list<Request*>::const_iterator it = requests.begin();
        std::advance(it, index);
        delete *it;
        requests.erase(it);
    }
}

int RequestContainer::size() const{
    return requests.size();
}

bool RequestContainer::is_in(const Request request) const{
    for (std::list<Request*>::const_iterator it = requests.begin(); it != requests.end(); ++it){
        if ((*it)->get_request() == request.get_request()){
            return true;
        }
    }
    return false;
}

void RequestContainer::show_elems() const{
    if (requests.size() > 0){
        for (std::list<Request*>::const_iterator it = requests.begin(); it != requests.end(); ++it){
            std::cout << "Request: " << (*it)->get_request() << " | Client_pid: " << (*it)->get_request_pid() << " | Timeout_pid: " << (*it)->get_timeout_pid() <<std::endl;
        }
    }
    else{
        std::cout << "No requests in container!" << std::endl;
    }
}



