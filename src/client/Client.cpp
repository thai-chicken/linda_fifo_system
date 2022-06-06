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
  printf("CLIENT | Fifo created: %s\n", this->fifo_name.c_str());
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
  // tuples::Message msg_serialized;
  Message* msg;
  char buffer_in[MESSAGE_SIZE];

  
  FILE* fd_client = open_own_fifo();

  if (fgets(buffer_in, sizeof(buffer_in), fd_client) == NULL)
  {
    perror("CLIENT | Error reading from own fifo | ");
    exit(1);
  }

  // Wlatuje sobie przykładowy string - 1234511
  // Odczytuję wartość na 6tym indeksie i w zależności od wartości deserializuję
  // switch (type)
  // {
  // case MessageType::TUPLE:
  //   msg = TupleMessage();
  //   break;
  
  // case MessageType::PATTERN:
  //   msg = TuplePatternMessage();
  //   break;

  // default:
  //   break;
  // }
  MessageType type = static_cast<MessageType>((buffer_in[5])-48);

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
  std::cout << "CZEGO CHCESZ DZIEWCZYNO JA WIEM"<<std::endl;
  // msg.deserialize(buffer_in);


  // msg_serialized.ParseFromString(buffer_in);
  // msg.pid = msg_serialized.pid();
  // msg.msg = msg_serialized.msg();
  // msg.command = msg_serialized.command();
  printf("CLIENT | Message received: %i and pid received: %d \n", msg->getCommand(), msg->getPid());

  this->close_own_fifo(fd_client);
  
}

void Client::send_msg(Message* msg)
{
  // tuples::Message msg_serialized;
  std::string buffer;
  FILE* fd_main;

  msg->setPid(int(getpid()));
  buffer = msg->serialize();
  // msg_serialized.set_pid(int(getpid()));
  // msg_serialized.set_msg(msg);
  // msg_serialized.set_command(cmd);
  // msg_serialized.SerializeToString(&buffer);

  fd_main = open_main_fifo();
  printf("CLIENT | Sending message: %s with size: %lu\n", buffer.c_str(), sizeof(buffer.c_str()));
  fputs(buffer.c_str(), fd_main);
  fflush(fd_main);
  printf("CLIENT | Sent message: %s with size: %lu\n", buffer.c_str(), sizeof(buffer.c_str()));

  this->close_main_fifo(fd_main);
}

void Client::action(Message* msg)
{
  this->create_fifo();
  this->send_msg(msg);
  if (msg->getCommand() == Command::INPUT || msg->getCommand() == Command::READ)
  {
    this->receive_msg();
  }
  // else if (cmd != "exit" && cmd != "quit" && cmd != "output"){

  // }
  else
  {
    printf("CLIENT | Invalid command.\n");
  }
  this->destroy_own_fifo();
}