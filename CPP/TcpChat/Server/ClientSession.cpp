#include <iostream>

#include "ClientSession.h"
#include "ChatServer.h"

using std::cout;
using std::string;
using std::mutex;
using std::lock_guard;

constexpr unsigned int RECV_BUF = 4096;

ClientSession::ClientSession(SOCKET socket, ChatServer* server)
{
  m_socket = socket;
  m_server = server;
  m_isActive = false;
}

ClientSession::~ClientSession() 
{
  Stop();
}

void ClientSession::Start()
{
  m_isActive.store(true);
  m_thread = std::thread(&ClientSession::Run, this);
}

void ClientSession::Stop()
{
  bool expected = true;
  if (m_isActive.compare_exchange_strong(expected, false))
  {
    if (m_socket != INVALID_SOCKET)
    {
      int operationResult = shutdown(m_socket, SD_BOTH);
      if (operationResult == -1)
        cout << "Failed to shutdown client connection properly" << WSAGetLastError() << "\n";
    }
  }

  if (m_thread.joinable())
    m_thread.join();

  if (m_socket != INVALID_SOCKET)
  {
    closesocket(m_socket);
    m_socket = INVALID_SOCKET;
  }
}

void ClientSession::Run()
{
  std::shared_ptr<ClientSession> self = shared_from_this();
  char buf[RECV_BUF];

  while (m_isActive.load(std::memory_order_acquire))
  {
    int bytes = recv(m_socket, buf, RECV_BUF, 0);

    if (bytes <= 0)
      break;

    string msg(buf, bytes);

    // Send to everyone
    m_server->BroadcastMsg(msg, this);
  }

  m_server->RemoveClient(this);
  m_isActive.store(false, std::memory_order_release);
}

void ClientSession::Send(const string& msg)
{ // TODO!!! Send all stream!
  lock_guard<mutex> lock(m_sendMutex);
  send(m_socket, msg.c_str(), msg.size(), 0);
}