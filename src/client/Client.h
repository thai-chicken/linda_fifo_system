#ifndef CLIENT_H
#define CLIENT_H

#include "../common/Message.h"
#include "../common/tuples.pb.h"

#include <boost/algorithm/string.hpp>
#include <string>
#include <sys/stat.h>

#define FIFO_MAIN_PATH "/tmp/fifo_main"

class Client
{
  public:
  Client();
  ~Client();
  void action(std::string msg, std::string cmd, int timeout=0);

  private:
  std::string fifo_name;
  void create_fifo();
  FILE* open_main_fifo() const;
  FILE* open_own_fifo() const;
  void receive_msg();
  void send_msg(std::string msg, std::string cmd, int timeout);
  void destroy_own_fifo() const;
  static void close_main_fifo(FILE* fd_client);
  static void close_own_fifo(FILE* fd_own_fifo);
};

#endif