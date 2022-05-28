#include <fcntl.h>
#include <iostream>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

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

std::string create_fifo()
{
  // Create a fifo.
  pid_t pid = get_process_pid();
  std::string fifo_path = std::string("/tmp/fifo_") + std::to_string(pid);
  mkfifo(fifo_path.c_str(), 0666);
  return fifo_path;
}

int open_main_fifo()
{
  // Open the main fifo.
  std::string main_fifo_path = "/tmp/fifo_main";
  int fd_main = open(main_fifo_path.c_str(), O_WRONLY);
  return fd_main;
}


int main()
{
  int fd_cl, fd_main;
  printf("CLIENT | Opened fifo: %d\n", fd_cl);
  fd_main = open_main_fifo();
  std::string fifo_path = create_fifo();
  printf("CLIENT | Created fifo: %s\n", fifo_path.c_str());
  // fd_cl = open(fifo_path.c_str(), O_RDONLY);
  // printf("CLIENT | Opened main fifo: %d\n", fd_main);
  std::string msg = "Hello, server!";
  // pid_t pid = get_process_pid();
  // Message message = {pid, msg};
  // printf("CLIENT | Sending message: %s with size: %lu\n", message.msg.c_str(), sizeof(message));
  write(fd_main, msg.c_str(), MSG_SIZE);
  printf("CLIENT | Sent message: %s with size: %lu\n", msg.c_str(), sizeof(msg));
  // Message received_message;
  // read(fd_cl, &received_message, sizeof(received_message));
  // printf("CLIENT | Received message: %s with size: %lu\n", received_message.msg.c_str(), sizeof(received_message));
  close(fd_main);
  // close(fd_cl);
  return 0;
}