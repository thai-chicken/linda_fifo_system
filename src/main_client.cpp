#include "client/Client.h"

int main(int argc, char const* argv[])
{
  // potem to zmienić żeby dla output nie trzeba było podawać timeoutu
  if (argc < 3)
  {
    printf("CLIENT | Usage: ./client <command> <message> <timeout>(default=0)\n");
    exit(1);
  }
  int timeout=0;
  std::string msg = argv[2];
  std::string command = argv[1];

  if (argv[3] != NULL)
    timeout = atoi(argv[3]);

  Client client = Client();
  client.action(msg, command, timeout);
  return 0;
}