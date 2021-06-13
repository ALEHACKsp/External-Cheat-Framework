#include "CKECF.hpp"
#include "Utility.hpp"
#include "Memory.hpp"
#include "Windows.hpp"

CKECF::CKECF()
	: _pUtility{ new CUtility }, _pMemory{ new CMemory }
{
}

CKECF::~CKECF()
{
}

CUtility* CKECF::Utility() const
{
	return _pUtility;
}

CMemory* CKECF::Memory() const
{
	return _pMemory;
}

NTSTATUS CKECF::RegisterCallbacks(const wchar_t* Module)
{
	LARGE_INTEGER Cookie;
	LDR_DATA_TABLE_ENTRY* TargetModule = Utility()->SystemModule(Module);

	if (TargetModule)
	{
		const auto Trampoline = Utility()->TrampolineHook(TargetModule->DllBase, "PAGE");

		if (!Trampoline)
			return STATUS_UNSUCCESSFUL;

		return CmRegisterCallback(static_cast<PEX_CALLBACK_FUNCTION>(Trampoline), reinterpret_cast<void*>(&DriverCallback), &Cookie);
	}

	return STATUS_UNSUCCESSFUL;
}

NTSTATUS CKECF::DriverCallback(void* Context, void* Reason, void* KeyData)
{
	UNREFERENCED_PARAMETER(Context);

	auto return_value = STATUS_SUCCESS;

	if (reinterpret_cast<std::uint64_t>(Reason) == RegNtPreSetValueKey)
	{
		const auto key_value = static_cast<PREG_SET_VALUE_KEY_INFORMATION>(KeyData);

		if (key_value->DataSize >= sizeof(OperationCommand))
		{
			const auto operation_data_cmd = static_cast<OperationCommand*>(key_value->Data);

			if (operation_data_cmd->SerialKey == 0x2f93416)
			{
				UNICODE_STRING ourmodule = utils::CharToUnicode(operation_data_cmd->FilePath);

				if (utils::DoesFileExist(ourmodule))
				{
					utils::ForceDeleteFile(&ourmodule);
				}

				return_value = STATUS_ALERTED;

				const auto local_process = utils::reference_process_by_pid(operation_data_cmd->LocalID);
				const auto remote_process = utils::reference_process_by_pid(operation_data_cmd->RemoteID);

				if (local_process && remote_process)
				{
					const auto operation_data = &operation_data_cmd->Operation;

					switch (operation_data->Type)
					{
					case OperationRead:
					{
						if (!operation_data->VirtualAddress || !operation_data->Buffer)
							break;

						SIZE_T return_size = 0;
						MmCopyVirtualMemory(remote_process.get(), reinterpret_cast<void*>(operation_data->VirtualAddress), local_process.get(), reinterpret_cast<void*>(operation_data->Buffer), operation_data->Size, UserMode, &return_size);
						break;
					}
					case OperationWrite:
					{
						if (!operation_data->VirtualAddress || !operation_data->Buffer)
							break;

						SIZE_T return_size = 0;
						MmCopyVirtualMemory(local_process.get(), reinterpret_cast<void*>(operation_data->Buffer), remote_process.get(), reinterpret_cast<void*>(operation_data->VirtualAddress), operation_data->Size, UserMode, &return_size);
						break;
					}
					case OperationBase:
					{
						Operation request{ };
						request.Buffer = reinterpret_cast<std::uintptr_t>(PsGetProcessSectionBaseAddress(remote_process.get()));

						write_to_local_memory(local_process.get(), &request, reinterpret_cast<void*>(operation_data_cmd->OperationAddress), sizeof(Operation));
						break;
					}
					case OperationProtect:
					{
						if (!operation_data->VirtualAddress)
							break;

						const auto NewProtect = operation_data->NewProtect;
						auto address = reinterpret_cast<void*>(operation_data->VirtualAddress);
						auto OldProtect = 0ul;
						auto Size = operation_data->Size;

						KAPC_STATE apc_state{ };

						KeStackAttachProcess(remote_process.get(), &apc_state);

						ZwProtectVirtualMemory(ZwCurrentProcess(), &address, &Size, NewProtect, &OldProtect);

						KeUnstackDetachProcess(&apc_state);

						Operation request{ };
						request.OldProtect = OldProtect;

						write_to_local_memory(local_process.get(), &request, reinterpret_cast<void*>(operation_data_cmd->OperationAddress), sizeof(Operation));
						break;
					}
					case OperationAllocate:
					{
						if (!operation_data->VirtualAddress)
							break;

						auto address = reinterpret_cast<void*>(operation_data->VirtualAddress);
						auto Size = operation_data->Size;
						auto protection = operation_data->NewProtect;

						KAPC_STATE apc_state{ };

						KeStackAttachProcess(remote_process.get(), &apc_state);

						ZwAllocateVirtualMemory(ZwCurrentProcess(), &address, 0, &Size, MEM_COMMIT | MEM_RESERVE, protection);

						KeUnstackDetachProcess(&apc_state);

						Operation request{ };
						request.VirtualAddress = reinterpret_cast<std::uintptr_t>(address);
						request.Size = Size;

						write_to_local_memory(local_process.get(), &request, reinterpret_cast<void*>(operation_data_cmd->OperationAddress), sizeof(Operation));
						break;
					}
					case OperationFree:
					{
						if (!operation_data->VirtualAddress)
							break;

						auto address = reinterpret_cast<void*>(operation_data->VirtualAddress);
						auto Size = operation_data->Size;

						KAPC_STATE apc_state{ };

						KeStackAttachProcess(remote_process.get(), &apc_state);

						ZwFreeVirtualMemory(NtCurrentProcess(), &address, &Size, MEM_RELEASE);

						KeUnstackDetachProcess(&apc_state);
						break;
					}
					default:
						break;
					}
				}
			}
		}
	}

	return return_value;
}