#pragma once
#include <thread>
#include <mutex>
#include <unordered_set>

#include "winsock2.h"

class ClientSession;

class ChatServer
{
public:
  ChatServer(const char* port);
  ~ChatServer();

  void Run();

private:
  void RemoveClient(ClientSession* client);
  void BroadcastMsg(const std::string& msg, ClientSession* sender);

private:
  const char* m_port;
  SOCKET m_socket;
  std::unordered_set<ClientSession*> m_clientSockets;
  std::mutex m_clientsMutex;

  friend class ClientSession;
};