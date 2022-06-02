#include "common/tuples.pb.h"

#include <fcntl.h>
#include <iostream>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
// #include "src/common/tuples.pb.h"
#include "common/Message.h"


pid_t get_process_pid()
{
  // Get the pid of the current process.
  return getpid();
}

std::string get_fifo_name()
{
  return std::string("/tmp/fifo_") + std::to_string(get_process_pid());
}

std::string create_fifo()
{
  // Create a fifo.
  std::string fifo_name = get_fifo_name();
  umask(0);
  if (mknod(fifo_name.c_str(), S_IFIFO | 0666, 0) < 0)
  {
    perror("CLIENT | Error creating main fifo | ");
    exit(1);
  }
  return fifo_name;
}

FILE* open_main_fifo()
{
  // Open the main fifo.
  std::string main_fifo_path = "/tmp/fifo_main";
  FILE* fd_main;
  if ((fd_main = fopen(main_fifo_path.c_str(), "w")) == NULL)
  {
    perror("CLIENT | Error opening main fifo | ");
    exit(1);
  }
  else
  {
    printf("CLIENT | Opened main fifo!\n");
  }
  return fd_main;
}

void send_msg(FILE* fd_main, std::string command, std::string msg)
{
  pid_t pid = get_process_pid();
  tuples::Message message;
  message.set_pid(int(pid));
  message.set_msg(msg);
  message.set_command(command);

  std::string buffer;
  message.SerializeToString(&buffer);

  printf("CLIENT | Sending message: %s with size: %lu\n", buffer.c_str(), sizeof(buffer.c_str()));
  fputs(buffer.c_str(), fd_main);
  fflush(fd_main);
  printf("CLIENT | Sent message: %s with size: %lu\n", buffer.c_str(), sizeof(buffer.c_str()));
}


FILE* open_own_fifo()
{
  std::string fifo_path = create_fifo();
  printf("CLIENT | Created fifo: %s\n", fifo_path.c_str());

  FILE* fd_cl;
  if ((fd_cl = fopen(fifo_path.c_str(), "r")) == NULL)
  {
    perror("CLIENT | Error opening client fifo.");
    exit(1);
  }
  else
  {
    printf("CLIENT | Opened client fifo: %s\n", fifo_path.c_str());
  }

  return fd_cl;
}

void receive_msg(FILE* fd_cl)
{
  Message received_message;
  int n;
  tuples::Message message_serialized;
  char buffer[MSG_SIZE];

  if (fgets(buffer, MSG_SIZE, fd_cl) == NULL)
  {
    perror("CLIENT | Error reading from main fifo.");
  }

  message_serialized.ParseFromString(buffer);
  received_message.pid = message_serialized.pid();
  received_message.msg = message_serialized.msg();
  printf("CLIENT | Message received: %s and pid received: %d \n", received_message.msg.c_str(), received_message.pid);
}


int main(int argc, char const* argv[])
{
  if (argc != 3)
  {
    printf("CLIENT | Usage: ./client <command> <message>\n");
    exit(1);
  }
  std::string msg = argv[2];
  std::string command = argv[1];

  printf("CLIENT | My fifo: %s\n", get_fifo_name().c_str());
  FILE* fd_cl;

  FILE* fd_main = open_main_fifo();
  send_msg(fd_main, command, msg);

  if (fclose(fd_main) != 0)
  {
    perror("CLIENT | Error closing main fifo | ");
    exit(1);
  }
  else
  {
    printf("CLIENT | Closed main fifo.\n");
  }


  fd_cl = open_own_fifo();
  receive_msg(fd_cl);


  std::string fifo_path = get_fifo_name();
  if (unlink(fifo_path.c_str()) == -1)
  {
    perror("CLIENT | Error unlinking fifo | ");
    exit(1);
  }
  else
  {
    printf("CLIENT | Unlinked fifo: %s\n", fifo_path.c_str());
  }

  return 0;
}