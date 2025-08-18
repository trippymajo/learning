#include <iostream>
#include "ChatServer.h"

int main()
{
  const char* port = "12345";

  try
  {
    ChatServer server(port);
    server.Run();
  }
  catch (const std::exception& ex)
  {
    std::cout << "Exception occured with server!\n" << ex.what() << "\n";
    return 1;
  }

  return 0;
}
