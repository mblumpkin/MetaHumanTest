# Changelog

Every step of the Figma → Unreal UI work, in order. Click a commit to see exactly what changed.
Repo: <https://github.com/mblumpkin/MetaHumanTest>

| # | Date | Commit | Step |
|---|---|---|---|
| 0 | 2026-11-06 | [`7556c1a`](https://github.com/mblumpkin/MetaHumanTest/commit/7556c1a) | Project put on GitHub (Git LFS for assets, Unreal `.gitignore`). Baseline before any UI work. |
| 1 | 2026-11-06 | [`9ff072c`](https://github.com/mblumpkin/MetaHumanTest/commit/9ff072c) | Figma assets downloaded to `SourceArt/Figma/Original`, reference screenshots to `SourceArt/Figma/Screens`, texture generator `Tools/FigmaUI/make_ui_textures.py`, generated PNGs in `SourceArt/Figma/UE`. |
| 2 | 2026-11-06 | [`d471dbc`](https://github.com/mblumpkin/MetaHumanTest/commit/d471dbc) | All C++ for the UI in `Source/MetaHumanTest/*/IHPC_UI` + `UMG`, `Slate`, `SlateCore` added to `Build.cs`. |
| 3 | 2026-11-06 | [`2575732`](https://github.com/mblumpkin/MetaHumanTest/commit/2575732) | Unreal Python import script `Tools/FigmaUI/ue_import_textures.py`. |
| 4 | 2026-11-06 | [`3e208b9`](https://github.com/mblumpkin/MetaHumanTest/commit/3e208b9) | C++ compiled (editor closed; *Succeeded*, no new warnings). 13 textures imported into `/Game/UserInterface/IHPC/Textures`. |
| 5 | 2026-11-07 | [`d80e956`](https://github.com/mblumpkin/MetaHumanTest/commit/d80e956) | 7 Widget Blueprints built in `/Game/UserInterface/IHPC/Widgets`; `Main` Level Blueprint now creates `WBP_IHPC_HUD`. Tested in Play-In-Editor; fixed the invisible selected-card outline. *(Main.umap also includes level edits saved before the editor was closed for the build.)* |
| 6 | 2026-11-07 | [`69b217c`](https://github.com/mblumpkin/MetaHumanTest/commit/69b217c) | `Tools/FigmaUI/build_widget_blueprints.py` — record of exactly how each Widget Blueprint was built. |
| 7 | 2026-11-07 | [`400a763`](https://github.com/mblumpkin/MetaHumanTest/commit/400a763) | `README/` folder: overview, Figma → Unreal map, code walkthrough, Widget Blueprint trees, how-tos, pipeline, limitations, screenshots. |
| 8 | 2026-11-07 | [`e5cfc4c`](https://github.com/mblumpkin/MetaHumanTest/commit/e5cfc4c) | README links to the checklist [issue #1](https://github.com/mblumpkin/MetaHumanTest/issues/1). |

## Step 2 — timeline / playback bar ([issue #2](https://github.com/mblumpkin/MetaHumanTest/issues/2))

| # | Date | Commit | Step |
|---|---|---|---|
| 9 | 2026-11-07 | [`f8b3479`](https://github.com/mblumpkin/MetaHumanTest/commit/f8b3479) | Figma layer `ihpc ui (1)ee 1` downloaded; `make_timeline_assets()` cuts the 8 buttons + knob and draws the marker → `T_IHPC_TL_*.png`. |
| 10 | 2026-11-07 | [`4fe9713`](https://github.com/mblumpkin/MetaHumanTest/commit/4fe9713) | C++: `CPP_UW_IHPCTimeline`; library play/pause/speed/date helpers; `AddTaggedPoint` / `AddSavedVideo` / `GetSavedVideos`; HUD wiring. |
| 11 | 2026-11-07 | [`8750fe7`](https://github.com/mblumpkin/MetaHumanTest/commit/8750fe7) | Built (editor closed; *Succeeded*); 10 timeline textures imported. |
| 12 | 2026-11-07 | [`59d2883`](https://github.com/mblumpkin/MetaHumanTest/commit/59d2883) | `WBP_IHPC_Timeline` built and added to `WBP_IHPC_HUD`; `BP_PlayerController` no longer creates the prototype `WB_AmationTimeline`. **Fix:** removed Git LFS `lockable` (it made committed assets read-only). |
| 13 | 2026-11-07 | [`af548f6`](https://github.com/mblumpkin/MetaHumanTest/commit/af548f6) | **Fix:** colour brushes no longer reference engine pictures (track was ~70% too dark; also the videos-row scroll thumb). Tested every timeline button in Play-In-Editor. |
| 14 | 2026-11-07 | [`d7ebd72`](https://github.com/mblumpkin/MetaHumanTest/commit/d7ebd72) | README pages updated for the timeline; `build_widget_blueprints.py` records `build_timeline()` and the HUD/controller change. |
| 15 | 2026-11-07 | [`31e0535`](https://github.com/mblumpkin/MetaHumanTest/commit/31e0535) | `CPP_PlayerController`: `MainHUDWidgetClass`, `MainHUDZOrder` (10), `GetMainHUD()`; `BeginPlay` creates the HUD like the other widgets. |
| 16 | 2026-11-07 | [`a00ce78`](https://github.com/mblumpkin/MetaHumanTest/commit/a00ce78) | Built; `BP_PlayerController` → *Main HUD Widget Class* = `WBP_IHPC_HUD`; HUD creation removed from the `Main` Level Blueprint. Tested: one HUD, menu, panel and timeline work. |
| 17 | 2026-11-07 | [`e574b41`](https://github.com/mblumpkin/MetaHumanTest/commit/e574b41) | README / build record updated for the controller-created HUD; [issue #2](https://github.com/mblumpkin/MetaHumanTest/issues/2) created. |
| 18 | 2026-11-07 | [`50de3ec`](https://github.com/mblumpkin/MetaHumanTest/commit/50de3ec) | **Fix:** `Main` Level Blueprint compiler error — Begin Play still had a wire to the deleted Create Widget node; wire removed, level recompiles cleanly; Play-In-Editor checked (one HUD, no new errors). |
| 19 | 2026-11-07 | [`019ae94`](https://github.com/mblumpkin/MetaHumanTest/commit/019ae94) | **Fix:** pixelated timeline icons — all 23 UI textures now have mipmaps (Simple Average), trilinear filtering, never stream; `ue_import_textures.py` updated to match. |
| 20 | 2026-11-07 | [`29158cb`](https://github.com/mblumpkin/MetaHumanTest/commit/29158cb) | `WBP_IHPC_Timeline` edited by the team in the editor. |

## Step 3 — layer switcher / opacity buttons ([issue #3](https://github.com/mblumpkin/MetaHumanTest/issues/3))

| # | Date | Commit | Step |
|---|---|---|---|
| 21 | 2026-11-07 | [`91804a2`](https://github.com/mblumpkin/MetaHumanTest/commit/91804a2) | Figma layers `ihpc ui rttd 2` + `Rectangle 24` downloaded (frames 1 and 4); `make_layer_switcher_assets()` → `T_IHPC_LS_*.png` (Skeleton-selected look rebuilt, badge separated). |
| 22 | 2026-11-07 | [`5279a5e`](https://github.com/mblumpkin/MetaHumanTest/commit/5279a5e) | C++: `CPP_UW_IHPCLayerSwitcher` (child of the teammate's `CPP_UW_AnatomyViewSwitcher`, unchanged) + `EIHPCAnatomyLayer`; build steps recorded in `build_widget_blueprints.py`. |
| 23 | 2026-11-07 | [`1f32f00`](https://github.com/mblumpkin/MetaHumanTest/commit/1f32f00) | Built (*Succeeded*); 7 textures imported; `WBP_IHPC_LayerSwitcher` built; `BP_PlayerController` creates it instead of the top-left placeholder; timeline moved 125 px left (Figma). Tested Skeleton / Muscles / Body in Play-In-Editor. |
| 24 | 2026-11-07 | [`5e73757`](https://github.com/mblumpkin/MetaHumanTest/commit/5e73757) | README pages 01–07 updated for the layer switcher; screenshots; [issue #3](https://github.com/mblumpkin/MetaHumanTest/issues/3) created. |
