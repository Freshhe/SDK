//==================================================================================================
//
//      ADVANCED MOD EXAMPLE - Teleport System
//
//      This example demonstrates:
//      - Accessing player transform
//      - Saving/loading positions
//      - Complex input handling
//      - Vector math operations
//
//      Controls:
//          F5 - Save current position
//          F6 - Teleport to saved position
//          F7 - Teleport forward 10 meters
//
//==================================================================================================

#include "sdhd_sdk.hh"

namespace TeleportMod
{
    using namespace SDK;

    // Saved position
    qVector3 g_SavedPosition;
    bool g_HasSavedPosition = false;

    // Hooks
    SDK::Hook g_UpdateHook;

    // Input state tracking
    bool g_KeyStates[256] = { false };

    bool IsKeyPressed(int vKey)
    {
        bool currentState = (GetAsyncKeyState(vKey) & 0x8000) != 0;
        bool wasPressed = !g_KeyStates[vKey] && currentState;
        g_KeyStates[vKey] = currentState;
        return wasPressed;
    }

    SimObject* GetPlayer()
    {
        // Player SimObject pointer (this RVA may vary by game version)
        return SDK_VAR_GET(SimObject*, 0x23E95F0);
    }

    TransformNodeComponent* GetPlayerTransform()
    {
        auto player = GetPlayer();
        if (!player) return nullptr;

        return player->GetComponent<TransformNodeComponent>();
    }

    void SavePosition()
    {
        auto transform = GetPlayerTransform();
        if (!transform) {
            OutputDebugStringA("[TeleportMod] ERROR: Could not get player transform!\n");
            return;
        }

        g_SavedPosition = transform->mPosition;
        g_HasSavedPosition = true;

        char msg[256];
        sprintf_s(msg, "[TeleportMod] Position saved: (%.2f, %.2f, %.2f)\n",
                  g_SavedPosition.x, g_SavedPosition.y, g_SavedPosition.z);
        OutputDebugStringA(msg);
    }

    void TeleportToSaved()
    {
        if (!g_HasSavedPosition) {
            OutputDebugStringA("[TeleportMod] No saved position! Press F5 first.\n");
            return;
        }

        auto transform = GetPlayerTransform();
        if (!transform) {
            OutputDebugStringA("[TeleportMod] ERROR: Could not get player transform!\n");
            return;
        }

        transform->mPosition = g_SavedPosition;

        char msg[256];
        sprintf_s(msg, "[TeleportMod] Teleported to: (%.2f, %.2f, %.2f)\n",
                  g_SavedPosition.x, g_SavedPosition.y, g_SavedPosition.z);
        OutputDebugStringA(msg);
    }

    void TeleportForward(float distance)
    {
        auto transform = GetPlayerTransform();
        if (!transform) {
            OutputDebugStringA("[TeleportMod] ERROR: Could not get player transform!\n");
            return;
        }

        // Get forward vector from rotation matrix
        // The transform matrix is in column-major order
        qVector3 forward;
        forward.x = transform->mRotation.m[0][2];  // Third column, first row
        forward.y = transform->mRotation.m[1][2];  // Third column, second row
        forward.z = transform->mRotation.m[2][2];  // Third column, third row

        // Normalize the forward vector
        float length = sqrtf(forward.x * forward.x +
                            forward.y * forward.y +
                            forward.z * forward.z);

        if (length > 0.0f) {
            forward.x /= length;
            forward.y /= length;
            forward.z /= length;
        }

        // Calculate new position
        qVector3 newPosition;
        newPosition.x = transform->mPosition.x + forward.x * distance;
        newPosition.y = transform->mPosition.y + forward.y * distance;
        newPosition.z = transform->mPosition.z + forward.z * distance;

        transform->mPosition = newPosition;

        char msg[256];
        sprintf_s(msg, "[TeleportMod] Teleported forward %.1fm to: (%.2f, %.2f, %.2f)\n",
                  distance, newPosition.x, newPosition.y, newPosition.z);
        OutputDebugStringA(msg);
    }

    void HandleInput()
    {
        // F5 - Save position
        if (IsKeyPressed(VK_F5)) {
            SavePosition();
        }

        // F6 - Teleport to saved position
        if (IsKeyPressed(VK_F6)) {
            TeleportToSaved();
        }

        // F7 - Teleport forward 10 meters
        if (IsKeyPressed(VK_F7)) {
            TeleportForward(10.0f);
        }

        // F8 - Teleport forward 50 meters
        if (IsKeyPressed(VK_F8)) {
            TeleportForward(50.0f);
        }
    }

    void OnUpdate()
    {
        // Call original game update
        g_UpdateHook.Original();

        // Handle our input
        HandleInput();
    }

    void Initialize()
    {
        // Hook into the InGame update loop
        g_UpdateHook.I_GameStateInGame(SDK::Hook::GameState_OnUpdate, OnUpdate);

        OutputDebugStringA("=================================\n");
        OutputDebugStringA("[TeleportMod] Initialized!\n");
        OutputDebugStringA("[TeleportMod] Controls:\n");
        OutputDebugStringA("  F5: Save current position\n");
        OutputDebugStringA("  F6: Teleport to saved position\n");
        OutputDebugStringA("  F7: Teleport forward 10m\n");
        OutputDebugStringA("  F8: Teleport forward 50m\n");
        OutputDebugStringA("=================================\n");
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        SDK::gBaseAddress = reinterpret_cast<uptr>(GetModuleHandleA(nullptr));
        TeleportMod::Initialize();
    }

    return TRUE;
}
