# Sleeping Dogs Basic Mod Template

This is a starter template for creating Sleeping Dogs mods.

## Building Your Mod

### Visual Studio Setup

1. **Create a new project**:
   - File → New → Project
   - Select "Dynamic-Link Library (DLL)" (C++)
   - Name your project

2. **Configure project properties** (Alt+F7):

   **General**:
   - Platform: `x64`
   - Configuration Type: `Dynamic Library (.dll)`
   - C++ Language Standard: `ISO C++17 Standard` (or C++20)

   **C/C++ → General**:
   - Additional Include Directories: Add path to SDK folder
     - Example: `$(SolutionDir)..\..\SDK\` or absolute path

   **Linker → Input**:
   - Additional Dependencies (if needed):
     ```
     d3d11.lib
     dinput8.lib
     XInput.lib
     ```

3. **Copy this `main.cpp` into your project**

4. **Build** (Ctrl+B) - you should get a `.dll` file

## Using Your Mod

1. **Build your mod** to create the DLL
2. **Inject the DLL** into `HKShip.exe` using a DLL injector:
   - Ultimate ASI Loader
   - Cheat Engine
   - Custom injector
3. **Launch Sleeping Dogs** and your mod will load!

## Common Modifications

### Example 1: Infinite Health

```cpp
SDK::Hook g_HealthHook;

float* GetPlayerHealth()
{
    auto player = SDK_VAR_GET(SimObject*, 0x23E95F0); // Player SimObject
    if (!player) return nullptr;

    auto healthComp = player->GetComponent<HealthComponent>();
    if (!healthComp) return nullptr;

    return &healthComp->mCurrentHealth;
}

void OnInGameUpdate()
{
    g_HealthHook.Original();

    float* health = GetPlayerHealth();
    if (health) {
        *health = 100.0f; // Max health
    }
}
```

### Example 2: Custom Input Handler

```cpp
#include "input/input.hh"

void OnInGameUpdate()
{
    g_InGameUpdateHook.Original();

    // Check if F1 key is pressed
    if (GetAsyncKeyState(VK_F1) & 0x8000) {
        OutputDebugStringA("[MyMod] F1 pressed!\n");
        // Do something cool
    }
}
```

### Example 3: Spawn a Vehicle

```cpp
#include "vehicles/vehicleutil.hh"
#include "sim/sim.hh"

void SpawnVehicle()
{
    // Get player position
    auto player = SDK_VAR_GET(SimObject*, 0x23E95F0);
    if (!player) return;

    auto transform = player->GetComponent<TransformNodeComponent>();
    if (!transform) return;

    qVector3 spawnPos = transform->mPosition;
    spawnPos.y += 2.0f; // Spawn above player

    // Spawn vehicle using game functions
    // Note: You'll need to find the correct RVA for vehicle spawn function
}
```

## Advanced Topics

### Accessing Game Variables

```cpp
// Get game variable by RVA
float& gameSpeed = SDK_VAR_GET(float, 0x12345678);
gameSpeed = 2.0f; // Double game speed
```

### Calling Game Functions

```cpp
// Call a game function
auto SpawnEffect = SDK_CALL_FUNC(void*, 0x12345678,
    qVector3&, qSymbol);

qVector3 pos(0, 0, 0);
qSymbol effectName("fx_explosion");
SpawnEffect(pos, effectName);
```

### Working with Components

```cpp
// Get a component from a SimObject
SimObject* obj = /* ... */;
auto animComp = obj->GetComponent<CharacterAnimationComponent>();

if (animComp) {
    // Modify animation
}
```

## Debugging Tips

1. **Use OutputDebugStringA()** to log messages
2. **View logs** with DebugView (from Sysinternals)
3. **Attach debugger** to HKShip.exe in Visual Studio
4. **Set breakpoints** in your mod code

## Resources

- **SDK Headers**: Browse `/SDK/` folder for available classes and functions
- **Game Functions**: Use a disassembler (Ghidra, IDA) to find RVAs
- **Community**: Check modding forums for help and RVA databases

## Troubleshooting

**Mod doesn't load:**
- Check DLL is x64 architecture
- Verify SDK path in include directories
- Check injector is working

**Game crashes:**
- Ensure you're calling Original() on hooks
- Check RVAs are correct for your game version
- Validate pointers before dereferencing

**Nothing happens:**
- Add debug prints to verify code runs
- Check hooks are set up in Initialize()
- Verify game state is correct
