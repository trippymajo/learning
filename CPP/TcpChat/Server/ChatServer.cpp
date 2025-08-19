#include <iostream>
#include <vector>

#include "ws2tcpip.h"

#include "ChatServer.h"
#include "ClientSession.h"

using std::cout;
using std::mutex;
using std::lock_guard;

constexpr int MAX_CONNECTIONS = 10;

ChatServer::ChatServer(const char* port)
{
  m_socket = INVALID_SOCKET;
  m_isRunning.store(false);
  m_port = port;
}

ChatServer::~ChatServer() 
{
  Stop();
}

void ChatServer::Stop()
{
  m_isRunning.store(false, std::memory_order_release);

  if (m_socket != INVALID_SOCKET)
  {
    closesocket(m_socket);
    m_socket = INVALID_SOCKET;
  }

  std::vector<std::shared_ptr<ClientSession>> snap;
  {
    lock_guard<mutex> lock(m_clientsMutex);
    snap.reserve(m_clientSockets.size());
    for (const auto& kval : m_clientSockets)
      snap.push_back(kval.second);

    m_clientSockets.clear();
  }

  for (const auto& pClient : snap)
  {
    if (pClient)
      pClient->Stop();
  }

  WSACleanup();
}

SOCKET ChatServer::CreateListenSocket()
{
  // Init winsocket
  WSADATA wsaData;
  int operationResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (operationResult != 0)
  {
    cout << "WSAStartup filed:" << operationResult << "\n";
    return INVALID_SOCKET;
  }

  struct addrinfo* result = nullptr, * ptr = nullptr, hints;
  memset(&hints, 0, sizeof(hints)); // Make all members of the struct = 0
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  // Resolve address
  operationResult = getaddrinfo(NULL, m_port.c_str(), &hints, &result);
  if (operationResult != 0)
  {
    cout << "getaddrinfo failed:" << operationResult << "\n";
    return INVALID_SOCKET;
  }

  SOCKET retVal = INVALID_SOCKET;

  // Creating socket
  for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next)
  {
    SOCKET s = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    if (s == INVALID_SOCKET)
      continue;

    if (bind(s, ptr->ai_addr, (int)ptr->ai_addrlen) == -1) 
    {
      closesocket(s);
      continue;
    }

    if (listen(s, MAX_CONNECTIONS) == -1)
    {
      closesocket(s);
      continue;
    }

    retVal = s;
    break;
  }

  freeaddrinfo(result);

  if (retVal == INVALID_SOCKET)
    cout << "Failed to create/bind/listen on any address.\n";

  return retVal;
}

void ChatServer::Run()
{
  m_socket = CreateListenSocket();
  if (m_socket == INVALID_SOCKET)
    return;

  cout << "Server listening on port " << m_port << "\n";
  m_isRunning.store(true, std::memory_order_release); // memory order release?

  while (m_isRunning.load(std::memory_order_acquire))
  {
    SOCKET socketClient = accept(m_socket, nullptr, nullptr);
    if (socketClient == INVALID_SOCKET)
    {
      std::cout << "Accepting failed" << WSAGetLastError() << "\n";
      continue;
    }

    // Create Session
    auto session = std::make_shared<ClientSession>(socketClient, this);
    {
      lock_guard<mutex> lock(m_clientsMutex);
      m_clientSockets[session.get()] = session;
    }
    session->Start();
  }
}

void ChatServer::BroadcastMsg(const std::string& msg, ClientSession* sender)
{
  std::vector<std::shared_ptr<ClientSession>> snap;
  {
    lock_guard<mutex> lock(m_clientsMutex);
    snap.reserve(m_clientSockets.size());
    for (const auto& kval : m_clientSockets)
      snap.push_back(kval.second);
  }

  for (const auto& pClient : snap)
  {
    if (!pClient || pClient.get() == sender)
      continue;

    pClient->Send(msg);
  }
}

void ChatServer::RemoveClient(ClientSession* client)
{
  lock_guard<mutex> lock(m_clientsMutex);
  m_clientSockets.erase(client);
}