# Traffic & Pedestrian Density Mod

**Make Hong Kong feel alive!** This mod increases the traffic and pedestrian density in Sleeping Dogs for a more realistic, bustling city experience.

## Features

✅ **Customizable Traffic Density** - Control the number of vehicles on the road (0-200)
✅ **Customizable Pedestrian Density** - Control the number of pedestrians (0-200)
✅ **Five Presets** - From quiet streets to ultra-crowded Hong Kong
✅ **Real-time Adjustments** - Change settings on the fly while playing
✅ **Performance-Friendly** - Designed to minimize impact on FPS
✅ **Info Display** - See live stats of active traffic and pedestrians

---

## Installation

1. **Build the mod:**
   - Open Visual Studio and create a new DLL project (x64)
   - Add the SDK include path to your project settings
   - Copy `TrafficPedDensityMod.cpp` into your project
   - Build to create `TrafficPedDensityMod.dll`

2. **Load into game:**
   - Use a DLL injector (Ultimate ASI Loader, Cheat Engine, etc.)
   - Inject `TrafficPedDensityMod.dll` into `HKShip.exe`
   - Launch Sleeping Dogs

3. **Verify it's working:**
   - Use [DebugView](https://docs.microsoft.com/en-us/sysinternals/downloads/debugview) to see the initialization message
   - Press F10 in-game to see current stats

---

## Controls

### Fine-Tuning

| Key | Action |
|-----|--------|
| **F1** | Decrease Traffic (-10 vehicles) |
| **F2** | Increase Traffic (+10 vehicles) |
| **F3** | Decrease Pedestrians (-10 peds) |
| **F4** | Increase Pedestrians (+10 peds) |
| **F10** | Toggle Info Display |

### Presets

| Key | Preset | Traffic | Parked | Pedestrians | Description |
|-----|--------|---------|--------|-------------|-------------|
| **F5** | Low | 30 | 30 | 20 | Quiet city, best performance |
| **F6** | Normal | 50 | 50 | 42 | Game default settings |
| **F7** | High | 80 | 70 | 80 | Busy city streets |
| **F8** | Ultra | 120 | 100 | 120 | Very crowded Hong Kong |
| **F9** | Extreme | 200 | 150 | 200 | Maximum density (⚠️ May cause lag!) |

---

## Presets Explained

### 🟢 Low (F5)
- **Best for:** Low-end PCs, missions with heavy action
- **Performance:** Excellent
- **Realism:** Below average
- Quiet streets, minimal traffic and pedestrians

### 🟡 Normal (F6) - Default
- **Best for:** Most players, balanced experience
- **Performance:** Good
- **Realism:** Average (game default)
- The way the game was designed to be played

### 🟠 High (F7) - Recommended
- **Best for:** Mid-range PCs, enhanced realism
- **Performance:** Good
- **Realism:** High
- Noticeably busier streets without major performance impact

### 🔴 Ultra (F8)
- **Best for:** High-end PCs, maximum immersion
- **Performance:** Moderate
- **Realism:** Very High
- Hong Kong feels truly alive and crowded

### ⚠️ Extreme (F9)
- **Best for:** Top-tier PCs, screenshots, cinematic experiences
- **Performance:** May cause lag/stuttering
- **Realism:** Maximum
- Not recommended for gameplay, best for screenshots or short sessions

---

## Performance Guide

### Recommended Settings by Hardware

**Low-End PC (GTX 750 Ti / RX 560 or lower):**
- Use **Low** or **Normal** preset
- Max: 50 traffic, 40 pedestrians

**Mid-Range PC (GTX 1060 / RX 580):**
- Use **High** preset
- Max: 100 traffic, 80 pedestrians

**High-End PC (RTX 2060+ / RX 5700 XT+):**
- Use **Ultra** preset
- Max: 150 traffic, 120 pedestrians

**Top-Tier PC (RTX 3080+ / RX 6800 XT+):**
- Try **Extreme** preset
- Max: 200 traffic, 200 pedestrians

### Performance Tips

1. **Start with Normal preset** (F6) and gradually increase
2. **Monitor your FPS** - if it drops below 30, reduce density
3. **Use High preset (F7)** for best balance of realism and performance
4. **Extreme preset** is only for screenshots or cinematic moments
5. **Reduce parked cars** if you don't notice them much
6. **Pedestrian density** impacts performance less than traffic

---

## How It Works

This mod interfaces with the game's spawning systems:

### Traffic System (WheeledVehicleManager)
- `SetMaxTrafficCars()` - Maximum moving vehicles
- `SetMaxParkedCars()` - Maximum parked vehicles
- `SetTrafficDensityScale()` - Spawn rate multiplier

### Pedestrian System (PedSpawnManager)
- `SetTargetPedCount()` - Target number of ambient peds
- `AdjustPedDensity()` - Ped spawn density per square meter

The mod dynamically adjusts these values and applies them in real-time.

---

## Troubleshooting

### Mod doesn't load
- Check that DLL is x64 architecture
- Verify SDK path in Visual Studio include directories
- Ensure DLL injector is working correctly
- Check DebugView for error messages

### No visible changes
- Wait 10-15 seconds after applying preset for spawning to update
- Try driving to a new area to trigger fresh spawns
- Press F10 to check if mod is active (shows stats)
- Increase density more dramatically (try Extreme preset)

### Game is laggy
- Lower the preset (F5 for Low)
- Use fine-tuning (F1/F3) to reduce traffic/peds
- Close background applications
- Reduce graphics settings in-game

### Game crashes
- Don't exceed 200 for any setting
- Start with Normal preset and increase gradually
- Ensure you have enough RAM (8GB+ recommended for Ultra/Extreme)

### Traffic/Peds disappear
- This is normal game behavior when you're not looking at them
- The game culls distant objects for performance
- Drive around to trigger new spawns

---

## Advanced Usage

### Custom Presets

You can create your own presets by editing the `PRESETS` array in the source code:

```cpp
const DensityPreset PRESETS[] = {
    { "Custom", 65, 60, 70, 1.2f, 1.3f },  // Your settings
    // name, traffic, parked, peds, traffic_scale, ped_scale
};
```

### Fine-Tuning for Specific Areas

Some areas benefit from different settings:

- **Downtown/Central** - Use High or Ultra for busy urban feel
- **Residential Areas** - Use Normal or High
- **Industrial/Docks** - Use Low or Normal (fewer people)
- **Highways** - Increase traffic, decrease pedestrians

### Cinematic Mode

For screenshots or recording:
1. Press F9 (Extreme preset)
2. Find a good location
3. Wait 15-20 seconds for full spawn
4. Take screenshots
5. Press F6 to return to Normal for gameplay

---

## Technical Details

### SDK Systems Used

**WheeledVehicleManager** (`/vehicles/wheeledvehiclemanager.hh`):
- Located at RVA `0x2433B10`
- Manages all traffic and parked vehicles
- Controls spawn rates and vehicle limits

**PedSpawnManager** (`/gamestate/hk/pedspawnmanager.hh`):
- Located at RVA `0x23DC610`
- Manages pedestrian spawning and lifecycle
- Controls ambient ped count and density

### Global Variables Modified

| Variable | RVA | Purpose |
|----------|-----|---------|
| `mTrafficCarSpawningEnabled` | 0x2090516 | Enable traffic spawning |
| `mParkedCarSpawningEnabled` | 0x2090517 | Enable parked car spawning |
| `msTargetPedCount` | 0x207AF0C | Target pedestrian count |
| `msBasePedsPerSquareMetre` | 0x207B21C | Ped density value |

---

## Compatibility

✅ **Compatible with:**
- All game versions (Definitive Edition)
- Other graphics mods
- Gameplay mods that don't modify spawning

⚠️ **May conflict with:**
- Other traffic/pedestrian density mods
- Scripts that control vehicle/ped spawning
- Performance optimization mods that limit spawning

---

## FAQ

**Q: Will this work with missions?**
A: Yes, but use Normal or High presets. Extreme may cause issues.

**Q: Does this work in multiplayer?**
A: Sleeping Dogs doesn't have multiplayer in the traditional sense. This mod is for single-player.

**Q: Can I save my settings?**
A: Currently settings reset when you reload. Future version may add config file.

**Q: Why do I see fewer vehicles than the max count?**
A: The game dynamically spawns based on your location. The max count is the total across the entire visible area.

**Q: Will this improve graphics?**
A: No, this only increases density. Use graphics mods for visual improvements.

**Q: Is this safe to use?**
A: Yes, it only modifies spawning variables. It doesn't modify save files or game files.

---

## Credits

- **SDK Development:** SDmodding Community
- **Mod Author:** [Your Name]
- **Sleeping Dogs:** United Front Games, Square Enix

---

## License

This mod is provided as-is for educational and personal use. Feel free to modify and share!

---

## Version History

### v1.0 (Current)
- Initial release
- 5 density presets (Low to Extreme)
- Real-time adjustment controls
- Info display system
- Performance optimized

---

## Support

For issues, suggestions, or feedback:
- Check the SDK documentation in `MODDING_GUIDE.md`
- Use DebugView to check for error messages
- Report issues on the GitHub repository

Enjoy your more realistic Hong Kong! 🏙️🚗👥
