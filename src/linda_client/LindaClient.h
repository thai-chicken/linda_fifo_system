#ifndef LINDACLIENT_H
#define LINDACLIENT_H

#include "../common/Constants.h"
#include "../common/Enums.h"
#include "../common/Message.h"
#include "../common/TupleMessage.h"
#include "../common/TuplePatternMessage.h"

#include <boost/algorithm/string.hpp>
#include <string>
#include <sys/stat.h>
#define FIFO_MAIN_PATH "/tmp/fifo_main"

class LindaClient
{
  public:
  LindaClient();
  ~LindaClient();
  void action(Message* msg);

  private:
  std::string fifo_name;
  void create_fifo();
  FILE* open_main_fifo() const;
  FILE* open_own_fifo() const;
  void receive_msg();
  void send_msg(Message* msg);
  void destroy_own_fifo() const;
  static void close_main_fifo(FILE* fd_client);
  static void close_own_fifo(FILE* fd_own_fifo);
};

#endif