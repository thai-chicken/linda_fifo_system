#include <fcntl.h>
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

int open_client_fifo(pid_t pid)
{
  // Open the main fifo.
  std::string client_fifo_path = std::string("/tmp/fifo_") + std::to_string(pid);
  int fd_client = open(client_fifo_path.c_str(), O_WRONLY);
  return fd_client;
}

std::string create_main_fifo()
{
  // Create a fifo.
  std::string fifo_name = "/tmp/fifo_main";
  if (mknod(fifo_name.c_str(), 0666, 0) < 0)
  {
    perror("SERVER | Error creating main fifo.");
  }
  printf("SERVER | Fifo created: %s\n", fifo_name.c_str());
  return fifo_name;
}

void handle_client_request(int fd_main)
{
  // while (1)
  // {
  int n;
  Message message_in;
  char str1[MSG_SIZE];
  if ((n = read(fd_main,str1, MSG_SIZE)) <= 0)
  {
    perror("SERVER | read error!");
  }
  else
  {
    printf("SERVER | Received message: %s of size %d\n", str1, n);
  }
  // int fd_client = open_client_fifo(message_in.pid);
  // printf("Opened client fifo: %d\n", fd_client);
  // Message message_out = {get_process_pid(), "Hello, client!"};
  // write(fd_client, &message_out, sizeof(message_out));
  // printf("Sent message: %s with size: %lu\n", message_out.msg.c_str(), sizeof(message_out));
  // close(fd_client);
  // }
}


int main()
{
  int fd_main;
  std::string main_fifo = create_main_fifo();
  printf("SERVER | Created fifo: %s\n", main_fifo.c_str());
  fd_main = open(main_fifo.c_str(), O_RDONLY);
  printf("SERVER | Opened fifo: %d\n", fd_main);
  handle_client_request(fd_main);
  close(fd_main);
  return 0;
}