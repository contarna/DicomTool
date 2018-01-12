#pragma once
#include "DcmTK.h"
#include <string>
#include <vector>
using namespace std;

struct NETCONFIG
{
	string IP;
	unsigned int Port;
	string OurAE;
	string PeerAE;
};

class DicomNet
{
public:
	DicomNet();
	~DicomNet();

public:
	NETCONFIG connectCfg;


private:
	char opt_peerip[64];
	char opt_peertitle[64];
	char opt_ourtitle[64];
	unsigned int opt_port;

	unsigned int timeout;
	T_ASC_Network* net;
	T_ASC_Parameters* params;
	T_ASC_Association* assoc;
	DIC_NODENAME localhost;
	DIC_NODENAME peerhost;

	static string logfile;
	bool m_bConnected;

	vector<string>  sendFiles;
public:
	// 连接服务器
	bool Connect();
	void ResultHandle(OFCondition cond, bool bMsgBox=false);
	// 记录日志
	static void LogMsg(string logFile, string msg);
	// 断开连接
	bool DisConnect();
};

