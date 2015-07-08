

#include "stdio.h"
#include "FixClientSock.h"
#include <winsock2.h>
#pragma  comment(lib,"wsock32.lib")



FixTcpClient::FixTcpClient()
{
	server_port = 8092;
}

FixTcpClient::~FixTcpClient()
{
}

int FixTcpClient::StartSock(char *szServerip, int nport)
{
	WSADATA WSAData;		///////////////

	strcpy_s(server_ip,20,szServerip);
	server_port = nport;

	if(WSAStartup(MAKEWORD(2,2),&WSAData)!=0)
	{
		printf("sock init fail!\n");
		return -1 ;
	}

	

	return 1 ;
}


int FixTcpClient::CreateSock()
{
	sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock==SOCKET_ERROR)
	{
		printf("sock create fail!\n");
		WSACleanup();
		return(-1);
	}
	return(1);
}


int FixTcpClient::CallServer()
{
	sockaddr_in ServerAddr;
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_addr.s_addr = inet_addr(server_ip);
	ServerAddr.sin_port   = htons(server_port);

	CreateSock();
	int ret;
	ret = connect(sock,(struct sockaddr *)&ServerAddr,sizeof(ServerAddr));
	if (ret == SOCKET_ERROR)
	{
		int err = GetLastError();
		printf("Connect fail! \n");
		closesocket(sock);
		return(-1);
	}
	return(1);
}


int FixTcpClient::TCPSend(char* data)
{
	int length;
	length = send(sock,data,strlen(data),0);
	//TRACE_LOG("SP  TCPSend:%s .", data);
	if (length<=0)
	{
		//TRACE_LOG("send data error!");
		closesocket(sock);
		WSACleanup();
		return(-1);
	}
	return(1);
}

int FixTcpClient::TCPRecive(char * Recvbuff)
{
	int length;
	length = recv(sock, Recvbuff, 1023, 0);
	if (length<=0)
	{
		return(-1);
	}
	return length;
}

int FixTcpClient::TCPClose()
{
	closesocket(sock);
	WSACleanup();
	return 1;
}

int FixTcpClient::TCPRecvLine(char *Recvbuff)
{
	int length,ret;
	length=0;
	char * buff = Recvbuff;
	while(1)
	{
		ret = recv(sock, buff, 1, 0);
		if (ret<=0)
		{
			return(-1);
		}
		else
		{
			length++;
			if((*buff)=='\n')
			{
				break;
			}
		}
		buff++;
	}
	return length;
}

