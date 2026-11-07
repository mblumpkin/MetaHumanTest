# 01 — Overview: what was built

## Scope (what was asked)

From the Figma page *Default iPad Version*, the components at the top of the layers panel:
**Component 4, Component 3, videos row, Settings, History & Timeline, Saved Videos, Progress, Profile** — plus their
functionality. The MetaHuman picture in the mock-ups was **not** recreated (the real 3D avatar is already in the level).

The iPad frames show how these components fit together, so they were assembled the same way:

| Figma frame | What it shows | Unreal |
|---|---|---|
| iPad Pro 12.9" – 1 | Title bar, closed menu (dark circle, white ▼) | ![](images/unreal_01_hud_closed.png) |
| iPad Pro 12.9" – 3 | Menu open: 5 tabs (white circle, blue ▲) | ![](images/unreal_02_nav_open.png) |
| iPad Pro 12.9" – 5 | Analysis panel: viewer, Component 3, videos row | ![](images/unreal_03_analysis_panel.png) |

> The playback bar and the skin/muscle/skeleton switcher in the Figma frames are **screenshots of the existing Unreal
> widgets** (`ihpc ui (1)ee 1`, `ihpc ui rttd 2` layers). They were left as they are.

## Behaviour

### Title bar + menu (`WBP_IHPC_HUD`)
* **Round arrow** under the title bar opens / closes the nav bar.
  * Closed: see-through navy circle `#1D3C55 @ 44%` with a white ▼.
  * Open: white circle `@ 86%` with a blue `#245983` ▲, moved down to sit under the nav bar.
* **Tabs** (Profile, Progress, Saved Videos, History & Timeline, Settings):
  * Hover or selected → Figma *Variant2*: bold, green `#B3CA7F`, 1.2× bigger.
  * Click a tab to select it, click again to unselect.
  * **Saved Videos** opens the analysis panel and closes the nav bar (exactly like frame 5).
  * The other four tabs highlight and fire an event (`OnTabChanged`) — **their screens are not designed yet** (see
    [07](07_Known_Limitations_And_Next_Steps.md)).
* Clicks on empty parts of the screen still reach the 3D view, so orbit / pan / zoom keep working.

### Analysis panel (`WBP_IHPC_AnalysisPanel`)
* Big dark rounded panel (`rgba(0,0,0,0.62)`, radius 29) with a viewer area, Component 3 on the right and the videos row
  at the bottom.
* **Inspect** button (magnifier with `</>` in the viewer corner): closes the big graph if it is open; otherwise fires
  `OnInspectClicked` for a future 3D close-up.

### Component 3 — graph accordion (`WBP_IHPC_GraphAccordion`)

| Closed (Figma Default) | Show graphs open (Figma Variant2) | Bones only | Tagged Points |
|---|---|---|---|
| (headers only) | ![](images/unreal_04_show_graphs.png) | ![](images/unreal_05_bones_filter.png) | ![](images/unreal_07_tagged_points.png) |

* Click a header to open its section; only **one section is open at a time**.
* **Show graphs**: two **live** line graphs (Movement Graph, Active Muscles) drawn from numbers, not pictures.
  * A green **playhead** line follows the avatar animation timeline.
  * **Switches**: with none on, every line shows (this is what Figma draws). Turn some on to show only those lines.
  * **Download** writes both graphs to `Saved/IHPC_Exports/IHPC_Graphs_<date>_<time>.csv`
    (hover the icon afterwards to see the full path).
  * **Zoom** shows a big Movement Graph in the viewer area:

    ![](images/unreal_06_zoom_and_playhead.png)
* **Active Muscles**: a list of muscles (sample names from the level's Muscles folder until real data is connected).
* **Tagged Points**: the same moments as the video cards; the dot colour = severity; clicking a row jumps the animation.

### Videos row (`WBP_IHPC_VideosRow` + `WBP_IHPC_VideoCard`)
* Five sample cards from Figma ("Soleus Muscle (R)", "Neck Axis (C2)", "Jan 4th, 2026", "13:55 p.m.").
* Each card's text comes from data; the **white tick** on the green→red bar is the severity (0 = left, 1 = right).
* Scroll with the mouse wheel or drag the thin light-blue indicator above the cards.
* **Click a card** → it gets a green outline and the avatar animation jumps to that moment (the playhead on the graphs
  moves there too):

  ![](images/unreal_08_selected_card.png)

### Component 4 — toggle switch (`WBP_IHPC_ToggleSwitch`)
Grey track + knob left (off) / green `#B3CA7F` track + knob right (on), exactly the two Figma variants.

## Connection to the existing project
* Clicking a card or tagged point calls the project's existing **`UAnimationTimelineComp`** (the component the
  `WB_AmationTimeline` play/pause/scrub bar already uses) on the player's pawn: `Pause()` then `SetNormalizedTime()`.
* Nothing in the existing widgets or C++ was changed, except the **Level Blueprint** of `Main` now creates the new HUD
  instead of `WBP_Header`, and `MetaHumanTest.Build.cs` now lists `UMG`, `Slate`, `SlateCore` (needed by the new code).
