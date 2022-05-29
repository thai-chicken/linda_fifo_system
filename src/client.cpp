#include <fcntl.h>
#include <iostream>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "tuples.pb.h"

#define MSG_SIZE 32


struct Message
{
  pid_t pid;
  std::string msg;
};

pid_t get_process_pid()
{
  // Get the pid of the current process.
  return getpid();
}

std::string get_fifo_name(){
  return std::string("/tmp/fifo_") + std::to_string(get_process_pid());
}

std::string create_fifo()
{
  // Create a fifo.
  std::string fifo_name = get_fifo_name();
  umask(0);
  if (mknod(fifo_name.c_str(), S_IFIFO | 0666, 0) < 0)
  {
    perror("CLIENT | Error creating main fifo.");
  }
  return fifo_name;
}

FILE* open_main_fifo()
{
  // Open the main fifo.
  std::string main_fifo_path = "/tmp/fifo_main";
  FILE* fd_main = fopen(main_fifo_path.c_str(), "w");
  return fd_main;
}

void send_msg(FILE* fd_main)
{
  std::string msg = "Hello, server!";
  pid_t pid = get_process_pid();
  tuples::Message message;
  message.set_pid(int(pid));
  message.set_msg(msg);

  std::string buffer;
  message.SerializeToString(&buffer);
  
  printf("CLIENT | Sending message: %s with size: %lu\n", buffer.c_str(), sizeof(buffer));
  fputs(buffer.c_str(), fd_main);
  fflush(fd_main);
  printf("CLIENT | Sent message: %s with size: %lu\n", buffer.c_str(), sizeof(buffer));
}


FILE* open_own_fifo(){
  std::string fifo_path = create_fifo();
  printf("CLIENT | Created fifo: %s\n", fifo_path.c_str());
  
  FILE* fd_cl = fopen(fifo_path.c_str(), "r");
  printf("CLIENT | Opened client fifo: %d\n", fd_cl);
  
  return fd_cl;
}

void receive_msg(FILE* fd_cl){
  Message received_message;
  int n;
  tuples::Message message_serialized;
  char buffer[MSG_SIZE];
  printf("HERE?");
  if (fgets(buffer, sizeof(message_serialized),fd_cl,)==NULL)
  {
      perror("CLIENT | Error reading from main fifo."); 
  }
  
  message_serialized.ParseFromString(buffer);
  received_message.pid = message_serialized.pid();
  received_message.msg = message_serialized.msg();
  printf("CLIENT | Message received: %s and pid received: %d \n", received_message.msg.c_str(), received_message.pid);

}



int main()
{
  FILE* fd_cl, fd_main;
  FILE* fd_main = open_main_fifo();
  send_msg(fd_main);
  fd_cl = open_own_fifo();
  receive_msg(fd_cl);

  close(fd_main);
  printf("CLIENT | Closed main fifo: %d.\n", fd_main);
  
  std::string fifo_path = get_fifo_name(); 
  unlink(fifo_path.c_str());
  printf("CLIENT | Unlinked fifo: %s\n", fifo_path.c_str());
  return 0;
}