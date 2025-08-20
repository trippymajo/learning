#pragma once

#include <thread>
#include <atomic>
#include <string>

#include "winsock2.h"

class ChatClient
{
public:
  ChatClient(const char* ipadd, const char* port);
  ~ChatClient() {};

  void Run();

private:
  void ReceiveMessages();
  void Disconnect();
  void SendMessages();
  void GracefulShutDown();

private:
  std::atomic<bool> m_isActive;
  std::string m_port;
  std::string m_ipadd;
  SOCKET m_socket;
  std::thread m_recv;
};