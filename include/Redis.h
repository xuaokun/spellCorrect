#ifndef _REDIS_H_
#define _REDIS_H_

#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>

#include <hiredis/hiredis.h>
using std::string;

class Redis{
public:
    Redis()
    :_connect(nullptr)
    ,_reply(nullptr)
    {}

    ~Redis(){
        if(_connect){
            redisFree(_connect);
        }
        this->_connect=nullptr;
        this->_reply=nullptr;
    }

    bool connect(string host,int port){
        this->_connect=redisConnect(host.c_str(),port);
        if(this->_connect!=NULL && this->_connect->err){
            printf("connect error:%s\n",this->_connect->errstr);
            return 0;
        }
        printf("Connect to redisServer Success\n"); 
        return 1;
    }

    string get(string key){
        this->_reply =(redisReply *)redisCommand(this->_connect,"GET %s",key.c_str());
        printf("Succeed to execute command:get %s\n",key.c_str());
        if(_reply->type == REDIS_REPLY_NIL){//没有收到任何查询结果
            return string("-1");
        }
            string str = this->_reply->str;
            freeReplyObject(this->_reply);
            return str;
    }

    void set(string key,string value){
        redisCommand(this->_connect,"SET %s %s",key.c_str(),value.c_str());
        printf("Succeed to execute command:set %s val\n",key.c_str());    
    }
private:
    redisContext* _connect;
    redisReply* _reply;
};
#endif