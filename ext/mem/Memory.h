#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <string>
#include "../utils/TypeDefs.h"
#include <memory>

extern "C" ll Mem_ReadVirtualMemory(
	HANDLE ProcecssHandle,
	PVOID addr,
	PVOID bfr,
	ULONG numBytesToRead,
	PULONG numOfBytesRead
);

extern "C" ll Mem_WriteVirtualMemory(
	HANDLE ProcessHandle,
	PVOID addr,
	PVOID bfr,
	ULONG numOfBytesToWrite,
	PULONG numOfBytesWritten
);

class Memory
{
private:
	HANDLE pHandle;

	DWORD pID;
	uintptr_t base;
public:
	Memory() = default;
	~Memory() = default;

	DWORD getPID(const char* pName);
	uintptr_t getBase(const char* modName);

	bool attach(const char* pName);

	void readRaw(uintptr_t addr, void* bfr, uintptr_t sz);
	std::string readString(uintptr_t addr);

	template <typename T>
	T read(uintptr_t addr) {
		T bfr{};

		Mem_ReadVirtualMemory(this->pHandle, reinterpret_cast<void*>(addr), &bfr, sizeof(T), nullptr);

		return bfr;
	}

	template <typename T>
	bool write(uintptr_t addr, T val) {
		if (Mem_WriteVirtualMemory(this->pHandle, reinterpret_cast<void*>(addr), &val, sizeof(T), nullptr))
			return true;
		else
			return false;
	}

	DWORD getPID();
	void setPID(DWORD newPID);

	uintptr_t getBase();
	void setBaseAddr(uintptr_t newBase);
};

inline std::unique_ptr<Memory> mem = std::make_unique<Memory>();