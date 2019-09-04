#pragma once
#include <string>
#include  <map>
using std::map;
using std::string;

class Configure{
public:
    Configure(const string &);
    map<string,string> & getConfigMap();
private:
    string _filepath;//配置文件路径
    map<string,string> _configMap;//文件内容
};