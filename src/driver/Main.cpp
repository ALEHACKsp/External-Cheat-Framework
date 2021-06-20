#include "Global.hpp"
#include "ECFrameworkDriver.hpp"
#include "Log.h"
#include "Shared.hpp"
#include "Socket.h"
#include "Windows.hpp"
#include "Memory.hpp"
#include "Utility.hpp"

VOID ServerThread(PVOID Context)
{ 
    UNREFERENCED_PARAMETER(Context);
    CECFrameworkDriver* ECFrameworkDriver = new CECFrameworkDriver;

    if (ECFrameworkDriver->Socket()->Initialize())
    {
        SOCKADDR_IN Address{ AF_INET, _byteswap_ushort(9095), INADDR_ANY, 0 };
        int ClientConnection{ 0 };
        int ServerSocket = ECFrameworkDriver->Socket()->CreateListenSocket(AF_INET, SOCK_STREAM);
        
        if (ServerSocket == ~0)
        {
            ECFrameworkDriver->Socket()->Shutdown();
            ECFrameworkDriver->Log()->Print("ServerThread(): CSocket::CreateListenSocket() Failed.");

            return;
        }
        
        if (ECFrameworkDriver->Socket()->Bind(ServerSocket, reinterpret_cast<SOCKADDR*>(&Address)))
        {
            ECFrameworkDriver->Socket()->CloseSocket(ServerSocket);
            ECFrameworkDriver->Socket()->Shutdown();
            ECFrameworkDriver->Log()->Print("ServerThread(): CSocket::Bind() Failed.");

            return;
        }

        if (ECFrameworkDriver->Socket()->Listen(ServerSocket, 10) == -1)
        {
            ECFrameworkDriver->Socket()->CloseSocket(ServerSocket);
            ECFrameworkDriver->Socket()->Shutdown();
            ECFrameworkDriver->Log()->Print("ServerThread(): CSocket::Listen() Failed.");

            return;
        }

        while (!ClientConnection && !ECFrameworkDriver->bShutdown())
        {
            ClientConnection = ECFrameworkDriver->Socket()->Accept(ServerSocket, reinterpret_cast<SOCKADDR*>(&Address));

            if (ClientConnection == ~0)
            {
                ECFrameworkDriver->Log()->Print("ServerThread(): CSocket::Accept() Failed.");

                break;
            }
        }

        // TODO
        /*while (ClientConnection && !ECFrameworkDriver->bShutdown())
        {
            CommandPacket Packet{};
            DWORD64 PacketResult{};
            
            if (ECFrameworkDriver->Socket()->Receive(ClientConnection, (void*)&Packet, sizeof(CommandPacket), 0))
            {
                if (Packet.Header != PacketMagic)
                    continue;

                if (!ProcessPacket(Packet, &PacketResult))
                    continue;

                if (!ProcessRequest(ClientConnection, PacketResult))
                    break;
            }
        }*/

        ECFrameworkDriver->Socket()->CloseSocket(ServerSocket);
        ECFrameworkDriver->Socket()->Shutdown();
    }

    delete ECFrameworkDriver;
}

NTSTATUS NTAPI DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(DriverObject);
    UNREFERENCED_PARAMETER(RegistryPath);

    CLog* EntryLog = new CLog;
    HANDLE Thread{ nullptr };

    if (!NT_SUCCESS(PsCreateSystemThread(&Thread, GENERIC_ALL, 0, 0, 0, ServerThread, 0)))
    {
        EntryLog->Print("Failed to Create Server Thread.");

        return STATUS_UNSUCCESSFUL;
    }

    ZwClose(Thread);
    delete EntryLog;

    return STATUS_SUCCESS;
}