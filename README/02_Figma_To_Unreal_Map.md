# 02 — Figma → Unreal map

Figma file: **Avatar Injury Prevention Program**, page **Default iPad Version** (file key `IImR794aBnUhWDt891b1wQ`).
Frames are **iPad Pro 12.9" (1366 × 1024)**. All Figma numbers below are Figma pixels.

## Component → asset → class

| Figma component (node id) | Widget Blueprint | C++ class (behaviour) |
|---|---|---|
| Profile `129:1274`, Progress `129:1277`, Saved Videos `129:1280`, History & Timeline `129:1283`, Settings `129:1286` | `WBP_IHPC_NavTab` (used 5×) | `UCPP_UW_IHPCNavTab` |
| Title bar / chevron / nav bar (frames 1 & 3, `99:252`, `128:1155`) | `WBP_IHPC_HUD` | `UCPP_UW_IHPCHud` |
| Analysis panel (frame 5, `129:1312`) | `WBP_IHPC_AnalysisPanel` | `UCPP_UW_IHPCAnalysisPanel` |
| Component 3 `157:654` | `WBP_IHPC_GraphAccordion` | `UCPP_UW_IHPCGraphAccordion` |
| Movement Graph / Active Muscles charts (inside Component 3) | *(no Blueprint — drawn in code)* | `UCPP_UW_IHPCLineGraph` |
| Component 4 `160:409` | `WBP_IHPC_ToggleSwitch` | `UCPP_UW_IHPCToggleSwitch` |
| videos row `157:620` | `WBP_IHPC_VideosRow` | `UCPP_UW_IHPCVideosRow` |
| one card inside videos row | `WBP_IHPC_VideoCard` | `UCPP_UW_IHPCVideoCard` |
| *(not in Figma — rows for the open Active Muscles / Tagged Points sections)* | *(built in code)* | `UCPP_UW_IHPCListRow` |

## Units

* **Sizes / positions**: Figma px = Unreal Slate units (1:1).
* **Font sizes**: Unreal uses points at 96 DPI → **Unreal size = Figma px × 0.75** (Figma 16 px → Unreal 12).
* **Colours**: Figma hex is sRGB; Unreal stores linear colour. Converted with the standard sRGB → linear formula
  (`UIHPCUILibrary::FigmaColor` in C++, `lc()` in `Tools/FigmaUI/build_widget_blueprints.py`).
* **Fonts**: Figma uses *Inter*, which is not in the project. The team's existing **Avenir** font is used:
  Inter Medium → **Avenir Medium**, Inter Bold → **Avenir Heavy**. The title uses Avenir Heavy in Figma too.

## Title bar, chevron, nav bar (`WBP_IHPC_HUD`)

| Element | Figma | Unreal |
|---|---|---|
| Title bar | `#1D3C55`, 105 tall, shadow `0 7 6.3 rgba(0,0,0,.25)` | `TitleBarBackground` Border, 105; `TitleBarShadow` image (`T_IHPC_DropShadow`) |
| Title text | "Avatar Injury Prevention Technology", Avenir Heavy 24, uppercase, white | `TitleText`, Avenir Heavy 18 pt |
| Title glow | linear gradient `rgb(211,216,169)` 0 → 35% → 0 (12.5% / 51.9% / 89.9%), 576 × 54 | `TitleGlow` image (`T_IHPC_TitleGlow`) |
| Nav bar | `#274A65`, 88 tall, same shadow | `NavBarBackground` Border in `NavBarSize` (88) + `NavBarShadow` |
| Tab separators | 1 × 23, white 33%, ~32 px gap each side | `TabSeparator_1..4` images, padding 32 |
| Chevron circle (closed) | 83 Ø, `#1D3C55` @ 44% | `ChevronCircle` (rounded image), colour set by C++ |
| Chevron circle (open) | white @ 86% | colour set by C++ |
| Chevron arrow | 27 × 12, stroke 5, white ▼ (closed) / `#245983` ▲ (open) | `ChevronIcon`: `T_IHPC_HeaderChevron_Down` / `T_IHPC_HeaderChevron_Up_Blue` |

*Note:* the Figma frames include the iPad status bar inside the title bar, so the Figma title sits slightly low. In
Unreal there is no status bar, so the title is centred vertically in the 105 px bar.

## Nav tab (`WBP_IHPC_NavTab`) — Figma Default / Variant2

| State | Figma | Unreal |
|---|---|---|
| Default | Inter Medium 20, white | `NormalFont` Avenir Medium 15 pt, `NormalColor` white |
| Variant2 (hover / selected) | Inter Bold 24, `#B3CA7F` | `HighlightFont` Avenir Heavy 15 pt × `HighlightScale` 1.2, `HighlightColor` `#B3CA7F` |

The extra size comes from a render scale so the neighbouring tabs do not shift when one is hovered.

## Analysis panel (`WBP_IHPC_AnalysisPanel`, 1217 × 798, placed 167 from the top, centred)

