# 06 — Pipeline and tools

How the Figma design became working Unreal UI, step by step, so it can be repeated.

## 1. Read the design
Using the Figma MCP connection (`get_metadata`, `get_design_context`, `get_screenshot`) on
`IImR794aBnUhWDt891b1wQ`, page *Default iPad Version*:
* the 8 requested components (node ids in [02](02_Figma_To_Unreal_Map.md)),
* iPad frames 1, 3 and 5 for how they fit together.
Screenshots of those frames are in `SourceArt/Figma/Screens/`.

## 2. Download the assets → `SourceArt/Figma/Original/`
SVGs (toggle states, chevrons, zoom, header circle/arrow, tab separator) and PNGs (download icon, inspect icon,
video-card art, chart picture).

## 3. Make Unreal-ready textures → `SourceArt/Figma/UE/`
```
python -I Tools/FigmaUI/make_ui_textures.py
```
[`Tools/FigmaUI/make_ui_textures.py`](../Tools/FigmaUI/make_ui_textures.py):
* rasterises each SVG from its exact numbers at 4× (Unreal cannot import SVG), with round caps/joins like the SVG;
* resizes the two PNG icons;
* builds the title-glow gradient and the bar drop-shadow from the Figma CSS values;
* rebuilds the **video card** without text: the Figma card picture has the text baked in, but every text-free row of the
  card body is identical, so one clean row is copied down the body; the white severity tick is removed by borrowing the
  pixels from another card where the tick is elsewhere.

## 4. Import into Unreal → `/Game/UserInterface/IHPC/Textures`
[`Tools/FigmaUI/ue_import_textures.py`](../Tools/FigmaUI/ue_import_textures.py) (runs inside Unreal's Python).
Sets UI-appropriate settings on each texture: *UserInterface2D* compression, no mipmaps, *UI* texture group, sRGB.
It was run headless while the editor was closed:
```
UnrealEditor-Cmd.exe <project>/MetaHumanTest.uproject -run=pythonscript -script=<project>/Tools/FigmaUI/ue_import_textures.py -unattended -nosplash -nullrhi -SCCProvider=None
```
(With the editor open you can run it from **Tools → Execute Python Script…**.)

## 5. Write the C++ → `Source/MetaHumanTest/*/IHPC_UI`
See [03](03_Code_Walkthrough.md). Built with the editor closed:
```
Build.bat MetaHumanTestEditor Win64 Development -Project=<project>/MetaHumanTest.uproject -WaitMutex
```
Result: *Succeeded*, no warnings in the new files.

## 6. Build the Widget Blueprints → `/Game/UserInterface/IHPC/Widgets`
Built through the Unreal MCP connection (UMG toolset + property tools). The exact script — every widget, size,
colour and font — is kept in [`Tools/FigmaUI/build_widget_blueprints.py`](../Tools/FigmaUI/build_widget_blueprints.py).
Build order: ToggleSwitch, NavTab, VideoCard → VideosRow → GraphAccordion → AnalysisPanel → HUD (each one uses the ones
before it).

## 7. Hook into the level
`Main` Level Blueprint: the existing *Create Widget* node's class was changed from `WBP_Header` to `WBP_IHPC_HUD`, and the
*Add to Viewport* Z-order set to 10 (draws above the existing timeline/anatomy widgets).

## 8. Test in Play-In-Editor
Clicked through every control (menu, tabs, accordion, switches, download, zoom, inspect, cards, tagged points) and
compared screenshots with the Figma frames — see the images in [01](01_Overview.md). One bug found and fixed: the selected
card's green outline was invisible because the outline inherited the fill's 0% opacity.

## Step 2 — timeline / playback bar (same pipeline)
1. **Read**: Figma layer `ihpc ui (1)ee 1` (`124:1059`) on frame 1 + the `x1.5` text layer (`153:323`).
2. **Download**: `SourceArt/Figma/Original/timeline_export.png` (1×) and `timeline_raw_1.png` (3× original).
3. **Measure**: each element's box from the 3× picture's alpha channel (see the table in [02](02_Figma_To_Unreal_Map.md)).
4. **Textures**: `make_timeline_assets()` in `make_ui_textures.py` — exact crops of the 8 buttons, a circular crop of the
   knob, and the marker dot drawn from its measured sizes/colour → `T_IHPC_TL_*` (10 textures).
5. **C++**: `CPP_UW_IHPCTimeline` + small additions to the library / accordion / panel / HUD; built with the editor
   closed (*Succeeded*, no new warnings).
6. **Import**: `ue_import_textures.py` (headless) — imports all PNGs; the existing 13 came out unchanged.
7. **Widget Blueprint**: `build_timeline()` in `build_widget_blueprints.py`; `add_timeline_to_hud_and_retire_prototype()`
   put it into the HUD and cleared `BP_PlayerController` → *Timeline Widget Class*.
8. **Test** in Play-In-Editor: every button, scrubbing, markers, and that recordings/bookmarks reach Saved Videos and
   Tagged Points (screenshots in [01](01_Overview.md)).

Two problems found and fixed during step 2:
* **Assets were read-only.** The Git LFS `lockable` attribute (first commit) makes Git set every committed `.uasset/.umap`
  to read-only until it is "locked", so Unreal could not save them. `lockable` was removed from `.gitattributes` and the
  read-only flags cleared. If you ever see "*… as it is read only*" when saving, run
  `attrib -R /S Content\*.uasset` (or untick *Read-only* in the file's Properties).
* **Colours looked too dark.** Colour brushes copied from the default ProgressBar / Slider styles still pointed at the
  engine's grey pictures (`ProgressBar_Background.png`, `Button.png`), which multiplied the colour (~70%). Cleared on the
  timeline track/slider and the videos-row scroll thumb; `brush_patch()` now always clears it.

## Git / GitHub
* Repo: <https://github.com/mblumpkin/MetaHumanTest> (private), branch `main`.
* `.uasset`, `.umap`, images and fonts are stored with **Git LFS** (see `.gitattributes`). (They are *not* marked
  lockable any more — see step 2 above.)
* `Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/`, `.vs/`, `*.sln` are ignored (rebuilt automatically).
* Every step above was committed and pushed separately — see [CHANGELOG.md](CHANGELOG.md).
