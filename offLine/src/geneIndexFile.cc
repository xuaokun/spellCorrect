#include "../include/Configure.h"
#include "../include/DictProducer.h"
#include "../include/GeneIndexFile.h"
#include <fstream>
#include <iostream>
#include <unistd.h>
using std::ifstream;
using std::ofstream;
using std::make_pair;
using std::cout;
using std::endl;
void DictProduce(){
    string dir=Configure("../conf/configure.txt").getConfigMap()["English"];
    DictProducer dicProucer(dir);
    dicProucer.buildEnDict();
    dicProucer.store_dict("dic_EN");
    string dir2=Configure("../conf/configure.txt").getConfigMap()["cnLibPath"];
    DictProducer dicProucer2(dir2);
    dicProucer2.addCnFiles();
    dicProucer2.buildCnDict();
    dicProucer2.store_dict("dic_CN");
}
void GeneIndexFile::geneEnIndex(const char* filePath){
    ifstream ifs;
    chdir("../data");
    ifs.open(filePath);
    string word;
    int freq,pos=0;
    while(ifs >> word,ifs >> freq){
        _dic.push_back(make_pair(word,freq));
        for(size_t idx=0;idx<word.size();idx++){
            int len = sizeof(word[idx]);
            _index[string(len,word[idx])].insert(pos);
        }
        ++pos;
    }
    ofstream ofs("Index.txt");
    for(auto & word : _index){
        ofs << word.first << " ";
        for(auto elem:word.second)
            ofs << elem << " ";
        ofs << endl;
    }
    ofs.close();
    cout << filePath << "已经生成英文索引" << endl;
}

void GeneIndexFile::geneCnIndex(const char* filePath){
    ifstream ifs;
    chdir("../data");
    ifs.open(filePath);
    string word;
    string oneWord;
    int freq,pos = _dic.size();
    while(ifs >> word,ifs >> freq){//依次遍历文件中的每一个单词
        _dic.push_back(make_pair(word,freq));
        
        size_t strPos = 0;
        while(strPos != word.size()){
            size_t wordLen = nBytesCode(word[strPos]);//utf-8编码中汉字的第一个字节
            oneWord = word.substr(strPos,wordLen);//截取一个汉字的长度
            strPos += wordLen;
            _index[oneWord].insert(pos);
        }
        ++pos;
    }
    cout << filePath << "已经生成中文索引" << endl;
}

void GeneIndexFile::store(){
    ofstream of("dic.txt");
    for(auto & elem:_dic){
        of << elem.first << " " << elem.second << endl;
    }
    cout << "已经生成中英文词典文件" << endl;
    ofstream ofs("Index.txt");
    for(auto & word : _index){
        ofs << word.first << " ";
        for(auto elem:word.second)
            ofs << elem << " ";
        ofs << endl;
    }
    ofs.close();
    cout << "已经生成中英文索引" << endl;
}

size_t GeneIndexFile::nBytesCode(const char ch){
    if(ch & (1 << 7)){
        int nBytes = 1;
        for(int idx = 0; idx != 6;++idx){
            if(ch & (1 << (6 - idx))){
                ++nBytes;
            }
            else
            {
                break;
            }
        }
        return nBytes;
    }
    return 1;
}

int main(){
    DictProduce();
    GeneIndexFile geneIndexFile;
    geneIndexFile.geneEnIndex("dic_EN");
    geneIndexFile.geneCnIndex("dic_CN");
    geneIndexFile.store();
    return 0;
}