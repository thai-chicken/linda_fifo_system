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

#define MSG_SIZE 80


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
  mkfifo(fifo_name.c_str(), 0666);
  return fifo_name;
}

int open_main_fifo()
{
  // Open the main fifo.
  std::string main_fifo_path = "/tmp/fifo_main";
  int fd_main = open(main_fifo_path.c_str(), O_WRONLY);
  return fd_main;
}

void send_msg(int fd_cl, int fd_main)
{
  std::string msg = "Hello, server!";
  pid_t pid = get_process_pid();
  tuples::Message message;
  message.set_pid(int(pid));
  message.set_msg(msg);

  std::string buffer;
  message.SerializeToString(&buffer);
  
  printf("CLIENT | Sending message: %s with size: %lu\n", buffer.c_str(), sizeof(buffer));
  write(fd_main, buffer.c_str(), sizeof(buffer));
  printf("CLIENT | Sent message: %s with size: %lu\n", buffer.c_str(), sizeof(buffer));
}


int open_own_fifo(){
  std::string fifo_path = create_fifo();
  printf("CLIENT | Created fifo: %s\n", fifo_path.c_str());
  
  int fd_cl = open(fifo_path.c_str(), O_RDONLY | O_NDELAY);
  printf("CLIENT | Opened client fifo: %d\n", fd_cl);
  
  return fd_cl;
}

void receive_msg(int fd_cl){
  Message received_message;
  int n;
  tuples::Message message_serialized;
  char buffer[MSG_SIZE];
  
  while ((n = read(fd_cl,buffer, sizeof(message_serialized))) <= 0)
    {
      if (n < 0)
      {
        perror("CLIENT | Error reading from main fifo.");
      } 
      else{
        // tutaj bedzie trzeba wymyslec cos, zeby nie bylo aktywnego oczekiwania być może w pętli otwierać fifo
        continue;
      }
    }
  
  message_serialized.ParseFromString(buffer);
  received_message.pid = message_serialized.pid();
  received_message.msg = message_serialized.msg();
  printf("CLIENT | Message received: %s and pid received: %d \n", received_message.msg.c_str(), received_message.pid);

}



int main()
{
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  int fd_cl, fd_main;
  fd_cl = open_own_fifo();
  fd_main = open_main_fifo();
  send_msg(fd_cl, fd_main);

  receive_msg(fd_cl);

  close(fd_main);
  std::string fifo_path = get_fifo_name(); 
  unlink(fifo_path.c_str());
  return 0;
}