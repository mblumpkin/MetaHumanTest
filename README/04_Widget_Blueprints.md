# 04 — Widget Blueprints

Folder: `Content/UserInterface/IHPC/Widgets/` (`/Game/UserInterface/IHPC/Widgets`).

**Rule:** names marked **(C++)** are used by the C++ parent class (`BindWidget`). You can move, restyle, resize or wrap
them, but **do not rename or delete them** — the Blueprint will refuse to compile and tell you which one is missing.
Everything else is free to change.

Parent class = the C++ class in [03_Code_Walkthrough.md](03_Code_Walkthrough.md). Class Defaults (the settings in the
Details panel when nothing is selected) hold the textures/fonts listed at the bottom of each section.

---

## WBP_IHPC_HUD — parent `CPP_UW_IHPCHud`
Full screen. Created by the **Main** level's Level Blueprint (Begin Play → Create Widget → Add to Viewport, Z-order 10).

```
HudCanvas (Canvas Panel, not hit-testable itself)
├─ AnalysisPanel (C++)          WBP_IHPC_AnalysisPanel, top-centre, y 167, 1217×798, hidden until Saved Videos
├─ ChevronGroup (C++)           Size Box 83×83, centre at (screen centre, 105) — moved down 88 when the menu opens
│  └─ ChevronOverlay
│     ├─ ChevronCircle (C++)    rounded image, colour set by C++
│     ├─ ChevronIcon (C++)      arrow texture, swapped by C++
│     └─ ChevronButton (C++)    invisible button over the circle
├─ NavBar (C++)                 Vertical Box, y 105, full width — collapsed until opened
│  ├─ NavBarSize (88 tall)
│  │  └─ NavBarBackground       #274A65
│  │     └─ TabRow (Horizontal Box, centred)
│  │        ├─ Tab_Profile (C++)          WBP_IHPC_NavTab
│  │        ├─ TabSeparator_1             1×23 white 33%, 32 px each side
│  │        ├─ Tab_Progress (C++)
│  │        ├─ TabSeparator_2
│  │        ├─ Tab_SavedVideos (C++)
│  │        ├─ TabSeparator_3
│  │        ├─ Tab_HistoryTimeline (C++)
│  │        ├─ TabSeparator_4
│  │        └─ Tab_Settings (C++)
│  └─ NavBarShadow              T_IHPC_DropShadow
└─ TitleBar                     Vertical Box, full width
   ├─ TitleBarSize (105 tall)
   │  └─ TitleBarOverlay
   │     ├─ TitleBarBackground  #1D3C55
   │     ├─ TitleGlow           T_IHPC_TitleGlow 576×54
   │     └─ TitleText           "AVATAR INJURY PREVENTION TECHNOLOGY", Avenir Heavy 18
   └─ TitleBarShadow            T_IHPC_DropShadow
```
Drawing order = list order (title bar on top, so the circle's top half hides behind it).
Class Defaults: `ChevronClosedTexture` = `T_IHPC_HeaderChevron_Down`, `ChevronOpenTexture` = `T_IHPC_HeaderChevron_Up_Blue`,
`AnalysisPanelTab` = Saved Videos, `NavBarHeight` = 88.

## WBP_IHPC_NavTab — parent `CPP_UW_IHPCNavTab`
```
TabButton (C++)       Button, invisible in every state
└─ TabLabel (C++)     TextBlock, Avenir Medium 15, centred
```
Class Defaults: `NormalFont` Avenir Medium 15, `HighlightFont` Avenir Heavy 15, `HighlightScale` 1.2,
`NormalColor` white, `HighlightColor` #B3CA7F. The HUD fills in each tab's `TabId` and `Label`.

## WBP_IHPC_AnalysisPanel — parent `CPP_UW_IHPCAnalysisPanel`
```
PanelSize (1217×798)
└─ PanelBackground            rgba(0,0,0,0.62), radius 29 (blocks clicks to the 3D view while open)
   └─ PanelCanvas
      ├─ ViewerFrame          (25,21) 870×559, rgba(0,0,0,0.29) r11
      ├─ ZoomedGraphContainer (C++, optional)  (47,49) 827×508 — hidden until zoom
      │  └─ ZoomedGraphStack
      │     ├─ ZoomedGraphTitle   "Movement Graph"
      │     └─ ZoomedGraph (C++, optional)  CPP_UW_IHPCLineGraph
      ├─ InspectButton (C++)  (835,519) 33×33
      │  └─ InspectIcon       T_IHPC_InspectCode
      ├─ VideosArea           (25,599) 870×171, rgba(49,49,49,0.66) r11
      ├─ VideosRow (C++)      WBP_IHPC_VideosRow (31,605) 858×160
      └─ GraphAccordion (C++) WBP_IHPC_GraphAccordion (910,26), auto height
```

## WBP_IHPC_GraphAccordion — parent `CPP_UW_IHPCGraphAccordion`
```
AccordionSize (286 wide)
└─ Sections (Vertical Box)
   ├─ Header_ShowGraphs (C++)          Button 47 tall, rgba(217,217,217,0.2) r11
   │  └─ … _Label "Show graphs" + Chevron_ShowGraphs (C++)
   ├─ Content_ShowGraphs (C++)         Size Box 560 tall (gap 7 above, 9 below)
   │  └─ ShowGraphsPanel               rgba(0,0,0,0.41) r11
   │     └─ ShowGraphsCanvas
   │        ├─ MovementGraphTitle      "Movement Graph"
   │        ├─ MovementGraph (C++)     (11,45) 268×142, DemoSeed 1
   │        ├─ ActiveMusclesTitle      "Active Muscles"
   │        ├─ ActiveMusclesGraph (C++) (11,233) 268×142, DemoSeed 8
   │        ├─ Toggle_Bones / Toggle_Nerves / Toggle_Muscles / Toggle_Tendons (C++)  WBP_IHPC_ToggleSwitch at x 22
   │        ├─ Legend_* (5×18 colour bars at x 70)   Label_* (text at x 80)
   │        ├─ DownloadButton (C++) → DownloadIcon   (214,523) 30×30
   │        └─ ZoomButton (C++) → ZoomIcon           (251,528) 20×19
   ├─ Header_ActiveMuscles (C++) … + Chevron_ActiveMuscles (C++)   (13 above)
   ├─ Content_ActiveMuscles (C++)      Border rgba(0,0,0,0.41) r11, padding 8
   │  └─ ActiveMuscles_ListSize (max 260) → ActiveMuscles_Scroll → ActiveMusclesList (C++)
   ├─ Header_TaggedPoints (C++) … + Chevron_TaggedPoints (C++)
   └─ Content_TaggedPoints (C++)
      └─ TaggedPoints_ListSize → TaggedPoints_Scroll → TaggedPointsList (C++)
```
Class Defaults: `ChevronUpTexture` / `ChevronDownTexture`, `ListFont` Avenir Medium 11, `StartOpenSection` None,
`ActiveMuscles` (sample names).

## WBP_IHPC_ToggleSwitch — parent `CPP_UW_IHPCToggleSwitch`
```
SwitchSize (38×20)
└─ SwitchCheckBox (C++)   style: unchecked = T_IHPC_Toggle_Off, checked = T_IHPC_Toggle_On (hover slightly brighter)
```

## WBP_IHPC_VideosRow — parent `CPP_UW_IHPCVideosRow`
```
RowCanvas
├─ ScrollTrack            (3,7) 842×15, white 5%, fully rounded
├─ ScrollSlider (C++)     (8,10) 832×9, no bar, thumb 62×9 rgba(153,191,222,0.51), Indent Handle on
└─ CardScrollBox (C++)    (24,41) 834×119, horizontal, no scrollbar — C++ adds the cards here
```
Class Defaults: `CardClass` = `WBP_IHPC_VideoCard`, `CardSpacing` 23, `Moments` = the 5 sample cards.

## WBP_IHPC_VideoCard — parent `CPP_UW_IHPCVideoCard`
```
CardSize (240×95)
└─ CardOverlay
   ├─ CardBackground          T_IHPC_VideoCard_BG (Figma card art without text)
   ├─ CardContent (Canvas, click-through)
   │  ├─ TitleText (C++)      (84,21) Avenir Heavy 13, ends with "…" if too long
   │  ├─ DateText (C++)       (84,47) Avenir Medium 10
   │  ├─ TimeText (C++)       (84,63) Avenir Medium 10
   │  ├─ SeverityTick (C++)   (0,0) 3×13 white — moved sideways by C++
   │  └─ SelectionHighlight (C++, optional)  green #B3CA7F outline, hidden until selected
   └─ CardButton (C++)        invisible button over the whole card (faint white on hover)
```
