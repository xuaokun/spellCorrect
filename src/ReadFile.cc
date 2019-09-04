#include "../include/ReadFile.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
using namespace std;

ReadFile* ReadFile::_pInstance = nullptr;
pthread_once_t ReadFile::_once = PTHREAD_ONCE_INIT;
ReadFile *ReadFile::getInstance(){
    pthread_once(&_once,init);
    return _pInstance;
}

void ReadFile::init(){
    if(_pInstance==nullptr){
        _pInstance=new ReadFile();
    }
}

void ReadFile::destroy(){
    if(_pInstance){
        delete _pInstance;
    }
}

void ReadFile::ReadDic(Configure &conf){
    ifstream ifs;
    cout << conf.getConfigMap()["dictFile_EN:"] << endl;
    ifs.open(conf.getConfigMap()["dictFile_EN:"]);
    if(!ifs){
        perror("ifs");
    }
    string word;
    int freq;
    while(ifs >> word,ifs >> freq){
        _dict.push_back(make_pair(word,freq));
    }
    ifs.close();
    cout << conf.getConfigMap()["indexFile:"] << endl;
    ifs.open(conf.getConfigMap()["indexFile:"]);
    if(!ifs){
        perror("ifs");
    }
    while(getline(ifs,word)){
        stringstream sst(word);
        string alp;
        sst >> alp;
        int num;
        while(sst >> num){
            _index[alp].insert(num);
        }
    }
    ifs.close();
}
    void ReadFile::DictPrint(){
    for(auto &item:_dict){
        cout << "word:" << item.first <<" freq:" << item.second << endl;
    }
    for(auto elem:_index){
        cout << elem.first << " num: ";
        for(auto num:elem.second){
            cout << num << " ";
        }
        cout << endl;
    }
}