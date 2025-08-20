#include <iostream>
#include "ChatServer.h"

#pragma comment(lib, "Ws2_32.lib")

int main()
{
  const char* ipadd = "";
  const char* port = "27015";

  try
  {
    ChatServer server(ipadd, port);
    server.Start();
  }
  catch (const std::exception& ex)
  {
    std::cout << "Exception occured with server!\n" << ex.what() << "\n";
    return 1;
  }

  return 0;
}
