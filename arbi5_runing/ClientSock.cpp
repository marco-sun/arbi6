

#include "stdio.h"
#include "ClientSock.h"
#include <winsock2.h>
#include <fcntl.h>
#pragma  comment(lib,"wsock32.lib")



CTcpClinet::CTcpClinet()
{
	server_port = 8092;
}

CTcpClinet::~CTcpClinet()
{
}

int CTcpClinet::StartSock(char *szServerip, int nport)
{
	WSADATA WSAData;		///////////////

//	strcpy_s(server_ip,20,szServerip);
	strcpy(server_ip,szServerip);
	server_port = nport;

	if(WSAStartup(MAKEWORD(2,2),&WSAData)!=0)
	{
		printf("sock init fail!\n");
		return -1 ;
	}

	

	return 1 ;
}


int CTcpClinet::CreateSock()
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


int CTcpClinet::CallServer()
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


int CTcpClinet::TCPSend(char* data)
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

int CTcpClinet::TCPRecive(char * Recvbuff)
{
	int length;
	length = recv(sock, Recvbuff, 1023, 0);
	if (length<=0)
	{
		return(-1);
	}
	return length;
}

int CTcpClinet::TCPClose()
{
	closesocket(sock);
	WSACleanup();
	return 1;
}

int CTcpClinet::TCPRecvLine(char *Recvbuff)
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

