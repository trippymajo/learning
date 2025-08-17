#pragma once
#include <thread>
#include <string>
#include <mutex>

#include "winsock2.h"

class ChatServer;

class ClientSession
{
public:
  ClientSession(SOCKET socket, ChatServer* server);
  ~ClientSession();

  void Start();
  void Send(const std::string& message);
  void Disconnect();
  
  SOCKET GetSocket() { return m_socket; };

private:
  void Run();

private:
  bool m_isActive;
  SOCKET m_socket;
  std::thread m_thread;
  std::mutex m_sendMutex;
  ChatServer* m_server;
};