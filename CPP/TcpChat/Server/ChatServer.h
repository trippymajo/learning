#pragma once
#include <mutex>
#include <atomic>
#include <memory>
#include <unordered_map>

#include "winsock2.h"

class ClientSession;

class ChatServer
{
public:
  ChatServer(const char* port);
  ~ChatServer();

  void Run();

private:
  void Stop();
  void RemoveClient(ClientSession* client);
  void BroadcastMsg(const std::string& msg, ClientSession* sender);
  SOCKET CreateListenSocket();

private:
  SOCKET m_socket;

  std::atomic<bool> m_isRunning;
  std::string m_port;

  std::unordered_map<ClientSession*, std::shared_ptr<ClientSession>> m_clientSockets;
  std::mutex m_clientsMutex;

  friend class ClientSession;
};