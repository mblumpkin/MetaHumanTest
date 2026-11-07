# 07 — Known limitations and next steps

## Placeholder / sample data (works, but not real yet)
| What | Where it comes from now | How to make it real |
|---|---|---|
| Video cards | 5 sample moments copied from Figma (`CPP_UW_IHPCVideosRow` constructor) | `SetMoments` / `AddMoment` from your recording system |
| Graph lines | `GenerateDemoData` (smooth sample curves) | `SetSeries` with motion-capture / muscle data |
| Active Muscles list | 6 names from the level's Muscles folder | `SetActiveMuscles` |
| Severity values | 0.17 / 0.41 read off the Figma card ticks | your injury-risk calculation |

## Interpretations (decisions made where Figma did not say)
* **Saved Videos opens the analysis panel.** The panel shows saved clips, graphs and tagged points, and frame 5 shows
  it with the menu closed. Change with *Analysis Panel Tab* on `WBP_IHPC_HUD`.
* **Profile, Progress, History & Timeline, Settings** highlight and fire `OnTabChanged`, but have no screen — none is in
  the requested components. (The desktop frames on the same Figma page show sub-menus such as *Manage Information,
  MRI & Cam Scan, Timeline…*; they can be built next.)
* **Switches = filters.** Figma draws all four switches off with all lines visible, so "none on" = show all; turning
  some on shows only those lines.
* **One accordion section open at a time** (keeps the panel within the screen; Figma shows one open).
* **Active Muscles / Tagged Points open state** is not designed in Figma; a simple list in the same style is used.
* **Zoom** shows a large Movement Graph in the empty viewer area; **Inspect** closes it (otherwise fires an event for a
  future 3D close-up).
* **Clicking a card / tagged point** pauses the avatar animation and jumps to the moment's time.
* **Title vertical position**: centred in the bar (the Figma frame includes an iPad status bar that PC builds do not have).

## Visual differences from Figma
* **Font**: Avenir (already in the project) instead of Inter. See [05](05_How_To_Change_Things.md) to switch.
* **Charts**: drawn live instead of the Figma chart picture — same layout, axes and label style, real data lines.
* **Frosted-glass blur** is not used (Figma panels here are plain semi-transparent fills, which match).
* The first card starts 24 px in and the row clips at that edge (Figma clips at the row edge).

## Things noticed in the existing project (not changed)
* Play-In-Editor logs `CommonUI without a CommonGameViewportClient` and two Game Features / OpenXR errors — these were
  there before and do not affect this UI.
* `CPP_UW_AnatomyViewSwitcher.cpp` has three `double → float` truncation warnings (lines 164, 182, 202).
* The older `WBP_Header` draft (`Content/UserInterface/Widgets`) is no longer created by the level; delete it if not needed.
* The project folder is also a Perforce workspace; the Unreal editor's Perforce session had expired ("Your session has
  expired, please login again") during this work.

## Suggested next steps
1. Connect real saved videos (recording → `AddMoment`).
2. Feed real motion data into the graphs (`SetSeries`) and make the **Download** export that data.
3. Build the Profile / Progress / History & Timeline / Settings screens (Figma desktop frames show their sub-menus).
4. Use the empty viewer area for a 3D close-up (e.g. a Scene Capture of the avatar) and hook it to **Inspect**.
5. Optionally switch to the Inter font.
