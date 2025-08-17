#include <iostream>

#include "ClientSession.h"
#include "ChatServer.h"

using std::cout;
using std::thread;
using std::string;
using std::mutex;
using std::lock_guard;

ClientSession::ClientSession(SOCKET socket, ChatServer* server)
{
  m_isActive = true;
  m_socket = socket;
  m_server = server;
}

ClientSession::~ClientSession() 
{
  Disconnect();
}

void ClientSession::Start()
{
  m_thread = thread(&ClientSession::Run, this);
  m_thread.detach();
}

void ClientSession::Run()
{
  char buf[1024];

  while (m_isActive)
  {
    int bytes = recv(m_socket, buf, sizeof(buf) - 1, 0);

    if (bytes <= 0)
      break;

    buf[bytes] = '\0';
    string msg(buf);

    // Send to everyone
    m_server->BroadcastMsg(msg, this);
  }

  Disconnect();

  m_server->RemoveClient(this);
}

void ClientSession::Send(const string& msg)
{
  lock_guard<mutex> lock(m_sendMutex);
  send(m_socket, msg.c_str(), msg.size(), 0);
}

void ClientSession::Disconnect()
{
  if (!m_isActive)
    return;

  m_isActive = false;

  int operationResult = shutdown(m_socket, SD_BOTH);
  if (operationResult == -1)
    cout << "Failed to shutdown client connection properly" << WSAGetLastError() << "\n";

  closesocket(m_socket);
}