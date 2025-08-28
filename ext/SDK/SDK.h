/*
*/#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

#include "../offsets/Offsets.h"
#include "../utils/Structures.h"
#include "../mem/Memory.h"

class Roblox {
public:
	uintptr_t addr;

	Roblox(uintptr_t address) {
		addr = address;
	}

	operator bool() {
		return addr != 0;
	}

	std::string Name() {
		return mem->readString(mem->read<uintptr_t>(addr + offsets::Name));
	}

	std::string Class() {
		return mem->readString(mem->read<uintptr_t>(mem->read<uintptr_t>(addr + offsets::ClassDescriptor) + offsets::ClassDescriptorToClassName));
	}

	bool IsA(std::string className) {
		if (Class() != className)
			return true;
		return false;
	}

	std::vector<Roblox> GetChildren() {
		uintptr_t childrenStart = mem->read<uintptr_t>(addr + offsets::Children);
		uintptr_t childrenEnd = mem->read<uintptr_t>(childrenStart + offsets::ChildrenEnd);

		std::vector<Roblox> returnVector;

		for (uintptr_t child = mem->read<uintptr_t>(childrenStart); child < childrenEnd; child += 0x10)
		{
			returnVector.emplace_back(Roblox(mem->read<uintptr_t>(child)));
		}

		return returnVector;
	}

	Roblox FindFirstChild(std::string name) {
		for (auto child : this->GetChildren()) {
			if (name == "")
				return child;

			if (child.Name() == name)
				return child;
		}

		return 0;
	}

	float Transparency() {
		return mem->read<float>(addr + offsets::Transparency);
	}

	Roblox FindFirstChildWhichIsA(std::string className = "") {
		for (auto& child : this->GetChildren())
		{
			if (child.Class() == className)
				return child;
		}
		return 0;
	}

	Vectors::Vector3 Position() {
		return mem->read<Vectors::Vector3>(mem->read<uintptr_t>(addr + offsets::Primitive) + offsets::Position);
	}

	Vectors::Vector3 Size() {
		return mem->read<Vectors::Vector3>(mem->read<uintptr_t>(addr + offsets::Primitive) + offsets::PartSize);
	}

	sCFrame CFrame() {
		if (Class() == "Camera")
		{
			auto rotation = mem->read<Matrixes::Matrix3x3>(addr + offsets::CameraRotation);
			auto position = mem->read<Vectors::Vector3>(addr + offsets::CameraPos);

			sCFrame newCFrame
			{
				rotation.r00, rotation.r01, rotation.r02,
				rotation.r10, rotation.r11, rotation.r12,
				rotation.r20, rotation.r21, rotation.r22,
				position.x, position.y, position.z
			};
			return newCFrame;
		}
		else
		{
			auto primitiveAddr = mem->read<uintptr_t>(addr + offsets::Primitive);

			auto rotation = mem->read<Matrixes::Matrix3x3>(primitiveAddr + offsets::Rotation);
			auto position = mem->read<Vectors::Vector3>(primitiveAddr + offsets::Position);

			sCFrame newCFrame
			{
				rotation.r00, rotation.r01, rotation.r02,
				rotation.r10, rotation.r11, rotation.r12,
				rotation.r20, rotation.r21, rotation.r22,
				position.x, position.y, position.z
			};
			return newCFrame;
		}
	}

	Roblox Character() {
		return Roblox(mem->read<uintptr_t>(addr + offsets::ModelInstance));
	}

	float Health() {
		auto character = Character();
		auto humanoid = character.FindFirstChildWhichIsA("Humanoid");

		return mem->read<float>(humanoid.addr + offsets::Health);
	}

	float MaxHealth() {
		auto character = Character();
		auto humanoid = character.FindFirstChildWhichIsA("Humanoid");

		return mem->read<float>(humanoid.addr + offsets::MaxHealth);
	}

	Roblox Team() {
		return Roblox(mem->read<uintptr_t>(addr + offsets::Team));
	}

	int RigType() {
		return mem->read<int>(addr + offsets::RigType);
	}

	void SetWalkspeed(float val) {
		val = std::round(val);
		mem->write(addr + offsets::WalkSpeedCheck, val);
		mem->write(addr + offsets::WalkSpeed, val);
	}

	void SetJumpPower(float val) {
		val = std::round(val);
		mem->write(addr + offsets::JumpPower, val);
	}

	float GetWalkSpeed() {
		return std::round(mem->read<float>(addr + offsets::WalkSpeed));
	}

	float GetJumpPower() {
		return std::round(mem->read<float>(addr + offsets::JumpPower));
	}

	float GetFOV() {
		auto radiantsFOV = mem->read<float>(addr + offsets::FOV);

		auto degreesFOV = radiantsFOV * 180 / 3.1415926535;

		return std::round(degreesFOV);

	}

	void SetFOV(float val) {
		val = std::round(val);

		auto radiantsValue = val * 3.1415926535 / 180;

		mem->write<float>(addr + offsets::FOV, radiantsValue);
	}
};