#ifndef _Socket_H_
#define _Socket_H_

#include <ntddk.h>
#include <wsk.h>

struct SOCKET_ASYNC_CONTEXT
{
	KEVENT CompletionEvent;
	PIRP Irp;
};

struct SOCKET
{
	WSK_SOCKET*	pSocket;
	SOCKET_ASYNC_CONTEXT ASyncContext;

	union
	{
		void* pDispatch;
		WSK_PROVIDER_CONNECTION_DISPATCH* pConnectionDispatch;
		WSK_PROVIDER_LISTEN_DISPATCH* pListenDispatch;
		WSK_PROVIDER_DATAGRAM_DISPATCH* pDatagramDispatch;
		WSK_PROVIDER_STREAM_DISPATCH* pStreamDispatch;
	};
};

struct SOCKET_ARRAY
{
	SOCKET* Array[128];
	ULONG CurrentIndex;
};

class CSocket
{
	// Members
public:

private:
	WSK_REGISTRATION _Registration;
	WSK_PROVIDER_NPI _Provider;
	WSK_CLIENT_DISPATCH _Dispatch;
	SOCKET_ARRAY _Sockets;

	// Methods
public:
	CSocket();
	~CSocket();

	bool Initialize();
	int CreateListenSocket(int Domain, int Type);
	bool CreateSocket(SOCKET* Socket, ADDRESS_FAMILY AddressFamily, USHORT SocketType, ULONG Protocol, ULONG Flags);
	int Accept(int SocketIndex, SOCKADDR* Address);
	int Receive(int SocketIndex, void* Buffer, size_t Length, int Flags);
	int Send(int SocketIndex, const void* Buffer, size_t Length, int Flags);
	bool Transfer(SOCKET* Socket, PVOID Buffer, PULONG Length, ULONG Flags, BOOLEAN Send);
	bool Bind(int SocketIndex, const SOCKADDR* Address);
	int CloseSocket(int SocketIndex);
	bool ASyncContextAllocate(SOCKET_ASYNC_CONTEXT* ASyncContext);
	void Shutdown();
	int Listen(int SocketIndex, int BackLog);

private:

};

#endif // _Socket_H_
