# Build Instructions - Traffic & Pedestrian Density Mod

This guide will walk you through building the Traffic & Pedestrian Density Mod from source.

## Prerequisites

Before you begin, ensure you have:

✅ **Visual Studio 2019 or newer** (Community Edition is free)
- Download from: https://visualstudio.microsoft.com/
- Install the "Desktop development with C++" workload

✅ **Windows SDK** (usually installed with Visual Studio)

✅ **Sleeping Dogs: Definitive Edition SDK**
- This should already be available at: `/home/user/SDK/`

---

## Quick Build (5 Minutes)

### Step 1: Create Visual Studio Project

1. Open Visual Studio
2. Click **File → New → Project**
3. Select **C++** → **Windows Desktop** → **Dynamic-Link Library (DLL)**
4. Name: `TrafficPedDensityMod`
5. Location: Choose where you want to save it
6. Click **Create**

### Step 2: Configure Project Settings

1. In Solution Explorer, **right-click** on your project → **Properties**
2. Change **Configuration** dropdown to **All Configurations**
3. Change **Platform** dropdown to **x64**

#### General Settings

Navigate to: **Configuration Properties → General**

| Setting | Value |
|---------|-------|
| Platform Toolset | v142 (or latest) |
| Configuration Type | Dynamic Library (.dll) |
| C++ Language Standard | ISO C++17 Standard (/std:c++17) |
| Windows SDK Version | 10.0 (latest) |

#### C/C++ Settings

Navigate to: **Configuration Properties → C/C++ → General**

| Setting | Value |
|---------|-------|
| Additional Include Directories | `C:\Path\To\SDK\` (or wherever your SDK is located) |

**Important:** Replace `C:\Path\To\SDK\` with the actual path to your SDK folder.
For example: `D:\Projects\SleepingDogs\SDK\`

**Tip:** You can use the browse button (...)  to navigate to the folder.

#### C/C++ → Precompiled Headers

Navigate to: **Configuration Properties → C/C++ → Precompiled Headers**

| Setting | Value |
|---------|-------|
| Precompiled Header | Not Using Precompiled Headers |

#### Linker → Input

Navigate to: **Configuration Properties → Linker → Input**

| Setting | Value |
|---------|-------|
| Additional Dependencies | Add these to existing:<br>`d3d11.lib`<br>`dinput8.lib`<br>`XInput.lib` |

Click **OK** to save all settings.

### Step 3: Add Source Code

1. **Delete** the auto-generated files:
   - Right-click `framework.h` → Delete
   - Right-click `pch.h` → Delete
   - Right-click `pch.cpp` → Delete
   - Right-click `dllmain.cpp` → Delete

2. **Add** the mod source:
   - Right-click on **Source Files** folder → **Add → Existing Item...**
   - Navigate to `SDK/examples/TrafficPedDensityMod/`
   - Select `TrafficPedDensityMod.cpp`
   - Click **Add**

### Step 4: Build the Mod

1. At the top of Visual Studio, change **Debug** to **Release** (for better performance)
2. Ensure **x64** is selected (NOT x86)
3. Click **Build → Build Solution** (or press `Ctrl+Shift+B`)

**Build Output:**
```
Build started...
1>------ Build started: Project: TrafficPedDensityMod, Configuration: Release x64 ------
1>TrafficPedDensityMod.cpp
1>   Creating library ...
1>TrafficPedDensityMod.vcxproj -> ...\x64\Release\TrafficPedDensityMod.dll
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========
```

### Step 5: Locate Your DLL

Your built DLL will be at:
```
YourProjectFolder\x64\Release\TrafficPedDensityMod.dll
```

**Success!** You now have a working mod DLL.

---

## Loading the Mod Into the Game

### Method 1: Ultimate ASI Loader (Recommended)

1. **Download Ultimate ASI Loader:**
   - Get it from: https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases
   - Download `Ultimate-ASI-Loader.zip`

2. **Install to game directory:**
   - Go to your Sleeping Dogs install folder:
     ```
     C:\Program Files (x86)\Steam\steamapps\common\Sleeping Dogs Definitive Edition\
     ```
   - Extract `dinput8.dll` from the ZIP to this folder
   - Create a folder called `scripts` in the same location

3. **Copy your mod:**
   - Copy `TrafficPedDensityMod.dll` to the `scripts` folder
   - Rename it to `TrafficPedDensityMod.asi`

4. **Launch the game** - your mod will load automatically!

### Method 2: Manual DLL Injection

1. **Download Cheat Engine:**
   - Get it from: https://www.cheatengine.org/downloads.php

2. **Launch Sleeping Dogs** and wait until you're in-game

3. **Open Cheat Engine:**
   - Click the computer icon (top-left)
   - Select `HKShip.exe` from the process list
   - Click **Open**

4. **Inject DLL:**
   - Click **Memory View** (top menu)
   - Click **Tools → Inject DLL**
   - Browse to your `TrafficPedDensityMod.dll`
   - Click **Open**

5. **Verify:**
   - Download [DebugView](https://docs.microsoft.com/en-us/sysinternals/downloads/debugview)
   - Run DebugView
   - You should see initialization messages from the mod

---

## Verification

Once the mod is loaded, you should see:

### In DebugView:
```
=====================================
[Traffic Mod] Initialized!

Controls:
  F1/F2 - Decrease/Increase Traffic (-/+10)
  F3/F4 - Decrease/Increase Pedestrians (-/+10)

