#include "Server.h"

Server::Server()
{
  this->request_container = RequestContainer();
  this->tuple_container = TupleContainer();
  this->quit = false;
  this->fifo_name = FIFO_MAIN_PATH;
}

Server::~Server()
{
  this->request_container.clear();
  this->tuple_container.clear();
}

FILE* Server::open_main_fifo() const
{
  FILE* fd_main;
  printf("SERVER | Waiting for clients...\n");
  if ((fd_main = fopen(this->fifo_name.c_str(), "r")) == NULL)
  {
    perror("SERVER | Error opening main fifo.");
    exit(1);
  }
  else
  {
    printf("SERVER | Opened main fifo!\n");
  }
  return fd_main;
}


Message Server::get_msg_deserialized(FILE* fd_main)
{
  Message msg;
  tuples::Message msg_deserialized;
  char buffer[MSG_SIZE];

  if (fgets(buffer, MSG_SIZE, fd_main) == NULL)
  {
    perror("SERVER | Error reading from main fifo.");
    exit(1);
  }

  msg_deserialized.ParseFromString(buffer);
  msg.pid = msg_deserialized.pid();
  msg.command = msg_deserialized.command();
  msg.msg = msg_deserialized.msg();

  printf("SERVER | Message received: %s and pid received: %d \n", msg.msg.c_str(), msg.pid);

  return msg;
}

void Server::handle_msg(Message msg_in)
{
  boost::algorithm::to_lower(msg_in.command);
  if (msg_in.command == "input")
  {
    Request request(msg_in.msg, msg_in.pid);
    this->request_container.add(request);
  }
  else if (msg_in.command == "output")
  {
    this->tuple_container.add(msg_in.msg);
  }
  else if (msg_in.command == "exit")
  {
    printf("SERVER | Received exit command.\n");
    this->quit = true;
  }
  else
  {
    printf("SERVER | Unknown command: %s\n", msg_in.command.c_str());
  }
}

void Server::handle_requests()
{
  int msg_sz;
  FILE* fd_main;
  Message msg_in;
  char buffer[MSG_SIZE];

  while (!this->quit)
  {
    fd_main = this->open_main_fifo();
    msg_in = this->get_msg_deserialized(fd_main);
    this->handle_msg(msg_in);
    this->show_state();
    this->send_to_client(msg_in);
    fclose(fd_main);
  }
}

void Server::show_state() const
{
  printf("\nTuples:\n");
  this->tuple_container.show_elems();
  printf("~~~~~~~\n\n");

  printf("\nRequests:\n");
  this->request_container.show_elems();
  printf("~~~~~~~\n\n");
}

FILE* Server::open_client_fifo(pid_t pid)
{
  FILE* fd_client;
  std::string client_fifo_name = "/tmp/fifo_" + std::to_string(pid);

  if ((fd_client = fopen(client_fifo_name.c_str(), "w")) == NULL)
  {
    perror(("SERVER | Error opening client " + std::to_string(pid) + " fifo.").c_str());
    exit(1);
  }
  else
  {
    printf("SERVER | Opened client fifo: %s\n", client_fifo_name.c_str());
  }

  return fd_client;
}

void Server::send_to_client(Message msg)
{
  tuples::Message msg_serialized;
  std::string buffer_out;

  FILE* fd_client = open_client_fifo(msg.pid);

  msg_serialized.set_pid(int(getpid())); // sends own pid to client
  msg_serialized.set_command("RETURN");
  msg_serialized.set_msg("Hello client " + std::to_string(msg.pid) + "!");
  msg_serialized.SerializeToString(&buffer_out);

  if (fputs(buffer_out.c_str(), fd_client) == EOF)
  {
    perror("SERVER | Error writing to client fifo.");
    exit(1);
  }
  else
  {
    printf("SERVER | To client sent message: %s with size %lu\n", msg_serialized.msg().c_str(), sizeof(buffer_out));
  }
  fflush(fd_client);

  fclose(fd_client);
}

void Server::destruct_fifo()
{
  unlink(FIFO_MAIN_PATH);
  printf("SERVER | Unlinked own descriptor!\n");
}

void Server::create_main_fifo()
{
  umask(0);

  if (mknod(this->fifo_name.c_str(), S_IFIFO | 0666, 0) < 0)
  {
    perror("SERVER | Error creating main fifo | ");
  }
  printf("SERVER | Fifo created: %s\n", this->fifo_name.c_str());
}

void Server::run()
{
  this->show_state();
  this->create_main_fifo();
  this->handle_requests();
  this->destruct_fifo();
}