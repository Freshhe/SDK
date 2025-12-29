//==================================================================================================
//
//      TRAFFIC & PEDESTRIAN DENSITY MOD
//
//      This mod allows you to increase traffic and pedestrian density for more realism.
//
//      Features:
//          - Control traffic vehicle count (0-200 cars)
//          - Control pedestrian count (0-200 peds)
//          - Density presets (Low, Normal, High, Ultra, Extreme)
//          - Real-time adjustments with hotkeys
//          - Save/restore settings
//
//      Controls:
//          F1 - Decrease Traffic
//          F2 - Increase Traffic
//          F3 - Decrease Pedestrians
//          F4 - Increase Pedestrians
//
//          F5 - Low Density Preset
//          F6 - Normal Density Preset (Game Default)
//          F7 - High Density Preset
//          F8 - Ultra Density Preset
//          F9 - Extreme Density Preset (Performance Warning!)
//
//          F10 - Toggle Info Display
//
//==================================================================================================

#include "sdhd_sdk.hh"
#include <stdio.h>

namespace TrafficPedDensityMod
{
    using namespace SDK;
    using namespace UFG;

    //--------------------------------------------------
    //  Configuration
    //--------------------------------------------------

    struct DensityPreset
    {
        const char* name;
        u32 trafficCars;
        u32 parkedCars;
        u32 pedestrians;
        f32 trafficDensityScale;
        f32 pedDensityScale;
    };

    const DensityPreset PRESETS[] = {
        { "Low",      30,  30,  20, 0.5f,  0.5f  },   // F5 - Low traffic/peds
        { "Normal",   50,  50,  42, 1.0f,  1.0f  },   // F6 - Game default
        { "High",     80,  70,  80, 1.5f,  1.5f  },   // F7 - Busy city
        { "Ultra",    120, 100, 120, 2.0f,  2.0f  },   // F8 - Very crowded
        { "Extreme",  200, 150, 200, 3.0f,  3.0f  }    // F9 - Maximum (WARNING: May lag!)
    };

    // Current settings
    u32 g_MaxTrafficCars = 50;
    u32 g_MaxParkedCars = 50;
    u32 g_MaxPedestrians = 42;
    f32 g_TrafficDensityScale = 1.0f;
    f32 g_PedDensityScale = 1.0f;

    // UI
    bool g_ShowInfo = true;
    int g_MessageTimer = 0;
    char g_CurrentMessage[256] = "";

    // Hooks
    SDK::Hook g_UpdateHook;

    // Input state tracking
    bool g_KeyStates[256] = { false };

    //--------------------------------------------------
    //  Input Handling
    //--------------------------------------------------

    bool IsKeyPressed(int vKey)
    {
        bool currentState = (GetAsyncKeyState(vKey) & 0x8000) != 0;
        bool wasPressed = !g_KeyStates[vKey] && currentState;
        g_KeyStates[vKey] = currentState;
        return wasPressed;
    }

    //--------------------------------------------------
    //  Density Control Functions
    //--------------------------------------------------

    void ApplyTrafficSettings()
    {
        auto vehicleMgr = WheeledVehicleManager::Instance();
        if (!vehicleMgr) return;

        WheeledVehicleManager::SetMaxTrafficCars(g_MaxTrafficCars);
        WheeledVehicleManager::SetMaxParkedCars(g_MaxParkedCars);
        WheeledVehicleManager::SetTrafficDensityScale(g_TrafficDensityScale);
    }

    void ApplyPedestrianSettings()
    {
        auto pedMgr = PedSpawnManager::Instance();
        if (!pedMgr) return;

        pedMgr->SetTargetPedCount(g_MaxPedestrians);
        pedMgr->AdjustPedDensity(g_PedDensityScale);
    }

    void SetMessage(const char* msg)
    {
        strcpy_s(g_CurrentMessage, msg);
        g_MessageTimer = 180; // Show for 3 seconds at 60fps
        OutputDebugStringA(msg);
    }

    void AdjustTraffic(int delta)
    {
        int newValue = (int)g_MaxTrafficCars + delta;
        if (newValue < 0) newValue = 0;
        if (newValue > 200) newValue = 200;

        g_MaxTrafficCars = (u32)newValue;

        // Also adjust density scale proportionally
        g_TrafficDensityScale = g_MaxTrafficCars / 50.0f;

        ApplyTrafficSettings();

        char msg[256];
        sprintf_s(msg, "[Traffic Mod] Traffic Vehicles: %d (Density: %.1fx)\n",
                  g_MaxTrafficCars, g_TrafficDensityScale);
        SetMessage(msg);
    }

    void AdjustPedestrians(int delta)
    {
        int newValue = (int)g_MaxPedestrians + delta;
        if (newValue < 0) newValue = 0;
        if (newValue > 200) newValue = 200;

        g_MaxPedestrians = (u32)newValue;

        // Also adjust density scale proportionally
        g_PedDensityScale = g_MaxPedestrians / 42.0f;

        ApplyPedestrianSettings();

        char msg[256];
        sprintf_s(msg, "[Traffic Mod] Pedestrians: %d (Density: %.1fx)\n",
                  g_MaxPedestrians, g_PedDensityScale);
        SetMessage(msg);
    }

