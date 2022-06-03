#include "client/Client.h"

int main(int argc, char const* argv[])
{
  // potem to zmienić żeby dla output nie trzeba było podawać timeoutu
  if (argc != 4)
  {
    printf("CLIENT | Usage: ./client <command> <message>\n");
    exit(1);
  }
  std::string msg = argv[2];
  std::string command = argv[1];
  int timeout = atoi(argv[3]);
  printf("timeout: %d", timeout);
  Client client = Client();
  client.action(msg, command, timeout);
  return 0;
}