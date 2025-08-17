#include <iostream>

#include "ws2tcpip.h"

#include "ChatServer.h"
#include "ClientSession.h"

using std::cin;
using std::cout;
using std::lock_guard;
using std::mutex;
using std::thread;

ChatServer::ChatServer(const char* port)
{
  m_port = port;
  m_socket = INVALID_SOCKET;
}

ChatServer::~ChatServer() 
{
  if (m_socket != INVALID_SOCKET)
    closesocket(m_socket);

  for (const auto& client : m_clientSockets)
    delete client;
  m_clientSockets.clear();

  WSACleanup();
}

void ChatServer::Run()
{
  // Init winsocket
  WSADATA wsaData;
  int startupResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (startupResult != 0)
  {
    cout << "WSAStartup filed:" << startupResult << "\n";
    return;
  }

  struct addrinfo* result = nullptr, *ptr = nullptr, hints;
  memset(&hints, 0, sizeof(hints)); // Make all members of the struct = 0
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = AI_PASSIVE;
  
  // Resolve address
  int operationResult = getaddrinfo(NULL, m_port, &hints, &result);
  if (operationResult != 0)
  {
    cout << "getaddrinfo failed:" << operationResult << "\n";
    WSACleanup();
    return;
  }

  // Creating socket
  m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
  if (m_socket == INVALID_SOCKET)
  {
    cout << "Socket creation filed" << WSAGetLastError() << "\n";
    freeaddrinfo(result);
    WSACleanup();
    return;
  }

  // binding socket
  operationResult = bind(m_socket, result->ai_addr, result->ai_addrlen);
  if (operationResult == -1)
  {
    cout << "Socket binding filed" << WSAGetLastError() << "\n";
    freeaddrinfo(result);
    closesocket(m_socket);
    WSACleanup();
    return;
  }
  freeaddrinfo(result);

  // Listen socket
  if (listen(m_socket, 10) == -1)
  {
    cout << "Failed listening socket" << WSAGetLastError() << "\n";
    freeaddrinfo(result);
    closesocket(m_socket);
    WSACleanup();
    return;
  }

  std::cout << "Server started...\n";

  while (true)
  {
    SOCKET socketClient = accept(m_socket, nullptr, nullptr);
    if (socketClient == INVALID_SOCKET)
    {
      std::cout << "Accepting failed" << WSAGetLastError() << "\n";
      continue;
    }

    // Create Session
    ClientSession* session = new ClientSession(socketClient, this);
    session->Start();

    {
      lock_guard<mutex> lock(m_clientsMutex);
      m_clientSockets.insert(session);
    }
  }
}

void ChatServer::RemoveClient(ClientSession* client)
{
  {
    lock_guard<mutex> lock(m_clientsMutex);
    m_clientSockets.erase(client);
    delete client;
  }
}

void ChatServer::BroadcastMsg(const std::string& msg, ClientSession* sender)
{
  lock_guard<mutex> lock(m_clientsMutex);
  for (const auto& client : m_clientSockets)
  {
    if (client == sender)
      continue;

    client->Send(msg);
  }
}