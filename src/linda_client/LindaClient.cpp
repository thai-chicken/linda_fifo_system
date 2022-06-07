#include "LindaClient.h"

LindaClient::LindaClient()
{
  this->fifo_name = "/tmp/fifo_" + std::to_string(getpid());
}

LindaClient::~LindaClient() {}

void LindaClient::create_fifo()
{
  umask(0);
  if (mknod(this->fifo_name.c_str(), S_IFIFO | 0666, 0) < 0)
  {
    perror("LINDA_CLIENT | Error creating own fifo | ");
  }
  printf("LINDA_CLIENT | Fifo created: %s\n", this->fifo_name.c_str());
}

FILE* LindaClient::open_main_fifo() const
{
  FILE* fd_main;
  if ((fd_main = fopen(FIFO_MAIN_PATH, "w")) == NULL)
  {
    perror("LINDA_CLIENT | Error opening main fifo | ");
    exit(1);
  }
  else
  {
    printf("LINDA_CLIENT | Opened main fifo!\n");
  }
  return fd_main;
}


FILE* LindaClient::open_own_fifo() const
{
  FILE* fd_client;
  if ((fd_client = fopen(this->fifo_name.c_str(), "r")) == NULL)
  {
    perror("LINDA_CLIENT | Error opening own fifo | ");
    exit(1);
  }
  else
  {
    printf("LINDA_CLIENT | Opened own fifo: %s\n", this->fifo_name.c_str());
  }
  return fd_client;
}

void LindaClient::destroy_own_fifo() const
{
  if (unlink(this->fifo_name.c_str()) == -1)
  {
    perror("LINDA_CLIENT | Error unlinking own fifo | ");
    exit(1);
  }
  else
  {
    printf("LINDA_CLIENT | Unlinked fifo: %s\n", this->fifo_name.c_str());
  }
}

void LindaClient::close_main_fifo(FILE* fd_main)
{
  if (fclose(fd_main) != 0)
  {
    perror("LINDA_CLIENT | Error closing main fifo | ");
    exit(1);
  }
  else
  {
    printf("LINDA_CLIENT | Closed main fifo.\n");
  }
  sleep(0.2);
}


void LindaClient::close_own_fifo(FILE* fd_own_fifo)
{
  if (fclose(fd_own_fifo) != 0)
  {
    perror("LINDA_CLIENT | Error closing own fifo | ");
    exit(1);
  }
  else
  {
    printf("LINDA_CLIENT | Closed own fifo.\n");
  }
}

void LindaClient::receive_msg()
{
  Message* msg;
  char buffer_in[MESSAGE_SIZE];
  FILE* fd_client = open_own_fifo();

  if (fgets(buffer_in, sizeof(buffer_in), fd_client) == NULL)
  {
    perror("LINDA_CLIENT | Error reading from own fifo | ");
    exit(1);
  }
  this->close_own_fifo(fd_client);


  MessageType type = static_cast<MessageType>((buffer_in[5]) - 48);

  switch (type)
  {
    case MessageType::TUPLE:
      msg = new TupleMessage;
      break;
    case MessageType::PATTERN:
      msg = new TuplePatternMessage;
      break;

    default:
      std::cout << "type: " << type << std::endl;
      break;
  }

  msg->deserialize(buffer_in);

  if (type == MessageType::TUPLE)
  {
    std::cout << "LINDA_CLIENT | Received: " << dynamic_cast<TupleMessage*>(msg)->getTuple() << ", PID: " << msg->getPid() << std::endl;
  }
  else
  {
    std::cout << "LINDA_CLIENT | Received: " << dynamic_cast<TuplePatternMessage*>(msg)->getTuplePattern() << ", PID: " << msg->getPid()
              << std::endl;
  }
}
void LindaClient::send_msg(Message* msg)
{
  std::string buffer;
  FILE* fd_main;

  msg->setPid(int(getpid()));
  buffer = msg->serialize();

  fd_main = open_main_fifo();
  printf("LINDA_CLIENT | Sending message!\n");
  fputs(buffer.c_str(), fd_main);
  fflush(fd_main);
  printf("LINDA_CLIENT | Sent message!\n");

  this->close_main_fifo(fd_main);
}

void LindaClient::linda_open()
{
  this->create_fifo();
}

void LindaClient::linda_close()
{
  this->destroy_own_fifo();
}

void LindaClient::linda_read(Message* msg)
{
  this->send_msg(msg);
  this->receive_msg();
}

void LindaClient::linda_input(Message* msg)
{
  this->send_msg(msg);
  this->receive_msg();
}

void LindaClient::linda_output(Message* msg)
{
  this->send_msg(msg);
}