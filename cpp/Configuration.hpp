#ifndef HAVE_CONFIGURATION
#define HAVE_CONFIGURATION

#include <iostream>
#include <fstream>
#include <map>

#include "debug.h"

using namespace std;

class Configuration
{
public:
  Configuration(string file);
  string get(string key);
  void set(string key, string value);
  void save();
private:
  fstream m_file;
  string m_filename;
  map <string, string> m_config;
};

#endif