| Element | Figma (layer) | Position / size inside the panel | Unreal widget |
|---|---|---|---|
| Panel | Rectangle 26, `rgba(0,0,0,0.62)`, radius 29 | 1217 × 798 | `PanelBackground` |
| Viewer frame | Rectangle 31, `rgba(0,0,0,0.29)`, r 11 | (25, 21) 870 × 559 | `ViewerFrame` |
| Viewer content | Rectangle 27 (MetaHuman picture — **not recreated**) | (47, 49) 827 × 508 | `ZoomedGraphContainer` (big graph, hidden until zoom) |
| Inspect icon | inspect-code 1, 33 × 33 | (835, 519) | `InspectButton` / `InspectIcon` |
| Videos area | Rectangle 34, `rgba(49,49,49,0.66)`, r 11 | (25, 599) 870 × 171 | `VideosArea` |
| Videos row | videos row instance | (31, 605) 858 × 160 | `VideosRow` |
| Component 3 | Component 3 instance | (910, 26) 286 wide | `GraphAccordion` |

## Component 3 (`WBP_IHPC_GraphAccordion`, 286 wide)

| Element | Figma | Unreal |
|---|---|---|
| Header | 286 × 47, `rgba(217,217,217,0.2)`, r 11; label Inter Bold 16 at x 22; chevron 15 × 9.5 at x 249 | `Header_*` buttons (hover 28%, pressed 34%), `Header_*_Label` Avenir Heavy 12 pt, `Chevron_*` |
| Spacing | header → header 13; header → content 7; content → next header 22 | Vertical Box slot paddings 13 / 7 / 9 |
| Content panel | 286 × 560, `rgba(0,0,0,0.41)`, r 11 | `Content_ShowGraphs` / `ShowGraphsPanel` |
| "Movement Graph" title | Inter Bold 16, centred, y ≈ 21 | `MovementGraphTitle` |
| Movement chart | 268 × 142 at (11, 45) | `MovementGraph` (`UCPP_UW_IHPCLineGraph`, seed 1) |
| "Active Muscles" title | y ≈ 209 | `ActiveMusclesTitle` |
| Active Muscles chart | 268 × 142 at (11, 233) | `ActiveMusclesGraph` (seed 8) |
| Toggles | Component 4 at (22, 397/429/461/493) | `Toggle_Bones/Nerves/Muscles/Tendons` |
| Legend bars | 5 × 18, r 43; `#1D277E` `#5D601E` `#3C2718` `#713D53` at x 70 | `Legend_*` images |
| Labels | Inter Bold 16 at x 80 | `Label_*` |
| Download | 30 × 30 at (214, 523) | `DownloadButton` / `T_IHPC_Download` |
| Zoom | 18 × 17 (20 × 19 with stroke) at (252, 529) | `ZoomButton` / `T_IHPC_Zoom` |

## Component 4 (`WBP_IHPC_ToggleSwitch`, 38 × 20)

| Variant | Figma | Unreal |
|---|---|---|
| Default (off) | track `#D9D9D9` @ 20%, r 10; knob white @ 65%, Ø 17 at x 10.5 | `T_IHPC_Toggle_Off` |
| Variant2 (on) | track `#B3CA7F` @ 76%; knob white @ 75% at x 27.5 | `T_IHPC_Toggle_On` |

## Videos row (`WBP_IHPC_VideosRow`, 858 × 160) and card (`WBP_IHPC_VideoCard`, 240 × 95)

| Element | Figma | Unreal |
|---|---|---|
| Scroll track | 842 × 15 at (3, 7), white 5%, radius 19 | `ScrollTrack` |
| Scroll thumb | 62 × 9, `rgba(153,191,222,0.51)`; Default at x 8, Variant2 at x 778 | `ScrollSlider` thumb (slider 832 wide at x 8, value 0 → 1) |
| Cards | ~240 wide, ~263 apart, tops at y ≈ 41 | `CardScrollBox` at (24, 41), `CardSpacing` 23 |
| Card art | `Rectangle 35` image (3 finished cards, text baked in) | `T_IHPC_VideoCard_BG` = the same art with text + tick removed |
| Title | bold, white, x 84 | `TitleText` Avenir Heavy 13 pt |
| Date / time | regular, white | `DateText`, `TimeText` Avenir Medium 10 pt |
| Severity tick | white, 3 × 13, on the green→red bar | `SeverityTick`, moved by `Severity × 237` px |

## Textures (`/Game/UserInterface/IHPC/Textures`)

| Texture | Made from | Used by |
|---|---|---|
| `T_IHPC_Toggle_Off` / `_On` | Component 4 SVG variants | ToggleSwitch |
| `T_IHPC_Chevron_Up` / `_Down` | accordion chevron SVG (15 × 9.5, stroke 2) | GraphAccordion headers |
| `T_IHPC_HeaderChevron_Down` / `_Up` / `_Up_Blue` | header "Vector 2" SVG (27 × 12, stroke 5) | HUD chevron |
| `T_IHPC_Zoom` | zoom "Vector" SVG | Zoom button |
| `T_IHPC_Download` | "pngtree-file-download-icon" PNG | Download button |
| `T_IHPC_InspectCode` | "inspect-code 1" PNG | Inspect button |
| `T_IHPC_VideoCard_BG` | "Rectangle 35" PNG, text removed | Video card |
| `T_IHPC_TitleGlow` | title glow gradient numbers | HUD title |
| `T_IHPC_DropShadow` | box-shadow numbers | HUD bar shadows |

All icons are rendered at 4× their Figma size so they stay sharp; the Widget Blueprints draw them at the Figma size.
