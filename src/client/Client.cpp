#include "Client.h"

Client::Client()
{
  this->fifo_name = "/tmp/fifo_" + std::to_string(getpid());
}

Client::~Client() {}

void Client::create_fifo()
{
  umask(0);
  if (mknod(this->fifo_name.c_str(), S_IFIFO | 0666, 0) < 0)
  {
    perror("CLIENT | Error creating own fifo | ");
  }
  printf("SERVER | Fifo created: %s\n", this->fifo_name.c_str());
}

FILE* Client::open_main_fifo() const
{
  FILE* fd_main;
  if ((fd_main = fopen(FIFO_MAIN_PATH, "w")) == NULL)
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


FILE* Client::open_own_fifo() const
{
  FILE* fd_client;
  if ((fd_client = fopen(this->fifo_name.c_str(), "r")) == NULL)
  {
    perror("CLIENT | Error opening own fifo | ");
    exit(1);
  }
  else
  {
    printf("CLIENT | Opened own fifo: %s\n", this->fifo_name.c_str());
  }
  return fd_client;
}

void Client::destroy_own_fifo() const
{
  if (unlink(this->fifo_name.c_str()) == -1)
  {
    perror("CLIENT | Error unlinking own fifo | ");
    exit(1);
  }
  else
  {
    printf("CLIENT | Unlinked fifo: %s\n", this->fifo_name.c_str());
  }
}

void Client::close_main_fifo(FILE* fd_main)
{
  if (fclose(fd_main) != 0)
  {
    perror("CLIENT | Error closing main fifo | ");
    exit(1);
  }
  else
  {
    printf("CLIENT | Closed main fifo.\n");
  }
}


void Client::close_own_fifo(FILE* fd_own_fifo)
{
  if (fclose(fd_own_fifo) != 0)
  {
    perror("CLIENT | Error closing own fifo | ");
    exit(1);
  }
  else
  {
    printf("CLIENT | Closed own fifo.\n");
  }
}

void Client::receive_msg()
{
  tuples::Message msg_serialized;
  Message msg;
  char buffer_in[MSG_SIZE];

  this->create_fifo();
  FILE* fd_client = open_own_fifo();

  if (fgets(buffer_in, sizeof(buffer_in), fd_client) == NULL)
  {
    perror("CLIENT | Error reading from own fifo | ");
    exit(1);
  }

  msg_serialized.ParseFromString(buffer_in);
  msg.pid = msg_serialized.pid();
  msg.msg = msg_serialized.msg();
  msg.command = msg_serialized.command();
  printf("CLIENT | Message received: %s and pid received: %d \n", msg.msg.c_str(), msg.pid);

  this->close_own_fifo(fd_client);
}

void Client::send_msg(std::string msg, std::string cmd)
{
  tuples::Message msg_serialized;
  std::string buffer;
  FILE* fd_main;

  msg_serialized.set_pid(int(getpid()));
  msg_serialized.set_msg(msg);
  msg_serialized.set_command(cmd);
  msg_serialized.SerializeToString(&buffer);

  fd_main = open_main_fifo();
  printf("CLIENT | Sending message: %s with size: %lu\n", buffer.c_str(), sizeof(buffer.c_str()));
  fputs(buffer.c_str(), fd_main);
  fflush(fd_main);
  printf("CLIENT | Sent message: %s with size: %lu\n", buffer.c_str(), sizeof(buffer.c_str()));

  this->close_main_fifo(fd_main);
}

void Client::action(std::string msg, std::string cmd)
{
  this->send_msg(msg, cmd);
  this->receive_msg();
  this->destroy_own_fifo();
}