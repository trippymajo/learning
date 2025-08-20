#include <iostream>

#include "ChatClient.h"
#include "ws2tcpip.h"

using std::cin;
using std::cout;
using std::thread;
using std::string;

constexpr unsigned int RECV_BUF = 4096;
constexpr int RECV_TIMEOUT = 3000; // ms

ChatClient::ChatClient(const char* ipadd, const char* port)
{
  m_isActive.store(false, std::memory_order_release);
  m_port = port;
  m_ipadd = ipadd;
  m_socket = INVALID_SOCKET;
}

void ChatClient::ReceiveMessages()
{
  char buf[1024];

  while (m_isActive.load(std::memory_order_acquire))
  {
    int bytes = recv(m_socket, buf, sizeof(buf) - 1, 0);

    if (bytes <= 0)
    {
      cout << "Server disconnected. Press Enter to exit.\n";
      m_isActive = false;
      break;
    }

    buf[bytes] = '\0';
    string msg(buf, bytes);

    cout << msg << "\n";
  }
}

void ChatClient::SendMessages()
{
  string msg;
  while (std::getline(cin, msg))
  {
    if (!m_isActive)
      break;

    // read num of bytes sent, and do send all untill the full message sent

    const char* ptr = msg.data();
    size_t size = msg.size();
    while (size > 0)
    {
      int sent = send(m_socket, ptr, static_cast<int>(size), 0);

      if (sent <= 0)
        break;

      ptr += sent;
      size -= static_cast<size_t>(sent);
    }
  }
}

void ChatClient::Run()
{
  WSADATA wsaData;
  int operationResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (operationResult != 0)
  {
    cout << "WSAStartup filed:" << operationResult << "\n";
    return;
  }

  struct addrinfo* result = nullptr, *ptr = nullptr, hints;
  memset(&hints, 0, sizeof(hints)); // Make all members of the struct = 0
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_flags = 0;

  operationResult = getaddrinfo(m_ipadd.c_str(), m_port.c_str(), &hints, &result);
  if (operationResult != 0)
  {
    cout << "getaddrinfo failed:" << operationResult << "\n";
    WSACleanup();
    return;
  }

  for (ptr = result; ptr != nullptr; ptr = ptr->ai_next)
  {
    // Try create socket
    m_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (m_socket == INVALID_SOCKET)
      continue;

    // Try connect socket
    if (connect(m_socket, ptr->ai_addr, (int)ptr->ai_addrlen) == 0)
        break;

    closesocket(m_socket);
  }

  freeaddrinfo(result);

  if (m_socket == INVALID_SOCKET)
  {
    cout << "Could not connect server" << WSAGetLastError() << "\n";
    WSACleanup();
    return;
  }

  m_isActive = true;

  m_recv = thread(&ChatClient::ReceiveMessages, this);

  SendMessages();

  Disconnect();
  m_recv.join();

  WSACleanup();
}

void ChatClient::GracefulShutDown()
{
  shutdown(m_socket, SD_SEND);

  int recvTimeOut = RECV_TIMEOUT;
  setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, 
    reinterpret_cast<const char*>(&recvTimeOut), sizeof(recvTimeOut));

  char buf[RECV_BUF];

  while (true)
  {
    int bytes = recv(m_socket, buf, sizeof(buf), 0);

    if (bytes <= 0)
      break;
  }
}

void ChatClient::Disconnect()
{
  m_isActive = false;

  if (m_socket != INVALID_SOCKET)
  {
    GracefulShutDown();
    closesocket(m_socket);
    m_socket = INVALID_SOCKET;
  }
}