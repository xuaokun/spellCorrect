///
/// @file    TestAcceptor.cc
/// @author  lemon(haohb13@gmail.com)
/// @date    2019-05-07 15:42:14
///
#include "../include/TextQuery.h"
#include "../include/Threadpool.h"
#include "../include/TcpServer.h"
#include "../include/Configure.h"
#include "../include/ReadFile.h"

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <functional>
using std::cout;
using std::endl;
using std::string;
using namespace wd;
using namespace std::placeholders;
class SpellCorrectServer
{
public:
	SpellCorrectServer(const string  &confFileName)
	:_conf(confFileName)
	,_threadpool(atoi(_conf.getConfigMap()["threadNum:"].c_str()), atoi(_conf.getConfigMap()["queSize:"].c_str()))
	,_server(_conf.getConfigMap()["myIP:"],atoi(_conf.getConfigMap()["myPort:"].c_str()))
	{}
	void start()
	{
		_threadpool.start();
		_server.setConnectionCallback(std::bind(&SpellCorrectServer::onConection,this,_1));
		_server.setMessageCallback(std::bind(&SpellCorrectServer::onMessage,this,_1));
		_server.setCloseCallback(std::bind(&SpellCorrectServer::onClose,this,_1));
		_server.start();
	}
	void onConection(const wd::TcpConnectionPtr &conn)
	{
		cout << conn->toString() << " has connected!" << endl;
		conn->send("welcome to server.");
	}
	void onMessage(const wd::TcpConnectionPtr &conn)
	{
		cout << "onMessage...." << endl;
		string msg = conn->receive();
		cout << ">> receive msg from client: " << msg << endl;
		TextQuery tq(msg, conn);
		_threadpool.addTask(std::bind(&TextQuery::query, tq));
	}
	void onClose(const wd::TcpConnectionPtr &conn)
	{
		cout << "onClose...." << endl;
		cout << conn->toString() << " has closed!" << endl;
	}
	void stop(){
		_threadpool.stop();
	}

	Configure &getConfigure(){
		return _conf;
	}
private:
	Configure  _conf;
	Threadpool _threadpool;
	TcpServer _server;
};

int main(void)
{
	SpellCorrectServer server("../conf/configure.txt");
	ReadFile *pRead=ReadFile::getInstance();
	pRead->ReadDic(server.getConfigure());
	//pRead->DictPrint();
	server.start();
	return 0;
}
