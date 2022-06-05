#include "Server.h"

Server::Server()
{
  this->request_container = RequestContainer();
  this->tuple_container = TupleContainer();
  this->quit = false;
  this->fifo_name = FIFO_MAIN_PATH;
  this->request_id = 0;
}

Server::~Server()
{
  this->request_container.clear();
  this->tuple_container.clear();
}

// SIE ROBI ES
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


Message* Server::get_msg_deserialized(FILE* fd_main)
{
  // tuples::Message msg_deserialized;
  char buffer[MESSAGE_SIZE];

  // TODO: TUTAJ JEST GIGA BUG --> wchodzi do pepoglad
  if (fgets(buffer, MESSAGE_SIZE, fd_main) == NULL)
  {
    std::cout << feof(fd_main) << std::endl;
    if (ferror(fd_main) != 0) {
        std::cout << "cos" << std::endl;
    }
    // osiagnieto koniec pliku??
    if (feof(fd_main) != 0) {
      std::cout << "pepoglad" << std::endl;
    }
    perror("SERVER | Error reading from main fifo.");
    
    exit(1);
  }

  MessageType type = static_cast<MessageType>((buffer[5])-48);
  Message* msg;
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
  

  msg->deserialize(buffer);
  // msg_deserialized.ParseFromString(buffer);
  // msg.pid = msg_deserialized.pid();
  // msg.command = msg_deserialized.command();
  // msg.msg = msg_deserialized.msg();

  printf("SERVER | Message received: %i and pid received: %d \n", msg->getCommand(), msg->getPid());
  
  return msg;
}

//TODO: Trzeba zrobić, da się radę ;)
void Server::handle_msg(Message* msg_in)
{
  // boost::algorithm::to_lower(msg_in.command);
  if ((msg_in->getCommand() == Command::INPUT) || (msg_in->getCommand() == Command::READ))
  {
    TuplePatternMessage* msg_req = dynamic_cast<TuplePatternMessage*>(msg_in);
    this->perform_request(msg_req);
  }
  else if (msg_in->getCommand() == Command::OUTPUT)
  {
    TupleMessage* msg_tup = dynamic_cast<TupleMessage*>(msg_in);
    this->perform_tuple(msg_tup);
  }
  // else if (msg_in.command == "exit" || msg_in.command == "quit")
  // {
  //   printf("SERVER | Received exit command.\n");
  //   this->quit = true;
  // }
  else
  {
    printf("SERVER | Unknown command: %i\n", msg_in->getCommand());
  }
}

void Server::handle_requests()
{
  int msg_sz;
  FILE* fd_main;
  Message* msg_in;
  char buffer[MESSAGE_SIZE];

  while (!this->quit)
  {
    fd_main = this->open_main_fifo();
    msg_in = this->get_msg_deserialized(fd_main);
    this->handle_msg(msg_in);
    this->show_state();
    // this->send_to_client(msg_in);
    fclose(fd_main);
  }
}

void Server::show_state()
{
  {
    std::lock_guard<std::mutex> lock(this->mtx_tuple);
    printf("\nTuples:\n");
    this->tuple_container.show_elems();
    printf("~~~~~~~\n\n");
  }
  {
    std::lock_guard<std::mutex> lock(this->mtx_request);
    printf("\nRequests:\n");
    this->request_container.show_elems();
    printf("~~~~~~~\n\n");
  }
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

void Server::send_to_client(Message* msg)
{
  // tuples::Message msg_serialized;
  std::string buffer_out;

  FILE* fd_client = open_client_fifo(msg->getPid());
  msg->setPid(int(getpid()));
  buffer_out = msg->serialize();
  // msg_serialized.set_pid(int(getpid())); // sends own pid to client
  // msg_serialized.set_command(command);
  // msg_serialized.set_msg(msg);
  // msg_serialized.SerializeToString(&buffer_out);

  if (fputs(buffer_out.c_str(), fd_client) == EOF)
  {
    perror("SERVER | Error writing to client fifo.");
    exit(1);
  }
  else
  {
    printf("SERVER | To client sent message: %i with size %lu\n", msg->getCommand(), sizeof(buffer_out));
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

//TODO: Ło kurka co to??????
//EDIT: CO TU SIE KURWA DZIEJE?!?!
void Server::perform_request(TuplePatternMessage* msg)
{
  int idx_in_tuple;
  // Message message_to_send;
  {
    std::lock_guard<std::mutex> lock(this->mtx_tuple);
    if ((idx_in_tuple = this->tuple_container.find(msg->getTuplePattern())) != -1)
    {
      // message_to_send.msg = this->tuple_container.get(idx_in_tuple);
      // message_to_send.pid = msg.pid;
      // message_to_send.command = "return";
      printf("PERFORM REQUEST | CREATING NEW THREAD!\n");
      std::thread send_thread(&Server::send_to_client, this, msg);
      send_thread.detach();

      if (msg->getCommand() == Command::INPUT)
      {
        this->tuple_container.remove(idx_in_tuple);
      }
      return;
    }
  }
  Request request(msg);
  {
    std::lock_guard<std::mutex> lock(this->mtx_request);
    this->request_container.add(request);
  }
  return;
}

void Server::perform_tuple(TupleMessage* msg)
{
  int idx_in_req;
  int cmd;
  {
    std::lock_guard<std::mutex> lock(this->mtx_request);
    
    if ((idx_in_req = this->request_container.find(msg->getTuple())) != -1)
    {
      // message_to_send.msg = msg.msg;
      // message_to_send.pid = this->request_container.get(idx_in_req).get_request_pid();
      // message_to_send.command = "return";
      printf("PERFORM TUPLE | CREATING NEW THREAD!\n");

      pid_t client_pid = this->request_container.get(idx_in_req).getMessage()->getPid();
      msg->setPid(client_pid);
      cmd = this->request_container.get(idx_in_req).getMessage()->getCommand(); 
      this->request_container.remove(idx_in_req);
      std::cout << "WYSYLAMY DO: " << client_pid << std::endl;

      std::thread send_thread(&Server::send_to_client, this, msg);
      send_thread.detach();
    }
  }
  {
    std::lock_guard<std::mutex> lock(this->mtx_tuple);
    if ((idx_in_req == -1) || (idx_in_req != -1 && cmd == Command::READ))
    {
      this->tuple_container.add(msg->getTuple());
    }
  }
}