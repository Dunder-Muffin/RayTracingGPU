#include "config.h"
#include <map>
static map<string, string> configs;

void add_configs(int config_count, const char** config_value)
{
  char key[255];
  char val[255];
  for (int i = 1; i < config_count; i++)
  {
    sscanf(config_value[i], "-%254s -%254s", key, val);
    configs[string(key)] = string(val);
  }
  string buildPath = string(config_value[0]);
  buildPath = buildPath.substr(0, buildPath.find_last_of('/'));
  string project = configs["project"];
  configs["projectPath"] = buildPath + "/../../Projects/" + project;
  configs["commonPath"] = buildPath + "/../../Common";
}
const char* get_config(const string &config_name)
{
  auto it = configs.find(config_name);
  return it == configs.end() ? nullptr : it->second.c_str();

}

bool get_bool_config(const string &config_name)
{
  auto it = configs.find(config_name);
  return it == configs.end() ? false : (it->second == "on" || it->second == "yes");
}