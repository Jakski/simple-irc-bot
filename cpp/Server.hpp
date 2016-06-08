#ifndef HAVE_SERVER
#define HAVE_SERVER

#include <iostream>

#include "TCPConnection.hpp"
#include "Configuration.hpp"

using namespace std;

class Server
  : public TCPConnection
{
public:
  Server(string config);
protected:
  Configuration m_config;
};

#endif
