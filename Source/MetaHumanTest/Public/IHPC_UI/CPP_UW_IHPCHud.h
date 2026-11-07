// CPP_UW_IHPCHud.h
// -----------------------------------------------------------------------------
// The full-screen IHPC HUD - the top of the screen in every Figma iPad frame.
//
//   +-------------------------------------------------------------------+
//   |              AVATAR INJURY PREVENTION TECHNOLOGY                    |  title bar (#1D3C55, 105 px)
//   +-------------------------------------------------------------------+
//   |  Profile | Progress | Saved Videos | History & Timeline | Settings  |  nav bar (#274A65, 88 px)
//   +-------------------------------------------------------------------+     (hidden until opened)
//                                (v)                                        chevron button
//
//   * Chevron button: opens / closes the nav bar (Figma frame 1 = closed, frame 3 = open).
//     Closed: dark see-through circle + white down arrow. Open: white circle + blue up arrow.
//   * Tabs: clicking a tab selects it (green highlight). Clicking it again unselects it.
//   * "Saved Videos" opens the analysis panel (Figma frame 5) and closes the nav bar,
//     because that frame shows the panel with the nav bar closed. Change AnalysisPanelTab
//     to make a different tab open the panel.
//   * Profile / Progress / History & Timeline / Settings have no screens in the Figma
//     components yet: they highlight and fire OnTabChanged so screens can be added later.
//
// Clicks on empty parts of the HUD pass straight through to the 3D view, so the
// existing mouse orbit / pan / zoom keeps working.
//
// Widget Blueprint: /Game/UserInterface/IHPC/Widgets/WBP_IHPC_HUD
// Created by the Main level's Level Blueprint (BeginPlay -> Create Widget -> Add to Viewport).
// -----------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IHPC_UI/IHPCTypes.h"
#include "CPP_UW_IHPCHud.generated.h"

class UButton;
class UImage;
class UTexture2D;
class UCPP_UW_IHPCNavTab;
class UCPP_UW_IHPCAnalysisPanel;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FIHPCTabChanged, EIHPCNavTab, NewTab);

UCLASS(Abstract)
class METAHUMANTEST_API UCPP_UW_IHPCHud : public UUserWidget
{
	GENERATED_BODY()

public:
	UCPP_UW_IHPCHud(const FObjectInitializer& ObjectInitializer);

	// ---------------------------------------------------------------- settings

	/** Which tab opens the analysis panel. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IHPC|Behaviour")
	EIHPCNavTab AnalysisPanelTab = EIHPCNavTab::SavedVideos;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IHPC|Behaviour")
	bool bStartWithNavBarOpen = false;

	/** Height of the nav bar (Figma: 88 px). The chevron moves down by this much when it opens. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IHPC|Layout")
	float NavBarHeight = 88.f;

	/** Arrow shown while the nav bar is closed (white, pointing down). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IHPC|Style")
	TObjectPtr<UTexture2D> ChevronClosedTexture;

	/** Arrow shown while the nav bar is open (blue #245983, pointing up). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IHPC|Style")
	TObjectPtr<UTexture2D> ChevronOpenTexture;

	/** Circle colour while closed: #1D3C55 at 44% (Figma frame 1). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IHPC|Style")
	FLinearColor ChevronCircleClosedColor;

	/** Circle colour while open: white at 86% (Figma frame 3). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IHPC|Style")
	FLinearColor ChevronCircleOpenColor;

	// ---------------------------------------------------------------- events

	UPROPERTY(BlueprintAssignable, Category = "IHPC|Events")
	FIHPCTabChanged OnTabChanged;

	// ---------------------------------------------------------------- functions

	UFUNCTION(BlueprintCallable, Category = "IHPC|HUD")
	void SetNavBarOpen(bool bOpen);

	UFUNCTION(BlueprintCallable, Category = "IHPC|HUD")
	void ToggleNavBar() { SetNavBarOpen(!bNavBarOpen); }

	UFUNCTION(BlueprintPure, Category = "IHPC|HUD")
	bool IsNavBarOpen() const { return bNavBarOpen; }

	/** Selects a tab (None = unselect all) and opens/closes the analysis panel to match. */
	UFUNCTION(BlueprintCallable, Category = "IHPC|HUD")
	void SelectTab(EIHPCNavTab Tab);

	UFUNCTION(BlueprintPure, Category = "IHPC|HUD")
	EIHPCNavTab GetSelectedTab() const { return SelectedTab; }

	UFUNCTION(BlueprintCallable, Category = "IHPC|HUD")
	void SetAnalysisPanelOpen(bool bOpen);

protected:
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	/** The nav bar strip (with the tabs inside). Shown/hidden by the chevron. */
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UWidget> NavBar;

	/** Circle + arrow + button together. Moved down by NavBarHeight when the nav bar opens. */
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UWidget> ChevronGroup;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UButton> ChevronButton;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UImage> ChevronCircle;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UImage> ChevronIcon;

	UPROPERTY(meta = (BindWidget)) TObjectPtr<UCPP_UW_IHPCNavTab> Tab_Profile;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UCPP_UW_IHPCNavTab> Tab_Progress;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UCPP_UW_IHPCNavTab> Tab_SavedVideos;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UCPP_UW_IHPCNavTab> Tab_HistoryTimeline;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UCPP_UW_IHPCNavTab> Tab_Settings;

	UPROPERTY(meta = (BindWidgetOptional)) TObjectPtr<UCPP_UW_IHPCAnalysisPanel> AnalysisPanel;

private:
	UFUNCTION() void HandleChevronClicked();
	UFUNCTION() void HandleTabClicked(EIHPCNavTab Tab);

	/** Updates the circle colour, arrow picture and position for the current open/closed state. */
	void RefreshChevron();

	TArray<UCPP_UW_IHPCNavTab*> GetTabs() const;

	bool bNavBarOpen = false;
	EIHPCNavTab SelectedTab = EIHPCNavTab::None;
};
