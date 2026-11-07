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

**Step 2 (added later):** the playback / timeline bar from the Figma layer **`ihpc ui (1)ee 1`** on frame 1 — record,
restart, pause, slow down, speed up, play, fast-forward, bookmark and the timeline itself — replacing the project's
prototype timeline. See [Timeline / playback bar](#timeline--playback-bar-wbp_ihpc_timeline) below.
(The skin/muscle/skeleton switcher, layer `ihpc ui rttd 2`, is still the existing widget.)

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

### Timeline / playback bar (`WBP_IHPC_Timeline`)

| Figma (`ihpc ui (1)ee 1` + the `x1.5` text) | Unreal |
|---|---|
| ![](images/figma_05_timeline.png) | ![](images/unreal_09_timeline_x1_5.png) |

At the bottom centre of the screen (inside the HUD, behind the analysis panel). Left to right:

| Button | What it does |
|---|---|
| **Record** (ring) | 1st click: start recording (also starts playback); the button turns red and a red dot pulses. 2nd click: stop → a new clip **"Recording N"** (date/time = when you started, position = where you started) appears as a **new card in Saved Videos**, a new **Tagged Point** and a new **marker** on the track. |
| **Restart** (◀\|) | Jump to the start; keeps playing if it was playing. |
| **Pause** | Pause the avatar animation. |
| **Slow down** (low speedometer) | One step slower: 3× → 2× → 1.5× → 1× → 0.5× → 0.25×. |
| **Timeline** | Dark part = already played, light part = still to play. **Drag or click** to scrub (playback pauses while you drag and resumes after). The knob shows the speed (**x1.5** in Figma). **Orange markers** = saved videos, recordings and bookmarks; **click a marker** to jump there; hover it to see its name. |
| **Speed up** (high speedometer) | One step faster. |
| **Play** | Play forward (from the start again if it is at the end). |
| **Fast-forward** (\|▶) | Jump to the **next marker** after the current time (or to the end if there is none). |
| **Bookmark** | Drop a marker at the current time; it is added to **Tagged Points** as "Bookmark N". |

| Recording (button red, dot pulsing) | New recordings in Saved Videos | Bookmark + recording in Tagged Points |
|---|---|---|
| ![](images/unreal_10_timeline_recording.png) | ![](images/unreal_11_recordings_in_saved_videos.png) | ![](images/unreal_12_bookmark_recording_tagged_points.png) |

The old prototype timeline (`WB_AmationTimeline`: three grey buttons + a slider) is **no longer created** — its slot in
`BP_PlayerController` (*Timeline Widget Class*) was cleared. The asset and its C++ (`WCPP_AnimationTimeline`) are still in
the project if you want them back ([05](05_How_To_Change_Things.md)).

![The new bar in the level](images/unreal_13_timeline_in_level.png)

## Connection to the existing project
* Clicking a card, tagged point or marker, and every timeline button, use the project's existing
  **`UAnimationTimelineComp`** on the player's pawn (`PlayForward`, `Pause`, `SetNormalizedTime`, `PlaySpeed`) — the same
  component the old prototype timeline used.
* The HUD is created by **`CPP_PlayerController`** like the project's other widgets: new `MainHUDWidgetClass` (set to
  `WBP_IHPC_HUD` in `BP_PlayerController`), `MainHUDZOrder` (10) and `GetMainHUD()`; the Level Blueprint of `Main` no
  longer creates any UI (it did during step 1). **`BP_PlayerController`** no longer creates `WB_AmationTimeline`.
  `MetaHumanTest.Build.cs` lists `UMG`, `Slate`, `SlateCore` (needed by the new code).
