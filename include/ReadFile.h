#include "../include/Configure.h"
#include <pthread.h>
#include <vector>
#include <unordered_map>
#include <set>
#include <iostream>
#include <utility>
using namespace std;
class ReadFile{
public:
    static ReadFile *getInstance();
    static void init();
    static void destroy();
    void ReadDic(Configure &conf);//产生vector和unordered_map
    void DictPrint();//测试
    vector<pair<string,int>> &getDict(){
        return _dict;
    }
    unordered_map<string,set<int>> &getIndex(){
        return _index;
    }
private:
    ReadFile(){}
    ~ReadFile(){cout << "~ReadFile()" << endl;}
private:
    static ReadFile *_pInstance;
    static pthread_once_t _once;
    vector<pair<string,int>> _dict;//存储单词和词频
    unordered_map<string,set<int>> _index;//候选词索引（int型的set集合为单词在vector中的下标）
};