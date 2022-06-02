#ifndef SERVER_H
#define SERVER_H

#include "../common/Message.h"
#include "../common/tuples.pb.h"
#include "RequestContainer.h"
#include "TupleContainer.h"
#include "boost/algorithm/string.hpp"

#include <fcntl.h>
#include <sys/stat.h>

#define FIFO_MAIN_PATH "/tmp/fifo_main"

class Server
{
  public:
  Server();
  ~Server();
  void run();
  void show_state() const;

  private:
  RequestContainer request_container;
  TupleContainer tuple_container;
  std::mutex mtx_request;
  std::mutex mtx_tuple;
  std::string fifo_name;
  bool quit;

  void handle_requests();
  FILE* open_main_fifo() const;
  Message get_msg_deserialized(FILE* fd_main);
  void handle_msg(Message msg_in);
  void create_main_fifo();

  static void send_to_client(Message message);
  static FILE* open_client_fifo(pid_t pid);
  static void destruct_fifo();
};

#endif
