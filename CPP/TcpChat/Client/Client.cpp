#include <iostream>
#include "ChatClient.h"

int main()
{
  try
  {
    const char* port = "12345";
    ChatClient cli(port);
    cli.Run();
  }
  catch (const std::exception& ex)
  {
    std::cout << "Exception occured:" << ex.what() << "\n";
    return 1;
  }

  return 0;
}