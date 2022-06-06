#include "LindaServer.h"

LindaServer::LindaServer()
{
  this->request_container = RequestContainer();
  this->tuple_container = TupleContainer();
  this->quit = false;
  this->fifo_name = FIFO_MAIN_PATH;
  this->request_id = 0;
}

LindaServer::~LindaServer()
{
  this->request_container.clear();
  this->tuple_container.clear();
}

FILE* LindaServer::open_main_fifo() const
{
  FILE* fd_main;
  printf("LINDA_SERVER | Waiting for clients...\n");
  if ((fd_main = fopen(this->fifo_name.c_str(), "r")) == NULL)
  {
    perror("LINDA_SERVER | Error opening main fifo.");
    exit(1);
  }
  else
  {
    printf("LINDA_SERVER | Opened main fifo!\n");
  }
  return fd_main;
}


Message* LindaServer::get_msg_deserialized(FILE* fd_main)
{
  char buffer[MESSAGE_SIZE];

  if (fgets(buffer, MESSAGE_SIZE, fd_main) == NULL)
  {
    std::cout << feof(fd_main) << std::endl;
    if (ferror(fd_main) != 0)
    {
      std::cout << "LINDA_SERVER | FERROR!" << std::endl;
    }
    if (feof(fd_main) != 0)
    {
      std::cout << "LINDA_SERVER | FEOF ERROR!" << std::endl;
    }
    perror("LINDA_SERVER | Error reading from main fifo.");
    exit(1);
  }
  fclose(fd_main);

  MessageType type = static_cast<MessageType>((buffer[5]) - 48);
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
      std::cout << "LINDA_SEVER | Wrong type: " << type << std::endl;
      break;
  }

  msg->deserialize(buffer);
  std::cout << "LINDA_SERVER | SPECIFIED TIMEOUT: " << msg->getTimeout() << std::endl;
  printf("LINDA_SERVER | Message received: %i and pid received: %d \n", msg->getCommand(), msg->getPid());
  return msg;
}

void LindaServer::handle_msg(Message* msg_in)
{
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
  else
  {
    printf("LINDA_SERVER | Unknown command: %i\n", msg_in->getCommand());
  }
}

void LindaServer::handle_requests()
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
  }
}

void LindaServer::show_state()
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

FILE* LindaServer::open_client_fifo(pid_t pid)
{
  FILE* fd_client;
  std::string client_fifo_name = "/tmp/fifo_" + std::to_string(pid);

  if ((fd_client = fopen(client_fifo_name.c_str(), "w")) == NULL)
  {
    perror(("LINDA_SERVER | Error opening client " + std::to_string(pid) + " fifo.").c_str());
    exit(1);
  }
  else
  {
    printf("LINDA_SERVER | Opened client fifo: %s\n", client_fifo_name.c_str());
  }

  return fd_client;
}

void LindaServer::send_to_client(Message* msg)
{
  std::string buffer_out;

  FILE* fd_client = open_client_fifo(msg->getPid());
  msg->setPid(int(getpid()));
  buffer_out = msg->serialize();

  if (fputs(buffer_out.c_str(), fd_client) == EOF)
  {
    perror("LINDA_SERVER | Error writing to client fifo.");
    exit(1);
  }
  else
  {
    printf("LINDA_SERVER | To client sent message: %i with size %lu\n", msg->getCommand(), sizeof(buffer_out));
  }
  fflush(fd_client);

  fclose(fd_client);
}

void LindaServer::destruct_fifo()
{
  unlink(FIFO_MAIN_PATH);
  printf("LINDA_SERVER | Unlinked own descriptor!\n");
}

void LindaServer::create_main_fifo()
{
  umask(0);

  if (mknod(this->fifo_name.c_str(), S_IFIFO | 0666, 0) < 0)
  {
    perror("LINDA_SERVER | Error creating main fifo | ");
  }
  printf("LINDA_SERVER | Fifo created: %s\n", this->fifo_name.c_str());
}

void LindaServer::run()
{
  this->show_state();
  this->create_main_fifo();
  this->handle_requests();
  this->destruct_fifo();
}

void LindaServer::perform_request(TuplePatternMessage* msg)
{
  int idx_in_tuple;
  {
    std::lock_guard<std::mutex> lock(this->mtx_tuple);
    if ((idx_in_tuple = this->tuple_container.find(msg->getTuplePattern())) != -1)
    {
      printf("LINDA_SERVER | PERFORMING REQUEST!\n");
      std::thread send_thread(&LindaServer::send_to_client, this, msg);
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
    std::lock_guard<std::mutex> lock(this->mtx_id);
    request.setId(this->request_id);
    this->increment_id();
  }
  {
    std::lock_guard<std::mutex> lock(this->mtx_request);
    this->request_container.add(request);
  }

  if (msg->getTimeout() > 0)
  {
    std::thread timeout_thread(&LindaServer::perform_timeout, this, request.getId(), msg);
    timeout_thread.detach();
  }
  return;
}

void LindaServer::perform_timeout(int request_id, Message* msg)
{
  int idx_in_req;
  bool is_state_to_show = false;
  sleep(msg->getTimeout());
  {
    std::lock_guard<std::mutex> lock(this->mtx_request);
    if ((idx_in_req = this->request_container.find_id(request_id)) != -1)
    {
      is_state_to_show = true;
      this->request_container.remove(idx_in_req);
      msg->setCommand(Command::TIMEOUT);
      std::cout << "LINDA_SERVER | SPECIFIED TIMEOUT FINISHED: " << msg->getTimeout() << std::endl;
      std::thread send_thread(&LindaServer::send_to_client, this, msg);
      send_thread.detach();
    }
  }
  if (is_state_to_show)
  {
    this->show_state();
  }
}

void LindaServer::perform_tuple(TupleMessage* msg)
{
  int idx_in_req;
  int cmd;
  {
    std::lock_guard<std::mutex> lock(this->mtx_request);

    if ((idx_in_req = this->request_container.find(msg->getTuple())) != -1)
    {
      printf("LINDA_SERVER | PERFORMING TUPLE!\n");
      pid_t client_pid = this->request_container.get(idx_in_req).getMessage()->getPid();
      msg->setPid(client_pid);
      cmd = this->request_container.get(idx_in_req).getMessage()->getCommand();
      this->request_container.remove(idx_in_req);
      std::cout << "LINDA_SERVER | SENDING TO: " << client_pid << std::endl;
      std::thread send_thread(&LindaServer::send_to_client, this, msg);
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