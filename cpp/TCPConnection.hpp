#ifndef HAVE_TCPCONNECTION
#define HAVE_TCPCONNECTION

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <iostream>

#include "debug.h"
#include "Connection.hpp"

#define BUFFSIZE 512

using namespace std;

class TCPConnection : public Connection
{
public:
  TCPConnection();
  TCPConnection(string host, string port);
  virtual void connect();
  virtual void connect(string host, string port);
  virtual void disconnect();
  virtual bool send(string msg);
  virtual bool recv(string &buff);
  ~TCPConnection();
private:
  int m_socket;
  bool m_opened;
  string m_port;
  string m_host;
  char m_buff[BUFFSIZE + 1];
};

#endif
