#include <iostream>
#include <vector>
#include <string>
#include "ChatServer.h"

#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char* argv[])
{
  std::vector<std::string> ipadds;
  std::string port = "27015";

  if (argc > 1) 
  {
    port = argv[1];

    for (int i = 2; i < argc; ++i)
      ipadds.emplace_back(argv[i]);
  }

  // If no ip provided, than standard
  if (ipadds.empty()) 
  {
    // Lookback localhost
    ipadds = 
    {
        "127.0.0.1",
        "::1"
    };
  }

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
