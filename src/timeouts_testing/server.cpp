#include "src/common/Message.h"
#include "src/common/tuples.pb.h"
#include "src/server/Request.h"
#include "src/server/RequestContainer.h"
#include "src/server/TupleContainer.h"

#include <boost/algorithm/string.hpp>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


#define FIFO_MAIN_PATH "/tmp/fifo_main"


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

void show_elements(TupleContainer* tuples, RequestContainer* requests)
{
  printf("\nTuples:\n");
  tuples->show_elems();
  printf("~~~~~~~\n\n");

  printf("\nRequests:\n");
  requests->show_elems();
  printf("~~~~~~~\n\n");
}

Message handle_message(tuples::Message message_in_serialized, TupleContainer* tuples, RequestContainer* requests, bool* quit)
{
  Message message_in;
  message_in.pid = message_in_serialized.pid();
  message_in.msg = message_in_serialized.msg();
  message_in.command = message_in_serialized.command();
  printf("SERVER | Message received: %s and pid received: %d \n", message_in.msg.c_str(), message_in.pid);

  // lower string
  boost::algorithm::to_lower(message_in.command);

  if (message_in.command == "input")
  {
    Request request(message_in.msg, message_in.pid);
    requests->add(request);
  }
  else if (message_in.command == "output")
  {
    tuples->add(message_in.msg);
  }
  else if (message_in.command == "exit")
  {
    printf("SERVER | Received exit command.\n");
    *quit = true;
  }
  else
  {
    printf("SERVER | Unknown command: %s\n", message_in.command.c_str());
  }
  return message_in;
}

void handle_client(Message message)
{
  FILE* fd_client = open_client_fifo(message.pid);
  printf("SERVER | Opened client fifo: %d\n", message.pid);

  tuples::Message message_out;
  message_out.set_pid(int(get_process_pid()));
  message_out.set_msg("Hello, client!");
  message_out.set_command("reply");

  std::string buffer_out;
  message_out.SerializeToString(&buffer_out);

  fputs(buffer_out.c_str(), fd_client);
  fflush(fd_client);

  printf("SERVER | Sent message: %s with size: %lu\n", buffer_out.c_str(), sizeof(buffer_out));
  fclose(fd_client);
}

void handle_requests(std::string main_fifo, TupleContainer* tuples, RequestContainer* requests)
{
  bool quit = false;
  while (!quit)
  {
    int n;
    FILE* fd_main;
    tuples::Message message_in_serialized;
    Message message_in;
    char buffer[MSG_SIZE];

    printf("SERVER | Waiting for message...\n");
    if ((fd_main = fopen(main_fifo.c_str(), "r")) == NULL)
    {
      perror("SERVER | Error opening main fifo.");
      exit(1);
    }
    else
    {
      printf("SERVER | Opened main fifo!\n");
    }

    if (fgets(buffer, MSG_SIZE, fd_main) == NULL)
    {
      perror("SERVER | Error reading from main fifo.");
    }
    printf("SERVER | Read message: %s\n", buffer);

    message_in_serialized.ParseFromString(buffer);
    message_in = handle_message(message_in_serialized, tuples, requests, &quit);

    show_elements(tuples, requests);

    handle_client(message_in);

    fclose(fd_main);
  }
}


int main()
{
  TupleContainer tuples;
  RequestContainer requests;

  show_elements(&tuples, &requests);

  std::string main_fifo = create_main_fifo();

  handle_requests(main_fifo, &tuples, &requests);

  unlink(FIFO_MAIN_PATH);
  printf("SERVER | Unlinked own descriptor!\n");

  return 0;
}