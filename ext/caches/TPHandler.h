#pragma once
#include "../globals.h"

#include <thread>
#include <vector>


inline void TPHandler()
{
	while (true)
	{
		auto fakeDataModel = mem->read<uintptr_t>(mem->getBase() + offsets::FakeDataModelPointer);
		auto dataModel = Roblox(mem->read<uintptr_t>(fakeDataModel + offsets::FakeDataModelToDataModel));
		auto placeId = mem->read<int>(dataModel.addr + offsets::PlaceId);
		uintptr_t visualEngine;

		if (!dataModel || dataModel.addr == 0 || dataModel.Name() == "LuaApp" || placeId != Globals::Rblx::lastPlaceID) // player left the game
		{

			while (dataModel.Name() != "Ugc")
			{
				fakeDataModel = mem->read<uintptr_t>(mem->getBase() + offsets::FakeDataModelPointer);
				dataModel = Roblox(mem->read<uintptr_t>(fakeDataModel + offsets::FakeDataModelToDataModel));
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}

			Globals::Rblx::DataModel = dataModel;

			visualEngine = mem->read<uintptr_t>(mem->getBase() + offsets::VisualEnginePointer);

			while (visualEngine == 0)
			{
				visualEngine = mem->read<uintptr_t>(mem->getBase() + offsets::VisualEnginePointer);
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}

			Globals::Rblx::VisualEngine = visualEngine;

			Globals::Rblx::Workspace = Globals::Rblx::DataModel.FindFirstChildWhichIsA("Workspace");
			Globals::Rblx::Players = Globals::Rblx::DataModel.FindFirstChildWhichIsA("Players");
			Globals::Rblx::Camera = Globals::Rblx::Workspace.FindFirstChildWhichIsA("Camera");

			Globals::Rblx::LocalPlayer = Roblox(mem->read<uintptr_t>(Globals::Rblx::Players.addr + offsets::LocalPlayer));

			Globals::Rblx::lastPlaceID = placeId;

			Globals::Caches::CachedPlayers.clear();
			Globals::Caches::CachedPlayerObjects.clear();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}
}

