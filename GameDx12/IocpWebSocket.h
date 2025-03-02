#pragma once
#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <thread>
class IocpWebSocket
{
public:
	static IocpWebSocket* IcopWebSocketInstance() {
		if (!lpIocpWebSocket) {
			lpIocpWebSocket = new IocpWebSocket();
			return lpIocpWebSocket;
		}
		return lpIocpWebSocket;
	}
	static IocpWebSocket* lpIocpWebSocket;

private:
	IocpWebSocket() {};
	~IocpWebSocket() {
		CloseHandle(m_hCompeletionPort);
	};
	void initializeIocpWebSocket()
	{
		m_hCompeletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 1);
	}



private:
	HANDLE m_hCompeletionPort;

private:
	static void releaseInstance()
	{

		if (lpIocpWebSocket)delete lpIocpWebSocket;

	}
	class IcopWebSocketHelp
	{
	public:
		IcopWebSocketHelp() {
			lpIocpWebSocket = IcopWebSocketInstance();
		}
		~IcopWebSocketHelp() {
			releaseInstance();
		}

	};
	
	static IcopWebSocketHelp iocpwebsockethelp;
};

