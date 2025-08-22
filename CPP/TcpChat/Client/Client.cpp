#include <iostream>
#include "ChatClient.h"

#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char* argv[])
{
  // Lookback localhost
  const char* ipadd = "127.0.0.1";
  const char* port = "27015";

  if (argc > 1)
    port = argv[1];

  if (argc > 2)
    ipadd = argv[2];

  try
  {
    ChatClient cli(ipadd, port);
    cli.Run();
  }
  catch (const std::exception& ex)
  {
    std::cout << "Exception occured:" << ex.what() << "\n";
    return 1;
  }

  return 0;
}