#ifndef HAVE_CONNECTION
#define HAVE_CONNECTION

#include <iostream>

using namespace std;

class Connection
{
public:
  virtual bool send(string msg) = 0;
  virtual bool recv(string &buff) = 0;
  virtual void connect(string host, string port) = 0;
  virtual void disconnect() = 0;
};

#endif
