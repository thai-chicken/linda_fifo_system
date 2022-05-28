#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "tuples.pb.h"

#define MSG_SIZE 80
#define FIFO_MAIN_PATH "/tmp/fifo_main"

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
  std::string fifo_name = FIFO_MAIN_PATH;
  if (mknod(fifo_name.c_str(), 0666, 0) < 0)
  {
    perror("SERVER | Error creating main fifo.");
  }
  printf("SERVER | Fifo created: %s\n", fifo_name.c_str());
  return fifo_name;
}

void handle_client_request(int fd_main)
{
  while (1)
  {
    int n;
    Message message_in;
    tuples::Message message_in_serialized;
    char buffer[MSG_SIZE];
    
    printf("SERVER | Waiting for message...\n");
    while ((n = read(fd_main,buffer, sizeof(message_in_serialized))) <= 0)
    {
      if (n < 0)
      {
        perror("SERVER | Error reading from main fifo.");
      } 
      else{
        // tutaj bedzie trzeba wymyslec cos, zeby nie bylo aktywnego oczekiwania
        continue;
      }
    }
    printf("SERVER | Message received: %s and pid received: %d \n", message_in_serialized.msg().c_str(), message_in_serialized.pid());

    message_in_serialized.ParseFromString(buffer);
    message_in.pid = message_in_serialized.pid();
    message_in.msg = message_in_serialized.msg();

    int fd_client = open_client_fifo(message_in.pid);
    printf("SERVER | Opened client fifo: %d\n", fd_client);
    
    tuples::Message message_out;
    message_out.set_pid(int(get_process_pid()));
    message_out.set_msg("Hello, client!");
    std::string buffer_out;
    message_out.SerializeToString(&buffer_out);

    write(fd_client, buffer_out.c_str(), sizeof(buffer_out));
    printf("SERVER | Sent message: %s with size: %lu\n", buffer_out.c_str(), sizeof(buffer_out));
    close(fd_client);
  }
}


int main()
{
  int fd_main;
  std::string main_fifo = create_main_fifo();
  printf("SERVER | Created fifo: %s\n", main_fifo.c_str());
  fd_main = open(main_fifo.c_str(), O_RDONLY);
  printf("SERVER | Opened fifo: %d\n", fd_main);
  
  handle_client_request(fd_main);
  
  unlink(FIFO_MAIN_PATH);
  return 0;
}