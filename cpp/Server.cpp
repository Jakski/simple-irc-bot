#include "Server.hpp"

Server::Server(string config)
  : m_config(config)
{
  connect(m_config.get("Host"), m_config.get("Port"));
}
