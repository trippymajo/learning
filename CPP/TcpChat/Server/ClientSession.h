#pragma once

#include <thread>
#include <string>
#include <memory>
#include <atomic>
#include <mutex>

#include "winsock2.h"

class ChatServer;

class ClientSession
{
public:
  ClientSession(SOCKET socket, ChatServer* server);
  ~ClientSession();

  void Start();
  void Stop();
  void SendMsg(const std::string& msg);

private:
  void ReceiveMsg();
  void GracefulShutdown();

private:
  SOCKET m_socket;
  ChatServer* m_server;

  std::atomic<bool> m_active;
  std::thread m_thread;
  std::mutex m_sendMutex;
};