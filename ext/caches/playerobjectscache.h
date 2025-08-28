#pragma once
#include <thread>
#include "../globals.h"

void CachePlayerObjects() {
	std::vector<RobloxPlayer> tempList;

	while (true)
	{
		tempList.clear();

		if (Globals::Caches::CachedPlayers.empty())
			continue;

		for (auto& player : Globals::Caches::CachedPlayers)
		{
			RobloxPlayer p;

			if (!player || player.addr == 0)
				continue;

			p.addr = player.addr;

			p.Name = player.Name();

			p.Team = player.Team();

			p.Character = player.Character();
			p.Humanoid = p.Character.FindFirstChildWhichIsA("Humanoid");

			p.Health = player.Health();
			p.MaxHealth = player.MaxHealth();

			p.RigType = p.Humanoid.RigType();

			p.Head = p.Character.FindFirstChild("Head");
			p.HumanoidRootPart = p.Character.FindFirstChild("HumanoidRootPart");

			switch (p.RigType)
			{
			case 0: // R6
				p.LeftArm = p.Character.FindFirstChild("Left Arm");
				p.LeftLeg = p.Character.FindFirstChild("Left Leg");

				p.RightArm = p.Character.FindFirstChild("Right Arm");
				p.RightLeg = p.Character.FindFirstChild("Right Leg");

				p.Torso = p.Character.FindFirstChild("Torso");

				break;
			case 1: // R15
				p.UpperTorso = p.Character.FindFirstChild("UpperTorso");
				p.LowerTorso = p.Character.FindFirstChild("LowerTorso");

				p.RightUpperArm = p.Character.FindFirstChild("RightUpperArm");
				p.RightLowerArm = p.Character.FindFirstChild("RightLowerArm");
				p.RightHand = p.Character.FindFirstChild("RightHand");

				p.LeftUpperArm = p.Character.FindFirstChild("LeftUpperArm");
				p.LeftLowerArm = p.Character.FindFirstChild("LeftLowerArm");
				p.LeftHand = p.Character.FindFirstChild("LeftHand");

				p.RightUpperLeg = p.Character.FindFirstChild("RightUpperLeg");
				p.RightLowerLeg = p.Character.FindFirstChild("RightLowerLeg");
				p.RightFoot = p.Character.FindFirstChild("RightFoot");

				p.LeftUpperLeg = p.Character.FindFirstChild("LeftUpperLeg");
				p.LeftLowerLeg = p.Character.FindFirstChild("LeftLowerLeg");
				p.LeftFoot = p.Character.FindFirstChild("LeftFoot");

				break;
			default:
				break;
			}

			tempList.push_back(p);
		}

		Globals::Caches::CachedPlayerObjects.clear();
		Globals::Caches::CachedPlayerObjects = tempList;

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}