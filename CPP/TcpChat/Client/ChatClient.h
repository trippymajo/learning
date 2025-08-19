#pragma once

#include <thread>
#include <atomic>
#include "winsock2.h"

class ChatClient
{
public:
  ChatClient(const char* port);
  ~ChatClient() {};

  void Run();

private:
  void ReceiveMessages();
  void Disconnect();
  void SendMessages();
  void GracefulShutDown();

private:
  std::atomic<bool> m_isActive;
  const char* m_port;
  SOCKET m_socket;
  std::thread m_recv;
};