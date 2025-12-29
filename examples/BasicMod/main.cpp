//==================================================================================================
//
//      SLEEPING DOGS MOD - Basic Template
//
//      This is a basic template for creating a Sleeping Dogs mod using the SDK.
//
//      Build Configuration:
//          - Platform: x64
//          - Configuration Type: Dynamic Library (.dll)
//          - C++ Language Standard: ISO C++17 or higher
//          - Include Directories: Add path to SDK root folder
//
//==================================================================================================

#include "sdhd_sdk.hh"

// Namespace for your mod
namespace MyMod
{
    // Game base address - automatically set by SDK
    using namespace SDK;

    // Example: Hook into game initialization
    SDK::Hook g_InitHook;

    void OnGameInitialized()
    {
        // This runs when the game initializes its systems
        // Perfect place to set up your mod

        // Call original function first
        g_InitHook.Original();

        // Your mod initialization code here
        // Example: Print to debug output
        OutputDebugStringA("[MyMod] Game initialized!\n");
    }

    // Example: Hook into the InGame state update
    SDK::Hook g_InGameUpdateHook;

    void OnInGameUpdate()
    {
        // This runs every frame while in-game
        // Be careful with performance here!

        // Call original function
        g_InGameUpdateHook.Original();

        // Your per-frame logic here
    }

    // Example: Hook into game state enter events
    SDK::Hook g_InGameEnterHook;

    void OnEnterInGame()
    {
        // This runs when entering the InGame state
        // Good for setting up gameplay modifications

        // Call original
        g_InGameEnterHook.Original();

        OutputDebugStringA("[MyMod] Entered game!\n");
    }

    // DLL Entry Point
    void Initialize()
    {
        // Set up your hooks here
        g_InitHook.I_InitGameSystems(OnGameInitialized);
        g_InGameUpdateHook.I_GameStateInGame(SDK::Hook::GameState_OnUpdate, OnInGameUpdate);
        g_InGameEnterHook.I_GameStateInGame(SDK::Hook::GameState_OnEnter, OnEnterInGame);
    }
}

// Windows DLL Entry Point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        // Disable thread notifications for better performance
        DisableThreadLibraryCalls(hModule);

        // Initialize SDK base address
        SDK::gBaseAddress = reinterpret_cast<uptr>(GetModuleHandleA(nullptr));

        // Initialize your mod
        MyMod::Initialize();
    }

    return TRUE;
}
