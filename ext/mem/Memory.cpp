#include "Memory.h"

DWORD Memory::getPID(const char* pName) {
	auto ss = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 pe{ sizeof(pe) };

	if (Process32First(ss, &pe)) {
		while (Process32Next(ss, &pe)) {
			if (strcmp(pe.szExeFile, pName) == 0) {
				CloseHandle(ss);
				return pe.th32ProcessID;
			}
		}
	}

	CloseHandle(ss);
	return NULL;
}

uintptr_t Memory::getBase(const char* modName) {
	auto pID = this->pID;
	auto ss = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID);

	MODULEENTRY32 me{ sizeof(me) };

	if (Module32First(ss, &me)) {
		do {
			if (strcmp(me.szModule, modName) == 0) {
				CloseHandle(ss);
				return reinterpret_cast<uintptr_t>(me.modBaseAddr);
			}
		} while (Module32Next(ss, &me));
	}

	CloseHandle(ss);
	return NULL;
}

bool Memory::attach(const char* pName) {
	auto pid = this->getPID(pName);

	this->pHandle = OpenProcess(PROCESS_ALL_ACCESS, false, pid);

	if (this->pHandle == INVALID_HANDLE_VALUE)
		return false;

	this->pID = pid;
	this->base = getBase(pName);

	return true;
}

void Memory::readRaw(uintptr_t addr, void* bfr, uintptr_t sz) {
	Mem_ReadVirtualMemory(this->pHandle, reinterpret_cast<void*>(addr), &bfr, sz, nullptr);
}

std::string Memory::readString(uintptr_t addr) {
	std::string result;
	char character;
	int offset = 0;

	int32_t StrLength = read<int32_t>(addr + 0x18);

	if (StrLength >= 16) {
		addr = read<uintptr_t>(addr);
	}

	while ((character = read<char>(addr + offset)) != 0)
	{
		result.push_back(character);
		offset += sizeof(character);
	}

	return result;
}

DWORD Memory::getPID() {
	return this->pID;
}

void Memory::setPID(DWORD newPid) {
	this->pID = newPid;
}

uintptr_t Memory::getBase() {
	return this->base;
}

void Memory::setBaseAddr(uintptr_t newBase) {
	this->base = newBase;
}