#include "Configure.h"
#include "DictProducer.h"
#include <fstream>
#include <vector>
#include <unordered_map>
#include <set>
#include <utility>
#include <iostream>
#include <unistd.h>
using std::ifstream;
using std::ofstream;
using std::unordered_map;
using std::set;
using std::vector;
using std::pair;
using std::make_pair;
using std::cout;
using std::endl;
void testDictProducer(){
    string dir=Configure("../conf/configure.txt").getConfigMap()["English"];
    DictProducer dicProucer(dir);
    dicProucer.build_dict();
    dicProucer.store_dict("dic_EN");
}
int main(){
    unordered_map<string,set<int>> Index;
    vector<pair<string,int>> dic;
    testDictProducer();
    ifstream ifs;
    chdir("../data");
    ifs.open("dic_EN");
    chdir("../bin");
    string word;
    int freq,pos=0;
    while(ifs >> word,ifs >> freq){
        dic.push_back(make_pair(word,freq));
        for(size_t idx=0;idx<word.size();idx++){
            Index[string(1,word[idx])].insert(pos);
        }
        ++pos;
    }
    chdir("../data");
    ofstream ofs("index.data");
    for(auto & word : Index){
        ofs << word.first << ": ";
        for(auto elem:word.second)
            ofs << elem << " ";
        ofs << endl;
    }
    ofs.close();
    chdir("../bin");
    return 0;
}