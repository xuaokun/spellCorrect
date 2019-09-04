#include "../include/TextQuery.h"
#include "../include/Configure.h"
#include "../include/ReadFile.h"
#include "../include/EditDistance.h"
#include <iostream>
#include <set>
#include <json/json.h>
using std::cout;
using std::endl;
using std::set;
using namespace Json;


void TextQuery::query(){//处理业务逻辑
    set<int> numSet;//单词序号的集合，去除重复序号
    ReadFile *pread=ReadFile::getInstance();
    for(size_t i=0;i<_msg.size();i++){//依次遍历查询单词的每个字母
        for(auto number:pread->getIndex()[string(1,_msg[i])]){//遍历set集合
           numSet.insert(number);
        }
    }
    for(int number:numSet){
        MyResult myresult;
        myresult._word=(pread->getDict()[number]).first;
        myresult._iFreq=(pread->getDict()[number]).second;
        myresult._iDist=distance(myresult._word);
        _resultQue.push(myresult);
    }

    cout << ">>" << current_thread::threadName << ": task process() " << endl;
    string response = geneJson();//要返回给客户端的消息
    int jsonLen = sizeof(response);//使用小火车私有协议
    //_conn->send(response);//由线程池的线程(计算线程)完成数据的发送,在设计上来说，是不合理的
                            //数据发送的工作要交还给IO线程(Reactor所在的线程)完成
                            //将send的函数的执行延迟到IO线程去操作
    _conn->sendInLoop(string(1,jsonLen));                   
    _conn->sendInLoop(response);
}

int TextQuery::distance(const string & rhs){
    return editDistance(_msg,rhs);
}

string TextQuery::geneJson(){
    Json::Value root;
    for(int i=0;i<3;i++){
        root[_msg].append(_resultQue.top()._word);
        _resultQue.pop();
    }
    Json::StyledWriter stylewriter;
    cout << stylewriter.write(root) << endl;
    return stylewriter.write(root);
}