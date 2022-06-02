#ifndef SERVER_H
#define SERVER_H

#include "../common/Message.h"
// #include "../common/Semaphore.h"
#include "../common/tuples.pb.h"
#include "RequestContainer.h"
#include "TupleContainer.h"
#include "boost/algorithm/string.hpp"

#include <fcntl.h>
#include <sys/stat.h>
#include <thread>

#define FIFO_MAIN_PATH "/tmp/fifo_main"

class Server
{
  public:
  Server();
  ~Server();
  void run();
  void show_state();

  private:
  RequestContainer request_container;
  TupleContainer tuple_container;
  std::mutex mtx_request; // mutex controlling access to request_container
  std::mutex mtx_tuple; // mutex controlling access to tuple_container

  std::string fifo_name;
  bool quit;

  void handle_requests();
  void perform_request(Message msg);
  void perform_tuple(Message msg);
  FILE* open_main_fifo() const;
  Message get_msg_deserialized(FILE* fd_main);
  void handle_msg(Message msg_in);
  void create_main_fifo();
  void send_to_client(std::string comand, std::string msg, pid_t pid);

  static FILE* open_client_fifo(pid_t pid);
  static void destruct_fifo();
};

#endif