Presets:
  F5 - Low Density
  F6 - Normal Density (Default)
  F7 - High Density
  F8 - Ultra Density
  F9 - Extreme Density (WARNING: May cause lag!)

  F10 - Toggle Info Display

Current Settings: Normal Preset
=====================================
```

### In-Game:
- Press **F7** (High Density preset)
- Wait 10-15 seconds
- You should see more vehicles and pedestrians spawning
- Press **F10** to see live stats in DebugView

---

## Troubleshooting Build Issues

### Error: "Cannot find sdhd_sdk.hh"

**Problem:** Visual Studio can't find the SDK headers.

**Solution:**
1. Check the **Additional Include Directories** path
2. Make sure it points to the SDK root folder (where `sdhd_sdk.hh` is located)
3. Use absolute paths, not relative paths
4. Don't include a trailing slash

### Error: "C1189: #error: (void*) isn't 64-bit"

**Problem:** You're building for x86 instead of x64.

**Solution:**
1. Change the platform dropdown from **Win32** to **x64**
2. Go to **Build → Configuration Manager**
3. Create a new x64 platform if it doesn't exist

### Error: LNK2001 unresolved external symbols

**Problem:** Missing library dependencies.

**Solution:**
1. Go to **Project Properties → Linker → Input**
2. Add to Additional Dependencies:
   ```
   d3d11.lib;dinput8.lib;XInput.lib
   ```

### Warning: C4244 conversion from 'double' to 'float'

**Solution:** This is harmless, but you can fix it by explicitly casting:
```cpp
// Change this:
float value = 1.5;

// To this:
float value = 1.5f;
```

---

## Performance Build (Advanced)

For maximum performance, use these additional compiler settings:

### C/C++ → Optimization (Release only)

| Setting | Value |
|---------|-------|
| Optimization | Maximum Optimization (/O2) |
| Inline Function Expansion | Any Suitable (/Ob2) |
| Favor Size Or Speed | Favor fast code (/Ot) |
| Enable Intrinsic Functions | Yes (/Oi) |

### C/C++ → Code Generation (Release only)

| Setting | Value |
|---------|-------|
| Enable Function-Level Linking | Yes (/Gy) |
| Floating Point Model | Fast (/fp:fast) |

### Linker → Optimization (Release only)

| Setting | Value |
|---------|-------|
| References | Eliminate Unreferenced Data (/OPT:REF) |
| Enable COMDAT Folding | Yes (/OPT:ICF) |

---

## Debugging the Mod

### Using DebugView

1. Download [DebugView](https://docs.microsoft.com/en-us/sysinternals/downloads/debugview)
2. Run as Administrator
3. Go to **Capture → Capture Win32**
4. Launch your game with the mod
5. You'll see all `OutputDebugStringA()` messages

### Using Visual Studio Debugger

1. Build in **Debug** mode (not Release)
2. Go to **Debug → Attach to Process**
3. Find and select `HKShip.exe`
4. Click **Attach**
5. Set breakpoints in your code
6. They'll trigger when that code runs

---

## Customizing the Mod

Want to change the presets or controls? Here's what to modify:

### Changing Presets

Edit the `PRESETS` array in `TrafficPedDensityMod.cpp`:

```cpp
const DensityPreset PRESETS[] = {
    // name,     traffic, parked, peds, traffic_scale, ped_scale
    { "Custom",   100,     80,     100,  2.0f,         2.0f  },
};
```

### Changing Controls

Find the `ProcessInput()` function and modify the key codes:

```cpp
// Change F7 to F12:
if (IsKeyPressed(VK_F12)) {  // Was VK_F7
    ApplyPreset(2); // High
}
```

**Common Virtual Key Codes:**
- `VK_F1` through `VK_F12` - Function keys
- `VK_NUMPAD0` through `VK_NUMPAD9` - Numpad
- `VK_ADD`, `VK_SUBTRACT` - Numpad +/-
- `VK_HOME`, `VK_END`, `VK_PRIOR` (Page Up), `VK_NEXT` (Page Down)

### Adding New Presets

1. Add a new preset to the array:
```cpp
const DensityPreset PRESETS[] = {
    { "Low",      30,  30,  20, 0.5f,  0.5f  },
    { "Normal",   50,  50,  42, 1.0f,  1.0f  },
    { "High",     80,  70,  80, 1.5f,  1.5f  },
    { "Ultra",    120, 100, 120, 2.0f,  2.0f  },
    { "Extreme",  200, 150, 200, 3.0f,  3.0f  },
    { "MyCustom", 90,  75,  90, 1.7f,  1.6f  },  // <-- New!
};
```

2. Add a hotkey to activate it:
```cpp
// In ProcessInput():
if (IsKeyPressed(VK_F11)) {
    ApplyPreset(5); // Index of your new preset
}
```

---

## Next Steps

Once you have the mod working:

1. **Experiment with different density levels** to find your ideal balance
2. **Check out other example mods** in `SDK/examples/`
3. **Read the modding guide** at `SDK/MODDING_GUIDE.md`
4. **Create your own mods!**

---

## Getting Help

If you're stuck:

1. **Check DebugView** for error messages
2. **Review this guide** step by step
3. **Check the main modding guide** at `SDK/MODDING_GUIDE.md`
4. **Ask the community** on the SDK GitHub

Happy modding! 🚗👥🏙️
