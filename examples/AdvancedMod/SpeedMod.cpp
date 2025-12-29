//==================================================================================================
//
//      ADVANCED MOD EXAMPLE - Game Speed Controller
//
//      This example demonstrates:
//      - Custom input handling
//      - Modifying game variables
//      - UI integration
//      - State management
//
//==================================================================================================

#include "sdhd_sdk.hh"

namespace SpeedMod
{
    using namespace SDK;

    // State
    float g_GameSpeed = 1.0f;
    float g_MinSpeed = 0.1f;
    float g_MaxSpeed = 5.0f;
    float g_SpeedStep = 0.1f;

    // Hooks
    SDK::Hook g_UpdateHook;

    // Input handling
    bool g_KeyStates[256] = { false };

    bool IsKeyPressed(int vKey)
    {
        bool currentState = (GetAsyncKeyState(vKey) & 0x8000) != 0;
        bool wasPressed = !g_KeyStates[vKey] && currentState;
        g_KeyStates[vKey] = currentState;
        return wasPressed;
    }

    void UpdateGameSpeed()
    {
        // Increase speed: Numpad +
        if (IsKeyPressed(VK_ADD)) {
            g_GameSpeed += g_SpeedStep;
            if (g_GameSpeed > g_MaxSpeed) {
                g_GameSpeed = g_MaxSpeed;
            }

            char msg[128];
            sprintf_s(msg, "[SpeedMod] Game speed: %.1fx\n", g_GameSpeed);
            OutputDebugStringA(msg);
        }

        // Decrease speed: Numpad -
        if (IsKeyPressed(VK_SUBTRACT)) {
            g_GameSpeed -= g_SpeedStep;
            if (g_GameSpeed < g_MinSpeed) {
                g_GameSpeed = g_MinSpeed;
            }

            char msg[128];
            sprintf_s(msg, "[SpeedMod] Game speed: %.1fx\n", g_GameSpeed);
            OutputDebugStringA(msg);
        }

        // Reset speed: Numpad *
        if (IsKeyPressed(VK_MULTIPLY)) {
            g_GameSpeed = 1.0f;
            OutputDebugStringA("[SpeedMod] Game speed reset to 1.0x\n");
        }
    }

    void ApplyGameSpeed()
    {
        // This is a simplified example
        // In reality, you'd need to find the correct game time scale variable
        // and modify it. The RVA would be found through reverse engineering.

        // Example (fictional RVA):
        // float& timeScale = SDK_VAR_GET(float, 0x23E8A40);
        // timeScale = g_GameSpeed;
    }

    void OnUpdate()
    {
        // Call original first
        g_UpdateHook.Original();

        // Handle input
        UpdateGameSpeed();

        // Apply speed modification
        ApplyGameSpeed();
    }

    void Initialize()
    {
        g_UpdateHook.I_GameStateInGame(SDK::Hook::GameState_OnUpdate, OnUpdate);

        OutputDebugStringA("=================================\n");
        OutputDebugStringA("[SpeedMod] Initialized!\n");
        OutputDebugStringA("[SpeedMod] Controls:\n");
        OutputDebugStringA("  Numpad + : Increase speed\n");
        OutputDebugStringA("  Numpad - : Decrease speed\n");
        OutputDebugStringA("  Numpad * : Reset speed\n");
        OutputDebugStringA("=================================\n");
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        SDK::gBaseAddress = reinterpret_cast<uptr>(GetModuleHandleA(nullptr));
        SpeedMod::Initialize();
    }
    return TRUE;
}
