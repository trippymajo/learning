#include <iostream>
#include "ChatClient.h"

#pragma comment(lib, "Ws2_32.lib")

int main()
{
  try
  {
    const char* ipadd = "127.0.0.1";
    const char* port = "27015";
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