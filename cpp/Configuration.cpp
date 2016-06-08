#include "Configuration.hpp"

Configuration::Configuration(string file)
  : m_filename(file)
{
  m_file.open(m_filename, ios::in | ios::out);
  if (!m_file.is_open())
	INFO("creating configuration file: %s", m_filename.c_str());
  else
	while (!m_file.eof()) {
	  string key, val;
	  m_file >> key >> val;
	  if (key.size() > 0)
		m_config[key] = val;
    }
  m_file.close();
}

string Configuration::get(string key)
{
  try {
	return m_config.at(key);
  } catch(out_of_range ex) {
	WARN("could not get configuration value: %s", ex.what());
	return "";
  }
}

void Configuration::set(string key, string value)
{
  m_config[key] = value;
}

void Configuration::save()
{
  m_file.open(m_filename, ios::out | ios::trunc);
  for (auto it = m_config.begin(); it != m_config.end(); it++) {
	INFO("%s => %s", it->first.c_str(), it->second.c_str());
	m_file << it->first << " " << it->second << endl;
  }
  m_file.close();
}
