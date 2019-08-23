#include "Configure.h"
#include <iostream>
#include <fstream>
using std::ifstream;
using std::cout;
using std::endl;

Configure::Configure(const string &filepath)
:_filepath(filepath)
{}
map<string,string> & Configure::getConfigMap(){
    ifstream ifs;
    ifs.open(_filepath);
    string mapKey,mapValue;
    while(ifs>>mapKey){
        ifs>>mapValue;
        _configMap[mapKey]=mapValue;
    }
    return _configMap;
}
int test(){
    Configure conf("../conf/configure.txt");
    cout << "Filename " << conf.getConfigMap()["English"] << endl;
    return 0;
}
