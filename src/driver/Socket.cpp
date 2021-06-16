#include "Socket.h"

namespace NSocket
{
    NTSTATUS ASyncContextCompletionRoutine(PDEVICE_OBJECT DeviceObject, PIRP Irp, PVOID CompletionEvent)
    {
        UNREFERENCED_PARAMETER(DeviceObject);
        UNREFERENCED_PARAMETER(Irp);

        KeSetEvent(static_cast<KEVENT*>(CompletionEvent), IO_NO_INCREMENT, FALSE);

        return STATUS_MORE_PROCESSING_REQUIRED;
    }
}

CSocket::CSocket()
    : _Registration{ NULL, NULL, NULL }, _Provider{ NULL, NULL }, _Dispatch{ MAKE_WSK_VERSION(1,0), 0, NULL },
    _Sockets{ 0, 0 }
{
}

CSocket::~CSocket()
{
}

bool CSocket::Initialize()
{
    WSK_CLIENT_NPI Client{ NULL, &_Dispatch };

    if (!NT_SUCCESS(WskRegister(&Client, &_Registration)))
        return false;

    if (!NT_SUCCESS(WskCaptureProviderNPI(&_Registration, WSK_INFINITE_WAIT, &_Provider)))
        return false;

    return true;
}

int CSocket::CreateListenSocket(int Domain, int Type)
{
    SOCKET* Socket;

    if (CreateSocket(Socket, static_cast<ADDRESS_FAMILY>(Domain), static_cast<USHORT>(Type), IPPROTO_TCP, WSK_FLAG_LISTEN_SOCKET))
    {
        int SocketIndex = _Sockets.CurrentIndex % 128;

        _Sockets.Array[SocketIndex] = Socket;

        return SocketIndex;
    }

    return -1;
}

bool CSocket::CreateSocket(SOCKET* Socket, ADDRESS_FAMILY AddressFamily, USHORT SocketType, ULONG Protocol, ULONG Flags)
{
    SOCKET* NewSocket = static_cast<SOCKET*>(ExAllocatePoolWithTag(PagedPool, sizeof(SOCKET), 'PTag'));

    if (!NewSocket)
        return false;

    if (!NT_SUCCESS(ASyncContextAllocate(&NewSocket->ASyncContext)))
        return false;

    _Provider.Dispatch
        ->WskSocket(_Provider.Client, AddressFamily, SocketType, Protocol, Flags, NULL, NULL, NULL, NULL, NULL, NewSocket->ASyncContext.Irp);

    KeWaitForSingleObject(&NewSocket->ASyncContext.CompletionEvent, Executive, KernelMode, FALSE, NULL);

    if (NT_SUCCESS(NewSocket->ASyncContext.Irp->IoStatus.Status))
    {
        NewSocket->pSocket = reinterpret_cast<WSK_SOCKET*>(NewSocket->ASyncContext.Irp->IoStatus.Information);
        NewSocket->pDispatch = const_cast<void*>(NewSocket->pSocket->Dispatch);

        *Socket = *NewSocket;
        
        return true;
    }

    return false;
}

int CSocket::Accept(int SocketIndex, SOCKADDR* Address)
{
    SOCKET* Socket = _Sockets.Array[SocketIndex - 1];
    SOCKET* NewSocket;

    KeResetEvent(&Socket->ASyncContext.CompletionEvent);
    IoReuseIrp(Socket->ASyncContext.Irp, STATUS_UNSUCCESSFUL);
    IoSetCompletionRoutine(Socket->ASyncContext.Irp, &NSocket::ASyncContextCompletionRoutine,
        &Socket->ASyncContext.CompletionEvent, TRUE, TRUE, TRUE);

    Socket->pListenDispatch
        ->WskAccept(Socket->pSocket, 0, NULL, NULL, NULL, Address, Socket->ASyncContext.Irp);

    KeWaitForSingleObject(&Socket->ASyncContext.CompletionEvent, Executive, KernelMode, FALSE, NULL);

    if (NT_SUCCESS(Socket->ASyncContext.Irp->IoStatus.Status))
    {
        NewSocket = static_cast<SOCKET*>(ExAllocatePoolWithTag(PagedPool, sizeof(SOCKET), 'PTag'));

        if (!NewSocket)
            return false;

        NewSocket->pSocket = (WSK_SOCKET*)Socket->ASyncContext.Irp->IoStatus.Information;
        NewSocket->pDispatch = (void*)NewSocket->pSocket->Dispatch;

        ASyncContextAllocate(&NewSocket->ASyncContext);

        _Sockets.Array[(_Sockets.CurrentIndex++) % 128] = NewSocket;

        return _Sockets.CurrentIndex % 128;
    }

    return -1;
}

int CSocket::Receive(int SocketIndex, void* Buffer, size_t Length, int Flags)
{
    SOCKET* Socket = _Sockets.Array[SocketIndex - 1];
    ULONG Size = (ULONG)Length;

    return Transfer(Socket, (PVOID)Buffer, &Size, (ULONG)Flags, FALSE);
}

