#pragma once
#include "../include/Threadpool.h"
#include "../include/TcpServer.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <queue>
typedef struct train{
	int jsonLen;
	char buf[65535];
}train_t;

using std::string;
using namespace wd;

struct MyResult{
    string _word;
    int _iFreq;
    int _iDist;
public:
	string get_word(){
		return _word;
	}
	int get_iFreq(){
		return _iFreq;
	}
	int get_iDist(){
		return _iDist;
	}
};
struct comp{
	bool operator()(MyResult left,MyResult right) const {
		if(left.get_iDist()==right.get_iDist()){
			if(left.get_iFreq()==right.get_iFreq()){
				return left.get_word() > right.get_word();
			}
			return left.get_iFreq() < right.get_iFreq();
		}
		return left.get_iDist() > right.get_iDist();
	}
};
class TextQuery
{
public:
	TextQuery(const string & msg,const TcpConnectionPtr & conn)
	: _msg(msg)
	,_response()
	,_conn(conn)
	{
		_msg[_msg.size()-1] = '\0';
	}
	//运行在线程池的某一个子线程中
	int queryCache();//查询缓存
	void query();//处理业务逻辑
	void reply();//将查询结果发送回客户端
	int distance(const string & rhs);//计算最小编辑距离
	void geneJson();//生成json格式的字符串
private:
	string _msg;
	string _response;
	TcpConnectionPtr _conn;
	std::priority_queue<MyResult,vector<MyResult>,comp> _resultQue;
};