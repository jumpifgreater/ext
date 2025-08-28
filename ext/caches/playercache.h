#pragma once
#include "../globals.h"

#include <thread>
#include <vector>

void CachePlayers() {
	std::vector<Roblox> tempList;

	while (true) {
		tempList.clear();

		auto children = Globals::Rblx::Players.GetChildren();
		if (children.empty())
			continue;


		for (auto player : Globals::Rblx::Players.GetChildren())
			tempList.push_back(player);

		Globals::Caches::CachedPlayers.clear();
		Globals::Caches::CachedPlayers = tempList;

		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
	}
}