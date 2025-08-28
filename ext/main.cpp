#include "mem/Memory.h"
#include <iostream>
#include "utils/Utils.h"
#include "utils/console.h"
#include "offsets/Offsets.h"
#include "globals.h"
#include <thread>
#include "caches/playercache.h"
#include "caches/playerobjectscache.h"
#include "renderer/renderer.h"
#include "caches/TPHandler.h"
#include "impl/misc.h"

std::string GetExecutableDir()
{
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);
    std::filesystem::path exePath(path);
    return exePath.parent_path().string();
}

int main()
{
    if (!IsGameRunning("Roblox"))
    {
        log("Roblox not found!", 2);
        log("Waiting for Roblox...", 0);
        while (!IsGameRunning("Roblox"))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    }

    log("Roblox found!", 1);

    log("Attaching to Roblox...", 0);
    if (!mem->attach("RobloxPlayerBeta.exe"))
    {
        log("Failed to attach to Roblox!", 2);
        log("Press any key to exit...", 0);
        std::cin.get();
        return -1;
    }

    log("Succesfully attached!", 1);

    if (mem->getPID("RobloxPlayerBeta.exe") == 0)
    {
        log("Failed to get Roblox's PID!", 2);
        log("Press any key to exit...", 0);
        std::cin.get();
        return -1;
    }

    log(std::string("Roblox PID -> " + std::to_string(mem->getPID())), 1);
    log(std::string("Roblox Base Address -> 0x" + toHexString(std::to_string(mem->getBase()), false, true)), 1);

    Globals::executablePath = GetExecutableDir();

    std::string fontsFolderPath = Globals::executablePath + "\\fonts";

    struct stat buffer;
    if (stat(fontsFolderPath.c_str(), &buffer) != 0)
    {
        log("Failed to find fonts folder!", 2);
        log("Press any key to exit...", 0);
        std::cin.get();
        return -1;
    }

    Globals::configsPath = Globals::executablePath + "\\configs";

    if (stat(Globals::configsPath.c_str(), &buffer) != 0)
    {
        std::filesystem::create_directory(Globals::configsPath);
    }

    auto fakeDataModel = mem->read<uintptr_t>(mem->getBase() + offsets::FakeDataModelPointer);
    auto dataModel = Roblox(mem->read<uintptr_t>(fakeDataModel + offsets::FakeDataModelToDataModel));

    while (dataModel.Name() != "Ugc")
    {
        fakeDataModel = mem->read<uintptr_t>(mem->getBase() + offsets::FakeDataModelPointer);
        dataModel = Roblox(mem->read<uintptr_t>(fakeDataModel + offsets::FakeDataModelToDataModel));
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    Globals::Rblx::DataModel = dataModel;

    auto visualEngine = mem->read<uintptr_t>(mem->getBase() + offsets::VisualEnginePointer);

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

    Globals::Rblx::lastPlaceID = mem->read<int>(Globals::Rblx::DataModel.addr + offsets::PlaceId);;

    log(std::string("DataModel -> 0x" + toHexString(std::to_string(Globals::Rblx::DataModel.addr), false, true)), 1);
    log(std::string("VisualEngine -> 0x" + toHexString(std::to_string(Globals::Rblx::VisualEngine), false, true)), 1);

    log(std::string("Workspace -> 0x" + toHexString(std::to_string(Globals::Rblx::Workspace.addr), false, true)), 1);
    log(std::string("Players -> 0x" + toHexString(std::to_string(Globals::Rblx::Players.addr), false, true)), 1);
    log(std::string("Camera -> 0x" + toHexString(std::to_string(Globals::Rblx::Camera.addr), false, true)), 1);

    log(std::string("Logged in as " + Globals::Rblx::LocalPlayer.Name()), 1);

    std::thread(ShowImgui).detach();
    std::thread(CachePlayers).detach();
    std::thread(CachePlayerObjects).detach();
    std::thread(TPHandler).detach();
    std::thread(MiscLoop).detach();

    std::cin.get();

    return 1;
}