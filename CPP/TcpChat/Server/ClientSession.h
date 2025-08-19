#pragma once
#include <thread>
#include <string>
#include <mutex>
#include <memory>
#include <atomic>

#include "winsock2.h"

class ChatServer;

class ClientSession : public std::enable_shared_from_this<ClientSession>
{
public:
  ClientSession(SOCKET socket, ChatServer* server);
  ~ClientSession();

  void Start();
  void Send(const std::string& message);
  void Stop();
  
  SOCKET GetSocket() { return m_socket; };

private:
  void Run();
  void GracefulShutDown();

private:
  ChatServer* m_server;
  SOCKET m_socket;

  std::atomic<bool> m_isActive;
  std::thread m_thread;
  std::mutex m_sendMutex;
};