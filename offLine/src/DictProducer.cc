#include "../include/DictProducer.h"
#include "../../include/cppjieba/Jieba.hpp"
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <dirent.h>
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;
DictProducer::DictProducer(const string &dir)
:_dir(dir)
{}

    void DictProducer::buildEnDict(){
    ifstream ifs;
    ifs.open(_dir);
    string strTmp;
    string wordTmp;
    int from,to;
    while(wordTmp.clear(),ifs>>wordTmp){
        from=-1;
        to=-1;
        for(int i=0;i<wordTmp.size();i++){
            if(wordTmp[i]>='A' && wordTmp[i]<='Z')
                wordTmp[i]=tolower(wordTmp[i]);
            if(from<0 && wordTmp[i]>='a' && wordTmp[i]<='z'){
                from=i;
            }
            if(from>=0 && to<0 &&(wordTmp[i]<='a' || wordTmp[i]>='z')){
                to=i-1;
            }
        }
        if(from>=0){
            if(to<0)
                to=wordTmp.size()-1;
            wordTmp=wordTmp.substr(from,to-from+1);
            if(wordTmp!=string())
                _dict[wordTmp]++;
        }
    }
    ifs.close();
}
void DictProducer::addCnFiles()
{
    string path = _dir;
    struct dirent* pDirInfo;
    DIR* pDir;
    pDir = opendir(path.c_str());
    if(pDir == nullptr){
        perror("opendir");
        return;
    }
    while((pDirInfo = readdir(pDir)) != nullptr)
    {
        if(*(pDirInfo->d_name) != '.')
            _cnFiles.push_back(path+'/'+string(pDirInfo->d_name));
    }
    closedir(pDir);
    cout << "已读取中文语料库目录信息" << endl;
    for(auto & s : _cnFiles)
        cout << s << endl;
    cout << endl;
}

void DictProducer::buildCnDict()
{
    using namespace cppjieba;
    Jieba jieba("../data/jiebaDict/jieba.dict.utf8",
                "../data/jiebaDict/hmm_model.utf8",
                "../data/jiebaDict/user.dict.utf8",
                "../data/jiebaDict/idf.utf8",
                "../data/jiebaDict/stop_words.utf8");
    for(auto & path : _cnFiles)
    {
        ifstream is(path);
        if(!is){
            perror("fopen");
            return;
        }
        string s;
        while(is >> s)
        {
            vector<string> words;
            jieba.Cut(s,words,true);
            for(auto & word:words)
                _dict[word]++;
        }
        is.close();
    }
    cout << "已读取中文语料库中所有单词共 : " << _dict.size() << "个" << endl;
    cout << endl;
}

void DictProducer::store_dict(const char *filepath){
    chdir("../data");
    ofstream ofs(filepath);
    for(auto & word : _dict){
        ofs << word.first << " " << word.second << "\n";
    }
    ofs.close();
    chdir("../bin");
}

void DictProducer::show_dict(){
        for(auto & word : _dict){
        cout << word.first << " " << word.second << "\n";
    }
}

