#include "DictProducer.h"
#include <fstream>
#include <iostream>
#include <unistd.h>
using std::ifstream;
using std::ofstream;
using std::cout;
using std::endl;

DictProducer::DictProducer(const string &dir)
:_dir(dir)
{}

void DictProducer::build_dict(){
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

