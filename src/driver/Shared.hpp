#ifndef _Shared_Hpp_
#define _Shared_Hpp_

#include <cstdint>

static constexpr std::uintptr_t secret_key = 0x2f93416;

enum OperationType
{
	OperationRead,
	OperationWrite,
	OperationBase,
	OperationProtect,
	OperationAllocate,
	OperationFree
};

struct Operation
{
	std::uintptr_t VirtualAddress;
	std::uintptr_t Size;
	std::uintptr_t Buffer;
	std::uint32_t NewProtect;
	std::uint32_t OldProtect;
	const char* ModuleName;
	OperationType Type;
};

struct OperationCommand
{
	std::uintptr_t LocalID;
	std::uintptr_t RemoteID;
	std::uintptr_t SerialKey;
	const char* FilePath;
	Operation Operation;
	std::uintptr_t OperationAddress;
};

#endif // !_Shared_Hpp_