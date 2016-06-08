#include "TCPConnection.hpp"

TCPConnection::TCPConnection()
  : m_opened(false)
{}

TCPConnection::TCPConnection(string host, string port)
  : m_host(host), m_port(port), m_opened(false)
{
  connect();
}

void TCPConnection::connect()
{
  if (m_opened) {
	WARN("connection is already opened");
    return;
  }
  if (m_host.size() == 0) {
	WARN("host field is empty");
	return;
  }
  if (m_port.size() == 0) {
	WARN("port field is empty");
	return;
  }
	  
  struct addrinfo hints, *res, *rp;
  int r;
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;
  r = getaddrinfo(m_host.c_str(), m_port.c_str(), &hints, &res);
  if (r != 0) {
	ERROR("getaddrinfo: %s", gai_strerror(r));
  }
  for (rp = res; rp != NULL; rp = rp->ai_next) {
	m_socket = socket(rp->ai_family, rp->ai_socktype,
					  rp->ai_protocol);
	if (m_socket == -1)
	  continue;
	if (::connect(m_socket, rp->ai_addr, rp->ai_addrlen) != -1)
	  break;
	::close(m_socket);
  }
  if (rp == NULL) {
	ERROR("could not connect to host");
  }
  INFO("connected to host %s on port %s", m_host.c_str(),
	   m_port.c_str());
  freeaddrinfo(res);
  m_opened = true;
}

void TCPConnection::connect(string host, string port)
{
  m_host = host;
  m_port = port;
  connect();
}

void TCPConnection::disconnect()
{
  ::close(m_socket);
  m_opened = false;
}

bool TCPConnection::send(string msg)
{
  int s = ::send(m_socket, msg.c_str(), msg.size(), 0);
  if (s == -1) {
	WARN("could not send message");
	return false;
  } else {
	return true;
  }
}

bool TCPConnection::recv(string &buff) {
  memset(m_buff, 0, BUFFSIZE + 1);
  int s = ::recv(m_socket, m_buff, BUFFSIZE, 0);
  if (s == -1) {
	WARN("error while receiving data");
	return false;
  } else {
	buff = m_buff;
	return true;
  }
}

TCPConnection::~TCPConnection() {
  ::close(m_socket);
  INFO("socket closed");
}
