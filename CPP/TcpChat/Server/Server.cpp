#include <iostream>
#include <vector>
#include <string>
#include "ChatServer.h"

#pragma comment(lib, "Ws2_32.lib")

int main()
{
  const std::vector<std::string> ipadds
  {
    "127.0.0.1",
    "::1"
  };

  const char* port = "27015";

  try
  {
    ChatServer server(ipadds, port);
    server.Start();
  }
  catch (const std::exception& ex)
  {
    std::cout << "Exception occured with server!\n" << ex.what() << "\n";
    return 1;
  }

  return 0;
}
