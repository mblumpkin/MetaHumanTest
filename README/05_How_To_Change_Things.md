# 05 — How to change things

## Change a colour, font, text, size or position
Open the Widget Blueprint in `Content/UserInterface/IHPC/Widgets`, select the widget in the **Hierarchy**, change it in
**Details**, **Compile**, **Save**. Keep the names marked (C++) in [04](04_Widget_Blueprints.md).

* Tab fonts / colours: open `WBP_IHPC_NavTab` → **Class Defaults** → *IHPC | Style*.
* Chevron colours / arrows: `WBP_IHPC_HUD` → Class Defaults → *IHPC | Style*.
* Accordion chevrons / list font: `WBP_IHPC_GraphAccordion` → Class Defaults.

## Switch the font to Inter (like Figma)
1. Download Inter (free, OFL licence) and drag `Inter-Medium.ttf` and `Inter-Bold.ttf` into the Content Browser
   (e.g. `/Game/UserInterface/IHPC/Fonts`). Answer **Yes** when Unreal asks to create a Font asset.
2. Replace the Avenir font in the TextBlocks and in the Class Defaults listed above
   (Avenir Medium → Inter Medium, Avenir Heavy → Inter Bold). Sizes stay the same.

## Make a different tab open the analysis panel
`WBP_IHPC_HUD` → Class Defaults → **Analysis Panel Tab**.

## Add a screen for Profile / Progress / History & Timeline / Settings
1. Make a new Widget Blueprint for the screen.
2. In `WBP_IHPC_HUD`, add it to `HudCanvas` (e.g. same place as `AnalysisPanel`), set it **Collapsed**, tick **Is Variable**.
3. In the HUD's Event Graph, add the event **On Tab Changed** (bound from *IHPC | Events*), and show the screen when
   `NewTab` equals your tab, hide it otherwise.
   (Or in C++: add a `BindWidgetOptional` pointer to `CPP_UW_IHPCHud` and show/hide it in `SelectTab`.)

## Show real saved videos instead of the sample cards
From Blueprint or C++, build an array of **IHPC Tagged Moment** (Title, Date Text, Time Text, Severity 0–1, Normalized
Time 0–1) and call **Set Moments** on the videos row (get it from the analysis panel's `VideosRow`). The Tagged Points
list is filled from the same data when the panel is constructed; call **Set Tagged Points** on the accordion if you
change the data later. **Add Moment** appends one card (e.g. after a new recording).

## Show real graph data
Call **Set Series** on `MovementGraph` / `ActiveMusclesGraph` (array of **IHPC Graph Series**: Name, Line Color,
Legend Color, Values, Visible). Keep the order Bones, Nerves, Muscles, Tendons so the switches still match. Values are
evenly spaced across the width; adjust **Min Value / Max Value / Value Ticks** if your data uses a different range. Set
**Generate Demo Data If Empty** off to stop sample curves appearing.

## Show real active muscles
Call **Set Active Muscles** on the accordion with an array of names.

## Timeline / playback bar
* **Speeds** the slow/fast buttons step through: `WBP_IHPC_Timeline` → Class Defaults → *Speed Steps*; starting speed =
  *Default Speed Index*.
* **Don't start playback when recording starts**: untick *Play When Recording Starts*.
* **Names of new clips/bookmarks**: *Recording Title Format* / *Bookmark Title Format* (`{0}` = the number).
* **Move the bar** (e.g. left of centre like Figma): select `Timeline` in `WBP_IHPC_HUD` and change its position /
  anchors. If you change the track length inside `WBP_IHPC_Timeline`, set *Track Length* to match.
* **Make fast-forward something else** (e.g. skip 5 seconds, or play at 4×): change
  `UCPP_UW_IHPCTimeline::FastForwardToNextMarker` (C++), or bind your own logic to the button in the Blueprint.
* **Save real video files when recording**: hook into *On Recording Saved* (HUD/Timeline) or extend
  `UCPP_UW_IHPCTimeline::ToggleRecording` — for example start/stop a Take Recorder or Movie Render capture there.
* **Get the old prototype timeline back**: `BP_PlayerController` → Class Defaults → *Timeline Widget Class* =
  `WB_AmationTimeline` (and delete `Timeline` from `WBP_IHPC_HUD` so there are not two).

## React to clicks in Blueprint
All events are under *IHPC | Events* on the widgets: `On Tab Changed`, `On Moment Selected`, `On Tagged Point Clicked`,
`On Graphs Exported`, `On Zoom Requested`, `On Inspect Clicked`, and on the timeline `On Recording Saved`,
`On Bookmark Added`, `On Marker Clicked`, `On Speed Changed`.

## Change where the CSV goes / its format
`UCPP_UW_IHPCGraphAccordion::ExportGraphsToCSV` in `Private/IHPC_UI/CPP_UW_IHPCGraphAccordion.cpp`.

## After the Figma file changes
1. Download the changed assets into `SourceArt/Figma/Original/` (same names).
2. `python -I Tools/FigmaUI/make_ui_textures.py` (from the project folder).
3. Import: in Unreal **Tools → Execute Python Script…** → `Tools/FigmaUI/ue_import_textures.py` (replaces the textures).
4. Update numbers in the Widget Blueprints by hand, using [02](02_Figma_To_Unreal_Map.md) as the checklist.

## Editing the C++
Close Unreal (Live Coding is on, so command-line builds are blocked while it is open), then build:
```
"C:/Program Files/Epic Games/UE_5.8/Engine/Build/BatchFiles/Build.bat" MetaHumanTestEditor Win64 Development "-Project=<project folder>/MetaHumanTest.uproject" -WaitMutex
```
or open `MetaHumanTest.sln` and build **Development Editor**. Small changes inside existing functions can also be
applied with Live Coding (Ctrl+Alt+F11) while the editor is open; adding new classes or `UPROPERTY`s needs a full build.
