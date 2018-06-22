#include "stdafx.h"
#include "DicomNet.h"

string DicomNet::logfile = _T("DcmNetlog.log");

DicomNet::DicomNet()
{
	timeout = 1000;
	m_bConnected = false;
	sprintf(localhost, "127.0.0.1");
}


DicomNet::~DicomNet()
{
}


// 连接服务器
bool DicomNet::Connect()
{
	if (m_bConnected)
	{
		return true;
		//DisConnect();
	}

	//Get the parameters of the connect
	strcpy(opt_peertitle, connectCfg.PeerAE.c_str());
	strcpy(opt_ourtitle, connectCfg.OurAE.c_str());
	strcpy(opt_peerip, connectCfg.IP.c_str());
	opt_port = connectCfg.Port;

	char  tips[MAX_BUFFER];
	sprintf(tips, "ASC_initializeNetwork…… timeout limit %d ms", timeout);
	LogMsg(logfile, tips);

	// network struct, contains DICOM upper layer FSM etc.
	OFCondition cond=ASC_initializeNetwork(NET_REQUESTOR, 0, timeout, &net);
	if (cond.bad())
	{
		ResultHandle(cond);
		return false;
	}


	sprintf(tips, "ASC_createAssociationParameters…… MAXPDU %d", ASC_DEFAULTMAXPDU);
	LogMsg(logfile, tips);
	// parameters of association request
	cond =ASC_createAssociationParameters(&params, ASC_DEFAULTMAXPDU);
	if (cond.bad())
	{
		ResultHandle(cond);
		return false;
	}
	
	sprintf(tips, "ASC_setAPTitles peerAE:%s outAE:%s", opt_peertitle,opt_ourtitle);
	LogMsg(logfile, tips);
	// set calling and called AE titles
	cond =ASC_setAPTitles(params, opt_peertitle, opt_ourtitle, NULL);
	if (cond.bad())
	{
		ResultHandle(cond);
		return false;
	}


	sprintf(peerhost, "%s:%d", opt_peerip, opt_port);
	sprintf(tips, "ASC_setPresentationAddresses: loaclhost:%s peerhost:%s", localhost, peerhost);
	LogMsg(logfile, tips);
	
	// the DICOM server accepts connections at server.nowhere.com port 104
	cond=ASC_setPresentationAddresses(params, localhost, peerhost);//这里要替换SCP_IP
	if (cond.bad())
	{
		ResultHandle(cond);
		return false;
	}
																		// list of transfer syntaxes, only a single entry here
	const char* ts[] = { UID_LittleEndianImplicitTransferSyntax };

	// add presentation context to association request
	ASC_addPresentationContext(params, 1, UID_VerificationSOPClass, ts, 1);

	// request DICOM association
	cond = ASC_requestAssociation(net, params, &assoc);
	if (cond.good())
	{
		if (ASC_countAcceptedPresentationContexts(params) == 1)
		{
			// the remote SCP has accepted the Verification Service Class
			DIC_US id = assoc->nextMsgID++; // generate next message ID
			DIC_US status; // DIMSE status of C-ECHO-RSP will be stored here
			DcmDataset *sd = NULL; // status detail will be stored here
								   // send C-ECHO-RQ and handle response
			DIMSE_echoUser(assoc, id, DIMSE_BLOCKING, 0, &status, &sd);
			delete sd; // we don't care about status detail
		}
		//Connected
		m_bConnected = true;
	}
	else
	{
		ResultHandle(cond,true);
		return false;
	}


	return true;
}


void DicomNet::ResultHandle(OFCondition cond, bool bMsgBox/*=false*/)
{
	

	char  msg[MAX_BUFFER];
	sprintf(msg, "%04x:%04x %s\n", cond.module(), cond.code(), cond.text());
	DicomNet::LogMsg(logfile, msg);

	if (bMsgBox)
	{
		::AfxMessageBox(msg);
	}
}


// 记录日志
void DicomNet::LogMsg(string logFile, string msg)
{
	FILE *fp;

	fp = fopen(logFile.c_str(), "at+");
	if (fp != NULL)
	{
		time_t tNowTime;
		time(&tNowTime);
		tm* tLocalTime = localtime(&tNowTime);
		char szTime[30] = { '\0' };
		strftime(szTime, 30, "[%Y-%m-%d %H:%M:%S] ", tLocalTime);
		string str_log = szTime;
		str_log += msg;
		str_log += _T("\n");
		fwrite(str_log.c_str(), sizeof(char), str_log.size(), fp);
		fclose(fp);
	}

	return;
}


// 断开连接
bool DicomNet::DisConnect()
{
	OFCondition cond;
	char  tips[MAX_BUFFER];
	sprintf(tips, "Diconnect…… ");
	LogMsg(logfile, tips);

	if (assoc != nullptr)
	{
		sprintf(tips, "ASC_releaseAssociation…… ");
		LogMsg(logfile, tips);
		cond = ASC_releaseAssociation(assoc);
		if (cond.bad())
		{
			ResultHandle(cond);
			return false;
		}

		sprintf(tips, "ASC_destroyAssociation…… ");
		LogMsg(logfile, tips);
		cond = ASC_destroyAssociation(&assoc);
		if (cond.bad())
		{
			ResultHandle(cond);
			return false;
		}
	}
	if (net != nullptr)
	{
		sprintf(tips, "ASC_dropNetwork…… ");
		LogMsg(logfile, tips);
		cond = ASC_dropNetwork(&net);
		if (cond.bad())
		{
			ResultHandle(cond);
			return false;
		}
	}

	m_bConnected = false;
	return true;
}

T_ASC_Association * DicomNet::GetCurrentAssoc()
{
	return assoc;
}
