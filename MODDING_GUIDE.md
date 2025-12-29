# Sleeping Dogs: Definitive Edition Modding Guide

## Table of Contents

1. [Introduction](#introduction)
2. [Types of Mods You Can Create](#types-of-mods-you-can-create)
3. [Development Setup](#development-setup)
4. [SDK Architecture](#sdk-architecture)
5. [Creating Your First Mod](#creating-your-first-mod)
6. [Common Modding Patterns](#common-modding-patterns)
7. [System Reference](#system-reference)
8. [Best Practices](#best-practices)
9. [Troubleshooting](#troubleshooting)

---

## Introduction

This SDK provides C++ access to Sleeping Dogs: Definitive Edition's internal systems. It's designed for creating gameplay modifications, visual enhancements, and custom features.

**What you need to know:**
- C++ programming (intermediate level)
- Basic understanding of game engines
- Familiarity with Visual Studio
- Understanding of x64 architecture

---

## Types of Mods You Can Create

### 🎮 Gameplay Mods
- **Character modifications**: Speed, strength, abilities
- **Combat tweaks**: Damage values, combo systems
- **Physics changes**: Gravity, vehicle handling, ragdolls
- **AI behavior**: NPC reactions, police aggression, traffic patterns
- **Difficulty adjustments**: Health, damage multipliers

### 🌆 World Mods
- **Time-of-day control**: Lock time, speed up/slow down day/night cycle
- **Weather modifications**: Force weather states, add new effects
- **Traffic control**: Density, vehicle types, pedestrian behavior
- **Spawn control**: Custom vehicle/character spawning

### 🎨 Visual Mods
- **Graphics enhancements**: Post-processing, depth of field, color grading
- **Shader modifications**: Custom visual effects
- **Camera control**: FOV, position, movement
- **Debug visualization**: Show collision, AI paths, navigation

### 🔊 Audio Mods
- **Music control**: Custom radio stations, soundtrack changes
- **Sound effects**: Volume control, custom sounds
- **Vehicle audio**: Engine sounds, environmental audio

### 🖥️ UI/HUD Mods
- **HUD customization**: Move, hide, or create UI elements
- **Custom menus**: Add configuration screens
- **Debug displays**: Show FPS, position, game state

### 🎯 Mission & Progression Mods
- **Objective manipulation**: Skip objectives, create custom missions
- **Progression changes**: Unlock all content, modify rewards
- **Save game manipulation**: Edit stats, money, experience

---

## Development Setup

### Prerequisites

1. **Visual Studio 2019 or newer**
   - Install "Desktop development with C++" workload
   - Ensure C++17 or C++20 support is enabled

2. **Windows SDK**
   - Required for DirectX 11 headers
   - Usually installed with Visual Studio

3. **Sleeping Dogs: Definitive Edition**
   - Steam or GOG version
   - Located at: `C:\Program Files (x86)\Steam\steamapps\common\Sleeping Dogs Definitive Edition\`

### SDK Setup

1. **Clone the SDK:**
   ```bash
   git clone https://github.com/SDmodding/SDK.git
   cd SDK
   ```

2. **Verify structure:**
   ```
   SDK/
   ├── sdhd_sdk.hh          (Main include file)
   ├── sdk/                  (Core SDK systems)
   ├── sim/                  (Simulation & components)
   ├── physics/              (Physics engine)
   ├── animation/            (Animation system)
   ├── ai/                   (AI systems)
   ├── render/               (Rendering)
   ├── ui/                   (User interface)
   └── examples/             (Example mods - START HERE!)
   ```

---

## SDK Architecture

### Core Concepts

#### 1. **RVA (Relative Virtual Address)**
The SDK uses RVAs to access game memory:

```cpp
#define SDK_RVA(x) SDK_BASE + x
```

- **SDK_BASE**: Game's base address in memory
- **x**: Offset from base address
- RVAs are constant across game sessions but may differ between game versions

#### 2. **Hooking System**
Modify game behavior by intercepting function calls:

```cpp
SDK::Hook myHook;

void MyCustomFunction()
{
    // Call original game function first
    myHook.Original();

    // Your custom code here
}

// Set up hook
myHook.I_GameStateInGame(SDK::Hook::GameState_OnUpdate, MyCustomFunction);
```

**Hook Types:**
- `I_CallRax`: Inline hook (replaces `mov rax, call rax` instructions)
- `I_FuncPtr`: Function pointer hook (replaces pointer in vtable/function table)

**Pre-configured Hooks:**
- `I_InitGameSystems`: Game initialization
- `I_GameStateInGame`: InGame state events
- `I_GameStateLoadGame`: LoadGame state events
- `I_SceneObjectProperties`: Scene object lifecycle

#### 3. **Component System**
Game objects use a component-based architecture:

```cpp
SimObject* player = SDK_VAR_GET(SimObject*, 0x23E95F0);
if (player) {
    auto health = player->GetComponent<HealthComponent>();
    auto transform = player->GetComponent<TransformNodeComponent>();
    auto anim = player->GetComponent<CharacterAnimationComponent>();
}
```

**Available Components** (35+):
- `HealthComponent`: Health, damage, death
- `TransformNodeComponent`: Position, rotation, scale
- `CharacterAnimationComponent`: Animation control
- `RigidBodyComponent`: Physics simulation
- `AIEntityComponent`: AI behavior
- `VehicleMoverComponent`: Vehicle physics
- `InventoryComponent`: Items and weapons
- See `/sim/components/` for full list

#### 4. **Game States**
The game uses a state machine:

```cpp
enum GameState {
    InGame,              // Active gameplay
    LoadGame,            // Loading save
    LoadNIS,             // Loading cutscene
    LoadChallenge,       // Loading challenge/mission
    RestoreCheckpoint,   // Loading checkpoint
    ChangeLocation       // Transitioning between areas
};
```

Hook into state transitions:
```cpp
// Called when entering InGame state
hook.I_GameStateInGame(SDK::Hook::GameState_OnEnter, OnEnterGame);

// Called every frame during InGame
hook.I_GameStateInGame(SDK::Hook::GameState_OnUpdate, OnUpdateGame);

// Called when exiting InGame state
hook.I_GameStateInGame(SDK::Hook::GameState_OnExit, OnExitGame);
```

---

## Creating Your First Mod

### Step-by-Step: Simple Health Display Mod

1. **Create Visual Studio Project**
   - New Project → C++ DLL
   - Platform: x64
   - Name: `HealthDisplayMod`

2. **Configure Include Path**
   - Right-click project → Properties
   - C/C++ → General → Additional Include Directories
   - Add path to SDK folder

3. **Write the Mod** (`main.cpp`):

```cpp
#include "sdhd_sdk.hh"
#include <stdio.h>

namespace HealthMod
{
    using namespace SDK;

    SDK::Hook g_UpdateHook;

    void DrawHealthInfo()
    {
        // Get player SimObject
        SimObject* player = SDK_VAR_GET(SimObject*, 0x23E95F0);
        if (!player) return;

        // Get health component
        auto health = player->GetComponent<HealthComponent>();
        if (!health) return;

        // Print to debug output
        char buffer[256];
        sprintf_s(buffer, "[Health] Current: %.1f / Max: %.1f\n",
                  health->mCurrentHealth, health->mMaxHealth);
        OutputDebugStringA(buffer);
    }

    void OnUpdate()
    {
        g_UpdateHook.Original();

        static int frameCount = 0;
        if (++frameCount % 60 == 0) { // Every 60 frames (~1 second)
            DrawHealthInfo();
        }
    }

    void Initialize()
    {
        g_UpdateHook.I_GameStateInGame(SDK::Hook::GameState_OnUpdate, OnUpdate);
        OutputDebugStringA("[HealthMod] Initialized!\n");
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        SDK::gBaseAddress = reinterpret_cast<uptr>(GetModuleHandleA(nullptr));
        HealthMod::Initialize();
    }
    return TRUE;
}
```

4. **Build the Mod**
   - Build → Build Solution (Ctrl+Shift+B)
   - Output: `HealthDisplayMod.dll` in `x64/Debug/` or `x64/Release/`

5. **Load the Mod**
   - Use a DLL injector (Ultimate ASI Loader, Cheat Engine, etc.)
   - Inject `HealthDisplayMod.dll` into `HKShip.exe`
   - Use DebugView to see output

---

## Common Modding Patterns

### Pattern 1: Accessing Game Variables

```cpp
// Get player position
SimObject* player = SDK_VAR_GET(SimObject*, 0x23E95F0);
auto transform = player->GetComponent<TransformNodeComponent>();
qVector3 position = transform->mPosition;

// Modify game speed (example RVA - not real)
float& timeScale = SDK_VAR_GET(float, 0x12345678);
timeScale = 2.0f;
```

### Pattern 2: Calling Game Functions

```cpp
// Define function signature and call it
auto PlaySound = SDK_CALL_FUNC(void, 0x12345678,
    const char*, float);  // RVA, params

PlaySound("sfx_explosion", 1.0f);
```

### Pattern 3: Virtual Function Calls

```cpp
// Call virtual function from object's vtable
SimObject* obj = /* ... */;
auto result = SDK_CALL_VFUNC(int, 5, obj,  // vtable index 5
    int, float)(obj, 10, 1.5f);  // Call with args
```

### Pattern 4: Custom Input Handling

```cpp
bool WasKeyJustPressed(int vKey)
{
    static bool keyStates[256] = {false};
    bool current = (GetAsyncKeyState(vKey) & 0x8000) != 0;
    bool pressed = current && !keyStates[vKey];
    keyStates[vKey] = current;
    return pressed;
}

void OnUpdate()
{
    if (WasKeyJustPressed(VK_F1)) {
        // Do something
    }
}
```

### Pattern 5: Safe Component Access

```cpp
template<typename T>
T* GetPlayerComponent()
{
    auto player = SDK_VAR_GET(SimObject*, 0x23E95F0);
    return player ? player->GetComponent<T>() : nullptr;
}

// Usage
if (auto health = GetPlayerComponent<HealthComponent>()) {
    health->mCurrentHealth = 100.0f;
}
```

---

## System Reference

### Simulation (`/sim/`)
- **SimObject**: Base game object
- **Sim**: Main simulation manager
- **LocalPlayer**: Player-specific functionality

### Physics (`/physics/`)
- **PhysicsEngine**: Main physics system
- **RigidBody**: Physics-enabled objects
- **Ragdoll**: Character ragdoll system
- **PhysicsVehicle**: Vehicle physics

### Animation (`/animation/`)
- **Creature**: Skeletal animation controller
- **Skeleton**: Bone hierarchy
- **AnimationDatabase**: Animation resources
- **BlendTree**: Animation blending

### AI (`/ai/`)
- **AIEntityComponent**: Core AI behavior
- **CopSystem**: Police AI
- **Stimulus**: AI perception system
- **ActionTree**: AI task sequences

### Rendering (`/render/`)
- **RenderWorld**: Main renderer
- **TimeOfDay**: Day/night cycle
- **DebugDraw**: Debug visualization

### UI (`/ui/`)
- **UIScreenManager**: Screen management
- **UIMessage**: Notifications
- **HUD widgets**: Health, weapons, radar, etc.

### Audio (`/audio/`)
- **AudioEntity**: 3D sound emitters
- **AudioEvent**: Sound playback

---

## Best Practices

### ✅ Do:
- **Always call `Original()`** on hooks before your code
- **Check pointers** before dereferencing
- **Use debug output** for logging
- **Test in release builds** before distribution
- **Comment your code** explaining what RVAs reference
- **Version-lock your mod** to specific game versions

### ❌ Don't:
- **Don't skip `Original()` calls** (will break game)
- **Don't perform heavy operations** in per-frame hooks
- **Don't hardcode RVAs** without documenting game version
- **Don't assume pointers are valid** (always check)
- **Don't modify memory** you don't understand

### Performance Tips:
```cpp
// ❌ Bad: Heavy work every frame
void OnUpdate() {
    for (int i = 0; i < 10000; i++) {
        // Expensive operation
    }
}

// ✅ Good: Throttle expensive work
void OnUpdate() {
    static int counter = 0;
    if (++counter % 60 == 0) {  // Once per second
        // Expensive operation
    }
}
```

---

## Troubleshooting

### Mod doesn't load
- **Check DLL architecture**: Must be x64
- **Verify SDK path**: Ensure includes are found
- **Check injector**: Try different injection method
- **View process modules**: Use Process Explorer to confirm DLL loaded

### Game crashes immediately
- **Missing `Original()` call**: Add it to all hooks
- **Invalid RVA**: RVA may be wrong for your game version
- **Stack corruption**: Check function signatures match
- **Null pointer**: Add null checks before dereferencing

### Mod loads but nothing happens
- **Add debug prints**: Verify code is executing
- **Check game state**: May need to wait for InGame state
- **Verify RVAs**: Use a disassembler to confirm addresses
- **Watch DebugView**: Ensure debug output is working

### Finding RVAs
1. **Use a disassembler** (Ghidra, IDA Pro, Binary Ninja)
2. **Search for strings** referenced by functions
3. **Analyze code patterns** from SDK headers
4. **Use community databases** if available
5. **Pattern scanning** for version-independent mods

### Debugging Tools
- **DebugView** (Sysinternals): View OutputDebugString messages
- **Process Explorer**: Check loaded DLLs
- **Visual Studio Debugger**: Attach to HKShip.exe
- **CheatEngine**: Memory scanning and debugging

---

## Example: Complete Vehicle Speed Mod

```cpp
#include "sdhd_sdk.hh"

namespace VehicleSpeedMod
{
    using namespace SDK;

    float g_SpeedMultiplier = 2.0f;
    SDK::Hook g_UpdateHook;

    void ModifyVehicleSpeed()
    {
        auto player = SDK_VAR_GET(SimObject*, 0x23E95F0);
        if (!player) return;

        auto occupant = player->GetComponent<CharacterOccupantComponent>();
        if (!occupant || !occupant->mVehicle) return;

        auto vehicleMover = occupant->mVehicle->GetComponent<VehicleMoverComponent>();
        if (!vehicleMover) return;

        // Modify max speed
        vehicleMover->mMaxSpeed *= g_SpeedMultiplier;
    }

    void OnUpdate()
    {
        g_UpdateHook.Original();

        if (GetAsyncKeyState(VK_F5) & 0x8000) {
            ModifyVehicleSpeed();
        }
    }

    void Initialize()
    {
        g_UpdateHook.I_GameStateInGame(SDK::Hook::GameState_OnUpdate, OnUpdate);
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        SDK::gBaseAddress = reinterpret_cast<uptr>(GetModuleHandleA(nullptr));
        VehicleSpeedMod::Initialize();
    }
    return TRUE;
}
```

---

## Next Steps

1. **Explore examples** in `/examples/` folder
2. **Study SDK headers** to understand available systems
3. **Join the community** for RVA databases and help
4. **Start small** with simple mods and build up
5. **Share your creations** with the community!

Happy modding! 🎮
