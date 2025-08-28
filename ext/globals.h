#pragma once
#include <Windows.h>
#include <cstdint>
#include <memory>

#include "mem/Memory.h"
#include "SDK/SDK.h"

struct RobloxPlayer
{
	uintptr_t addr = 0;
	int RigType = 0;

	std::string Name = "";
	float Health = 0.0f;
	float MaxHealth = 0.0f;

	Roblox Team = Roblox(0);
	Roblox Character = Roblox(0);
	Roblox Humanoid = Roblox(0);

	//R6
	Roblox Head = Roblox(0);
	Roblox HumanoidRootPart = Roblox(0);
	Roblox LeftArm = Roblox(0);
	Roblox LeftLeg = Roblox(0);
	Roblox RightArm = Roblox(0);
	Roblox RightLeg = Roblox(0);
	Roblox Torso = Roblox(0);

	//R15
	Roblox UpperTorso = Roblox(0);
	Roblox LowerTorso = Roblox(0);
	Roblox RightUpperArm = Roblox(0);
	Roblox RightLowerArm = Roblox(0);
	Roblox RightHand = Roblox(0);
	Roblox LeftUpperArm = Roblox(0);
	Roblox LeftLowerArm = Roblox(0);
	Roblox LeftHand = Roblox(0);
	Roblox RightUpperLeg = Roblox(0);
	Roblox RightLowerLeg = Roblox(0);
	Roblox RightFoot = Roblox(0);
	Roblox LeftUpperLeg = Roblox(0);
	Roblox LeftLowerLeg = Roblox(0);
	Roblox LeftFoot = Roblox(0);
};

namespace Globals {
	namespace Rblx {
		Roblox DataModel(0);
		uintptr_t VisualEngine;

		Roblox Workspace(0);
		Roblox Players(0);
		Roblox Camera(0);
		Roblox LocalPlayer(0);

		int lastPlaceID;
	}

	namespace Caches {
		std::vector<Roblox> CachedPlayers;
		std::vector<RobloxPlayer> CachedPlayerObjects;

	}

	inline std::string executablePath;
	inline std::string configsPath;
	inline std::vector<std::string> configsArray;
}