int CSocket::Send(int SocketIndex, const void* Buffer, size_t Length, int Flags)
{
    SOCKET* Socket = _Sockets.Array[SocketIndex - 1];
    ULONG Size = (ULONG)Length;

    return Transfer(Socket, (PVOID)Buffer, &Size, (ULONG)Flags, TRUE);
}

bool Transfer(SOCKET* Socket, PVOID Buffer, PULONG Length, ULONG Flags, BOOLEAN Send)
{
    WSK_BUF WskBuffer{ IoAllocateMdl(Buffer, (ULONG)WskBuffer.Length, FALSE, FALSE, NULL), 0, *Length };

    __try
    {
        MmProbeAndLockPages(WskBuffer.Mdl, KernelMode, IoWriteAccess);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        IoFreeMdl(WskBuffer.Mdl);

        return false;
    }

    KeResetEvent(&Socket->ASyncContext.CompletionEvent);
    IoReuseIrp(Socket->ASyncContext.Irp, STATUS_UNSUCCESSFUL);
    IoSetCompletionRoutine(Socket->ASyncContext.Irp, &NSocket::ASyncContextCompletionRoutine,
        &Socket->ASyncContext.CompletionEvent, TRUE, TRUE, TRUE);

    if (Send)
        Socket->pConnectionDispatch->WskSend(Socket->pSocket, &WskBuffer, Flags, Socket->ASyncContext.Irp);
    else
        Socket->pConnectionDispatch->WskReceive(Socket->pSocket, &WskBuffer, Flags, Socket->ASyncContext.Irp);

    KeWaitForSingleObject(&Socket->ASyncContext.CompletionEvent, Executive, KernelMode, FALSE, NULL);
    MmUnlockPages(WskBuffer.Mdl);
    IoFreeMdl(WskBuffer.Mdl);
    
    if (NT_SUCCESS(Socket->ASyncContext.Irp->IoStatus.Status))
    {
        *Length = (ULONG)Socket->ASyncContext.Irp->IoStatus.Information;
        return true;
    }
    else
    {
        return false;
    }
}

bool CSocket::Bind(int SocketIndex, const sockaddr* LocalAddress)
{
    SOCKET* Socket = _Sockets.Array[SocketIndex - 1];

    KeResetEvent(&Socket->ASyncContext.CompletionEvent);
    IoReuseIrp(Socket->ASyncContext.Irp, STATUS_UNSUCCESSFUL);
    IoSetCompletionRoutine(Socket->ASyncContext.Irp, &NSocket::ASyncContextCompletionRoutine, 
        &Socket->ASyncContext.CompletionEvent, TRUE, TRUE, TRUE);

    Socket->pListenDispatch
        ->WskBind(Socket->pSocket, const_cast<sockaddr*>(LocalAddress), 0, Socket->ASyncContext.Irp);

    KeWaitForSingleObject(&Socket->ASyncContext.CompletionEvent, Executive, KernelMode, FALSE, NULL);

    if (NT_SUCCESS(Socket->ASyncContext.Irp->IoStatus.Status))
        return false;

    return true;
}

int CSocket::CloseSocket(int SocketIndex)
{
    SOCKET* Socket = _Sockets.Array[_Sockets.CurrentIndex - 1];

    KeResetEvent(&Socket->ASyncContext.CompletionEvent);
    IoReuseIrp(Socket->ASyncContext.Irp, STATUS_UNSUCCESSFUL);
    IoSetCompletionRoutine(Socket->ASyncContext.Irp, &NSocket::ASyncContextCompletionRoutine,
        &Socket->ASyncContext.CompletionEvent, TRUE, TRUE, TRUE);

    Socket->pConnectionDispatch->Basic.WskCloseSocket(Socket->pSocket, Socket->ASyncContext.Irp);

    KeWaitForSingleObject(&Socket->ASyncContext.CompletionEvent, Executive, KernelMode, FALSE, NULL);
    IoFreeIrp(Socket->ASyncContext.Irp);
    ExFreePoolWithTag(Socket, 'PTag');

    _Sockets.Array[_Sockets.CurrentIndex - 1] = NULL;

    if (!NT_SUCCESS(Socket->ASyncContext.Irp->IoStatus.Status))
        return false;

    return true;
}

bool CSocket::ASyncContextAllocate(SOCKET_ASYNC_CONTEXT* ASyncContext)
{
    KeInitializeEvent(&ASyncContext->CompletionEvent, SynchronizationEvent, FALSE);

    ASyncContext->Irp = IoAllocateIrp(1, FALSE);

    if (!ASyncContext->Irp)
        return false;

    IoSetCompletionRoutine(ASyncContext->Irp, &NSocket::ASyncContextCompletionRoutine, &ASyncContext->CompletionEvent, TRUE, TRUE, TRUE);

    return true;
}

void CSocket::Shutdown()
{
    WskReleaseProviderNPI(&_Registration);
    WskDeregister(&_Registration);
}

// ???????????????????
int CSocket::Listen(int SocketIndex, int BackLog)
{
    UNREFERENCED_PARAMETER(SocketIndex);
    UNREFERENCED_PARAMETER(BackLog);

    return 0;
}
// ???????????????????