    void ApplyPreset(int presetIndex)
    {
        if (presetIndex < 0 || presetIndex >= ARRAYSIZE(PRESETS)) return;

        const DensityPreset& preset = PRESETS[presetIndex];

        g_MaxTrafficCars = preset.trafficCars;
        g_MaxParkedCars = preset.parkedCars;
        g_MaxPedestrians = preset.pedestrians;
        g_TrafficDensityScale = preset.trafficDensityScale;
        g_PedDensityScale = preset.pedDensityScale;

        ApplyTrafficSettings();
        ApplyPedestrianSettings();

        char msg[256];
        sprintf_s(msg, "[Traffic Mod] Preset Applied: %s\n  Traffic: %d | Parked: %d | Peds: %d\n",
                  preset.name, g_MaxTrafficCars, g_MaxParkedCars, g_MaxPedestrians);
        SetMessage(msg);
    }

    //--------------------------------------------------
    //  Input Processing
    //--------------------------------------------------

    void ProcessInput()
    {
        // F1/F2 - Adjust Traffic
        if (IsKeyPressed(VK_F1)) {
            AdjustTraffic(-10);
        }
        if (IsKeyPressed(VK_F2)) {
            AdjustTraffic(+10);
        }

        // F3/F4 - Adjust Pedestrians
        if (IsKeyPressed(VK_F3)) {
            AdjustPedestrians(-10);
        }
        if (IsKeyPressed(VK_F4)) {
            AdjustPedestrians(+10);
        }

        // F5-F9 - Presets
        if (IsKeyPressed(VK_F5)) {
            ApplyPreset(0); // Low
        }
        if (IsKeyPressed(VK_F6)) {
            ApplyPreset(1); // Normal
        }
        if (IsKeyPressed(VK_F7)) {
            ApplyPreset(2); // High
        }
        if (IsKeyPressed(VK_F8)) {
            ApplyPreset(3); // Ultra
        }
        if (IsKeyPressed(VK_F9)) {
            ApplyPreset(4); // Extreme
        }

        // F10 - Toggle Info Display
        if (IsKeyPressed(VK_F10)) {
            g_ShowInfo = !g_ShowInfo;

            char msg[256];
            sprintf_s(msg, "[Traffic Mod] Info Display: %s\n", g_ShowInfo ? "ON" : "OFF");
            SetMessage(msg);
        }
    }

    //--------------------------------------------------
    //  Update Loop
    //--------------------------------------------------

    void OnUpdate()
    {
        // Call original game update
        g_UpdateHook.Original();

        // Process input
        ProcessInput();

        // Update message timer
        if (g_MessageTimer > 0) {
            g_MessageTimer--;
        }

        // Display current stats periodically if enabled
        if (g_ShowInfo) {
            static int infoCounter = 0;
            if (++infoCounter % 300 == 0) { // Every 5 seconds
                auto vehicleMgr = WheeledVehicleManager::Instance();
                auto pedMgr = PedSpawnManager::Instance();

                if (vehicleMgr && pedMgr) {
                    char msg[512];
                    sprintf_s(msg,
                        "[Traffic Mod] Active - Traffic: %d/%d | Parked: %d/%d | Peds: %d/%d\n",
                        vehicleMgr->mNumTrafficCars, g_MaxTrafficCars,
                        vehicleMgr->mNumParkedCars, g_MaxParkedCars,
                        pedMgr->mNumAmbientCurrentlyActive, g_MaxPedestrians);
                    OutputDebugStringA(msg);
                }
            }
        }
    }

    //--------------------------------------------------
    //  Initialization
    //--------------------------------------------------

    void Initialize()
    {
        // Hook into the InGame update loop
        g_UpdateHook.I_GameStateInGame(SDK::Hook::GameState_OnUpdate, OnUpdate);

        // Apply initial settings (start with Normal preset)
        ApplyPreset(1);

        OutputDebugStringA("=====================================\n");
        OutputDebugStringA("[Traffic Mod] Initialized!\n");
        OutputDebugStringA("\n");
        OutputDebugStringA("Controls:\n");
        OutputDebugStringA("  F1/F2 - Decrease/Increase Traffic (-/+10)\n");
        OutputDebugStringA("  F3/F4 - Decrease/Increase Pedestrians (-/+10)\n");
        OutputDebugStringA("\n");
        OutputDebugStringA("Presets:\n");
        OutputDebugStringA("  F5 - Low Density\n");
        OutputDebugStringA("  F6 - Normal Density (Default)\n");
        OutputDebugStringA("  F7 - High Density\n");
        OutputDebugStringA("  F8 - Ultra Density\n");
        OutputDebugStringA("  F9 - Extreme Density (WARNING: May cause lag!)\n");
        OutputDebugStringA("\n");
        OutputDebugStringA("  F10 - Toggle Info Display\n");
        OutputDebugStringA("\n");
        OutputDebugStringA("Current Settings: Normal Preset\n");
        OutputDebugStringA("=====================================\n");
    }
}

//--------------------------------------------------
//  DLL Entry Point
//--------------------------------------------------

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        SDK::gBaseAddress = reinterpret_cast<uptr>(GetModuleHandleA(nullptr));
        TrafficPedDensityMod::Initialize();
    }

    return TRUE;
}
