#pragma once

#include <string>
#include <thread>
#include <vector>
#include <memory>
#include <atomic>
#include <mutex>

#include "winsock2.h"

class ClientSession;

class ChatServer 
{
public:
  ChatServer(const std::vector<std::string>& ips, const std::string& port);
  ~ChatServer();

  void Start();
  void Stop();
  void BroadcastMsg(const std::string& msg, ClientSession* pSender);

private:
  void AcceptClients(SOCKET socket);
  void PrintSockaddr(const sockaddr* addr);

private:
  SOCKET CreateListeningSocket(const std::string& ip);

  std::string m_port;
  std::vector<std::string> m_ips;
  std::vector<SOCKET> m_listenSockets;

  std::atomic<bool> m_running;
  std::vector<std::thread> m_acceptThreads;

  std::vector<std::shared_ptr<ClientSession>> m_clients;
  std::mutex m_clientsMutex;
};