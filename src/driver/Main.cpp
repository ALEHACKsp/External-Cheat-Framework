#include "Global.hpp"
#include "ECFrameworkDriver.hpp"
#include "Log.h"
#include "Shared.hpp"
#include "Socket.h"
#include "Windows.hpp"
#include "Memory.hpp"
#include "Utility.hpp"

NTSTATUS NTAPI DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    UNREFERENCED_PARAMETER(DriverObject);
    UNREFERENCED_PARAMETER(RegistryPath);

    CECFrameworkDriver* ECFrameworkDriver = new CECFrameworkDriver;
    pGlobal = new CGlobal(ECFrameworkDriver);

    {
        SOCKADDR Address{ AF_INET, _byteswap_ushort(9095), INADDR_ANY };

        if (!ECFrameworkDriver->Socket()->Initialize())
            return 0;

        int ServerSocket = ECFrameworkDriver->Socket()->CreateListenSocket(AF_INET, SOCK_STREAM);

        ECFrameworkDriver->Socket()->Bind(ServerSocket, &Address);

        int ClientSocket = ECFrameworkDriver->Socket()->Accept(ServerSocket, &Address);

        {
            char ReceiveBuffer[1024]{ 0 };

            ECFrameworkDriver->Socket()->Receive(ClientSocket, ReceiveBuffer, sizeof(ReceiveBuffer) - 1, 0);
            ReceiveBuffer[sizeof(ReceiveBuffer) - 1] = '\0';

            ECFrameworkDriver->Log()->Print("Received: %s", ReceiveBuffer);
        }

        {
            char SendText[]{ "Hello!" };

            ECFrameworkDriver->Socket()->Send(ClientSocket, SendText, sizeof(SendText), 0);
        }

        ECFrameworkDriver->Socket()->CloseSocket(ServerSocket);
    }

    ECFrameworkDriver->Socket()->Shutdown();

    return STATUS_UNSUCCESSFUL;
}
