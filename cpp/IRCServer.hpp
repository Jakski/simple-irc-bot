#ifndef HAVE_IRCSERVER
#define HAVE_IRCSERVER

#include <iostream>
#include <vector>
#include <functional>
#include <regex>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <ctime>

#include "Server.hpp"

using namespace std;

class IRCServer: public Server
{
public:
  IRCServer(string config);
  bool join(string channel);
  bool part(string channel);
  bool cmd(string command, vector<string> args);
  void add_command(string regexp, function<vector<string>(void)> trigger);
  void start_loop();
  bool privmsg(string target, string text);
  bool recv(string &buff);
  bool send(string msg);
private:
  void ping_find(string text);
  bool ping(string text);
  vector<string> parse(string text);
  void exit_loop();
  bool m_exit;
  map<string, function<vector<string>(void)>> m_actions;
};

#endif
