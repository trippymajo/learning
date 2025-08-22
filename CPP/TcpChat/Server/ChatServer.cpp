#include "ChatServer.h"
#include "ClientSession.h"

#include <iostream>
//#include <format>

#include "WS2tcpip.h"

using std::cout;
using std::cerr;
//using std::format;

constexpr int MAX_LISTEN = 10;

ChatServer::ChatServer(const std::vector<std::string>& ips, const std::string& port)
{
  m_ips = ips;
  m_port = port;
  m_running.store(false, std::memory_order_release);
}

ChatServer::~ChatServer()
{
  Stop();
}

void ChatServer::Start()
{
  cout << "Strarting server...\n";
  {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
      cerr << "WSAStartup failed\n";
      return;
    }
  }

  for (const auto& ip : m_ips)
    m_listenSockets.push_back(CreateListeningSocket(ip));

  if (m_listenSockets.size() == 0)
  {
    cerr << "Failed to create listening sockets\n";
    return;
  }

  m_running.store(true, std::memory_order_release);

  for (const auto& socket : m_listenSockets)
  {
    m_acceptThreads.emplace_back(&ChatServer::AcceptClients, this, socket);
  }

  while (m_running.load(std::memory_order_acquire))
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void ChatServer::Stop()
{
  m_running.store(false, std::memory_order_release);

  for (auto& s : m_listenSockets)
  {
    if (s != INVALID_SOCKET)
      closesocket(s);
  }
  m_listenSockets.clear();

  for (auto& t : m_acceptThreads)
  {
    if (t.joinable())
      t.join();
  }
  m_acceptThreads.clear();

  std::lock_guard<std::mutex> lock(m_clientsMutex);
  for (auto& c : m_clients)
    c->Stop();
  m_clients.clear();

  WSACleanup();
}

SOCKET ChatServer::CreateListeningSocket(const std::string& ip)
{
  SOCKET retSocket = INVALID_SOCKET;

  struct addrinfo hints, *result = nullptr;
  memset(&hints, 0, sizeof(hints)); // make all fields 0
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;

  if (getaddrinfo(ip.c_str(), m_port.c_str(), &hints, &result) != 0)
    return retSocket;

  for (addrinfo* ptr = result; ptr != nullptr; ptr = ptr->ai_next)
  {
    SOCKET s = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    int opt = 1;
    // SO_REUSEADDR SO_EXCLUSIVEADDRUSE
    setsockopt(s, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, reinterpret_cast<const char*>(&opt), sizeof(opt));

    if (s == INVALID_SOCKET)
      continue;

    if (bind(s, ptr->ai_addr, (int)ptr->ai_addrlen) == -1)
    {
      closesocket(s);
      continue;
    }

    if (listen(s, MAX_LISTEN) == -1)
    {
      closesocket(s);
      continue;
    }

    cout << "Server listening on: ";
    PrintSockaddr(ptr->ai_addr);
    retSocket = s;
    break;
  }

  freeaddrinfo(result);
  return retSocket;
}

void ChatServer::PrintSockaddr(const sockaddr* addr)
{
  char ipStr[INET6_ADDRSTRLEN] = {};
  int port = 0;

  if (addr->sa_family == AF_INET) // IPv4
  {
    sockaddr_in* ipv4 = (sockaddr_in*)addr;
    if (inet_ntop(AF_INET, &(ipv4->sin_addr), ipStr, sizeof(ipStr)))
      port = ntohs(ipv4->sin_port);
  }
  else if (addr->sa_family == AF_INET6) // IPv6
  {
    sockaddr_in6* ipv6 = (sockaddr_in6*)addr;
    if (inet_ntop(AF_INET6, &(ipv6->sin6_addr), ipStr, sizeof(ipStr)))
      port = ntohs(ipv6->sin6_port);
  }
  else
  {
    std::cerr << "Unknown address family\n";
  }

  cout << ipStr << ":" << port << "\n";
}

void ChatServer::AcceptClients(SOCKET socket)
{
  while (m_running.load(std::memory_order_acquire))
  {
    SOCKET clientSocket = accept(socket, nullptr, nullptr);
    if (clientSocket == INVALID_SOCKET)
      continue;

    cout << "Client connected: ";
    sockaddr addr;
    int addrsize = sizeof(addr);
    if (getpeername(clientSocket, &addr, &addrsize) == 0)
      PrintSockaddr(&addr);

    auto session = std::make_shared<ClientSession>(clientSocket, this);
    session->Start();
    std::lock_guard<std::mutex> lock(m_clientsMutex);
    m_clients.push_back(session);
  }
}

void ChatServer::BroadcastMsg(const std::string& msg, ClientSession* pSender)
{
  std::lock_guard<std::mutex> lock(m_clientsMutex);

  for (const auto& pClient : m_clients)
  {
    if (pClient && pClient.get() != pSender)
      pClient->SendMsg(msg);
  }

  cout << "Message broadcated:" << msg << "\n";
}