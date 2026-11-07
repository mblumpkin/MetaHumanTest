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
| 7 | 2026-11-07 | *(this commit)* | `README/` folder: overview, Figma → Unreal map, code walkthrough, Widget Blueprint trees, how-tos, pipeline, limitations, screenshots. |
