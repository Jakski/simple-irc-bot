#include "IRCServer.hpp"

IRCServer::IRCServer(string config)
  : Server(config), m_exit(false)
{
  string nick = m_config.get("Nick"),
	user = m_config.get("User"),
	real = m_config.get("Realname"),
	channel = m_config.get("Channel"),
	buff;
  if (nick == "")
	ERROR("could not get user's nick");
  if (user == "")
	ERROR("could not get username");
  if (real == "")
	ERROR("could not get realname");
  if (channel == "")
  	WARN("no default channel set");
  
  vector<string> args;
  
  // nick 
  args.push_back(nick);
  cmd("NICK", args);
  args.clear();
  
  // handle server's verbosity
  recv(buff);
  // handle handshake ping, if sent with ident checking or as separate message
  recv(buff);
  ping_find(buff);
  recv(buff);
  ping_find(buff);

  // ident
  args.push_back(user);
  args.push_back("- -");
  args.push_back(":" + real);
  cmd("USER", args);
  args.clear();

  // handle greeting
  recv(buff);
  recv(buff);
  
  // join default channel
  join(channel);
}

bool IRCServer::join(string channel)
{
  vector<string> args;
  args.push_back(channel);
  return cmd("JOIN", args);
}

bool IRCServer::part(string channel)
{
  vector<string> args;
  args.push_back(channel);
  return cmd("PART", args);
}

bool IRCServer::cmd(string command, vector<string> args)
{
  string toSend = command;
  for(auto it=args.begin(); it!=args.end(); ++it) {
	toSend = toSend + " " + *it;
  }
  return send(toSend + "\r\n");
}

void IRCServer::add_command(string regexp,
							function <vector<string>(void)> trigger)
{
  m_actions[regexp] = trigger;
}

bool IRCServer::privmsg(string target, string text)
{
  vector<string> args;
  args.push_back(target);
  args.push_back(":" + text);
  return cmd("PRIVMSG", args);
}

bool IRCServer::recv(string &buff)
{
  if (Server::recv(buff)) {
	INFO("received: %s", buff.c_str());
	return true;
  } else {
	return false;
  }
}

bool IRCServer::send(string msg)
{
  if (Server::send(msg)) {
	INFO("sent: %s", msg.c_str());
	return true;
  } else {
	return false;
  }
}

void IRCServer::ping_find(string text)
{
  vector<string> args = parse(text);
  auto it = find(args.begin(), args.end(), "PING");
  if (it != args.end()) {
	ping(*(++it));
  }
}

bool IRCServer::ping(string text)
{
  vector<string> args;
  args.push_back(text);
  return cmd("PONG", args);
}

vector<string> IRCServer::parse(string text)
{
  istringstream ss(text);
  istream_iterator<string> begin(ss), end;
  vector<string> ret(begin, end);
  return ret;
}

void IRCServer::start_loop()
{
  while (!m_exit) {
	string buff;
	if (!recv(buff))
	  continue;
	auto reply = parse(buff);
	if (reply[0] == "PING") {
	  ping(reply[1]);
	} else if (reply[1] == "PRIVMSG" and reply[3] == ":.") {
	  if (reply[4] == "exit") {
		exit_loop();
	  } else if (reply[4] == "hello") {
		privmsg(reply[2], "Hello on " + reply[2] + " channel! :D");
	  } else if (reply[4] == "time") {
		time_t t = time(NULL);
		string r = asctime(localtime(&t));
		privmsg(reply[2], r);
	  } else if (reply[4] == "who_are_you?") {
		privmsg(reply[2], "I'm an integral part of Wired. Nice to meet you :)");
	  }
	}
  }
}

void IRCServer::exit_loop()
{
  m_exit = true;
}
