#include "../include/TextQuery.h"
#include "../include/Configure.h"
#include "../include/ReadFile.h"
#include "../include/EditDistance.h"
#include "../include/Redis.h"

#include <iostream>
#include <set>
#include <json/json.h>
#include <hiredis/hiredis.h>
using std::cout;
using std::endl;
using std::set;
using namespace Json;

extern __thread const char* current_thread::threadName;
extern __thread Redis* redis;

int TextQuery::queryCache(){
    _response = redis->get(_msg);
    cout << _response << endl;
    if(_response == "-1"){
        cout << current_thread::threadName << "缓存未命中" << endl;
        return -1;
    }
    else{
        cout << current_thread::threadName << "缓存命中" << endl;
        reply();
        return 0;
    }
}

void TextQuery::query(){//处理业务逻辑
    int ret = queryCache();
    if(ret == 0){
        return;
    }
    set<int> numSet;//单词序号的集合，去除重复序号
    ReadFile *pread=ReadFile::getInstance();
    for(size_t i=0;i<_msg.size();i++){//依次遍历查询单词的每个字母
        for(auto number:pread->getIndex()[string(1,_msg[i])]){//遍历set集合
           numSet.insert(number);
        }
    }
    for(int number:numSet){//将候选词按最小编辑距离由小到大加入集合中
        MyResult myresult;
        myresult._word=(pread->getDict()[number]).first;
        myresult._iFreq=(pread->getDict()[number]).second;
        myresult._iDist=distance(myresult._word);
        _resultQue.push(myresult);
    }
    geneJson();//以json格式要返回给客户端的消息
    reply();//消息回送给客户端
    redis->set(_msg,_response);//将查询结果保存至缓存
    cout << "已将该候选词结果存在cache:" << _response << endl;
}

void TextQuery::reply(){

    int jsonLen = sizeof(_response);//使用小火车私有协议
    //_conn->send(response);//由线程池的线程(计算线程)完成数据的发送,在设计上来说，是不合理的
                            //数据发送的工作要交还给IO线程(Reactor所在的线程)完成
                            //将send的函数的执行延迟到IO线程去操作
    _conn->sendInLoop(string(1,jsonLen));                   
    _conn->sendInLoop(_response);
}

int TextQuery::distance(const string & rhs){
    return editDistance(_msg,rhs);
}

void TextQuery::geneJson(){
    Json::Value root;
    for(int i=0;i<3;i++){
        root[_msg].append(_resultQue.top()._word);
        cout << _resultQue.top()._word << endl;
        _resultQue.pop();
    }
    Json::StyledWriter stylewriter;
    cout << stylewriter.write(root) << endl;
    _response = stylewriter.write(root);
}