#pragma once
#include <string>
using namespace std;

void add_configs(int config_count, const char** config_value);
const char* get_config(const string &config_name);
bool get_bool_config(const string &config_name);

