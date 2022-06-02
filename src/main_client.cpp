#include "client/Client.h"

int main(int argc, char const* argv[])
{
  if (argc != 3)
  {
    printf("CLIENT | Usage: ./client <command> <message>\n");
    exit(1);
  }
  std::string msg = argv[2];
  std::string command = argv[1];
  Client client = Client();
  client.action(msg, command);
  return 0;
}