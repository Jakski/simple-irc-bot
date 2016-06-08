#include <iostream>
#include "debug.h"
#include "IRCServer.hpp"

using namespace std;

int main(int argc, char **argv)
{
  IRCServer s("config");
  s.start_loop();
  INFO("main loop finished");
  return 0;
}
