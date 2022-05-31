#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "tuples.pb.h"

#define MSG_SIZE 32
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

FILE* open_client_fifo(pid_t pid)
{
  // Open the main fifo.
  std::string client_fifo_path = std::string("/tmp/fifo_") + std::to_string(pid);
  FILE* fd_client = fopen(client_fifo_path.c_str(), "w");
  return fd_client;
}

std::string create_main_fifo()
{
  // Create a fifo.
  std::string fifo_name = FIFO_MAIN_PATH;
  umask(0);
  if (mknod(fifo_name.c_str(), S_IFIFO | 0666, 0) < 0)
  {
    perror("SERVER | Error creating main fifo.");
  }
  printf("SERVER | Fifo created: %s\n", fifo_name.c_str());
  return fifo_name;
}

void handle_client_request(std::string main_fifo)
{
  while (1)
  {
    int n;
    FILE* fd_main;
    Message message_in;
    tuples::Message message_in_serialized;
    char buffer[MSG_SIZE];
    printf("SERVER | Waiting for message...\n");
    
    if ((fd_main = fopen(main_fifo.c_str(), "r"))==NULL){
      perror("SERVER | Error opening main fifo.");
      exit(1);
    }
    else{
      printf("SERVER | Opened main fifo!\n");
    }

    if (fgets(buffer, MSG_SIZE,fd_main ) == NULL)
    {
      perror("SERVER | Error reading from main fifo.");
    }
    printf("SERVER | Read message: %s\n", buffer);

    message_in_serialized.ParseFromString(buffer);
    message_in.pid = message_in_serialized.pid();
    message_in.msg = message_in_serialized.msg();
    printf("SERVER | Message received: %s and pid received: %d \n", message_in.msg.c_str(), message_in.pid);


    FILE* fd_client = open_client_fifo(message_in.pid);
    printf("SERVER | Opened client fifo: %d\n", message_in.pid);
    
    tuples::Message message_out;
    message_out.set_pid(int(get_process_pid()));
    message_out.set_msg("Hello, client!");
    std::string buffer_out;
    message_out.SerializeToString(&buffer_out);

    fputs(buffer_out.c_str(), fd_client);
    fflush(fd_client);
    printf("SERVER | Sent message: %s with size: %lu\n", buffer_out.c_str(), sizeof(buffer_out));
    fclose(fd_client);
    fclose(fd_main);
  }
}


int main()
{
  std::string main_fifo = create_main_fifo();
  printf("SERVER | Created fifo: %s\n", main_fifo.c_str());

  
  handle_client_request(main_fifo);
  
  unlink(FIFO_MAIN_PATH);
  printf("SERVER | Unlinked own descriptor!\n");

  return 0;
}