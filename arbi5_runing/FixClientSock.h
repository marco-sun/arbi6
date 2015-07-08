#pragma once

class FixTcpClient
{
public:
	FixTcpClient();
	virtual ~FixTcpClient();
	
	int StartSock(char *szServerip, int nport);
	int CreateSock();
	int CallServer();
	int TCPSend(char* data);
	int TCPRecive(char * Recvbuff);
	int TCPClose();
    int TCPRecvLine(char *Recvbuff);

private:
	char server_ip[20];
	int server_port;
	unsigned int sock;
//	
	
		
};