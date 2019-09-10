#pragma once
#include <string>
#include  <map>
#include <vector>
using std::vector;
using std::map;
using std::string;

class DictProducer{
public:
    DictProducer(const string &dir);
    void buildCnDict();
    void addCnFiles();
    void buildEnDict();
    void store_dict(const char *filepath);
    void show_dict();//查看词典，测试用
private:
    string _dir;//语料库文件存放路径
    map<string,int> _dict;//单词与词频的词典
    vector<string> _cnFiles;
};