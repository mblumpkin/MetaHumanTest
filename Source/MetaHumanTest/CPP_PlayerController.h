// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_AbbyCharacter.h"
#include "CPP_MouseFunctionComp.h"
#include "CPP_UW_AnatomyViewSwitcher.h"
#include "CPP_UW_PawnSwitcher.h"
#include "CPP_UW_SpeedSlider.h"
#include "IHPC_UI/CPP_UW_IHPCHud.h"
#include "InputMappingContext.h"
#include "InputActionValue.h"   // add this
#include "GameFramework/PlayerController.h"
#include "CPP_PlayerController.generated.h"

/**
 * 
 */


class UWCPP_AnimationTimeline;

UCLASS()
class METAHUMANTEST_API ACPP_PlayerController : public APlayerController
{
	GENERATED_BODY()



	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere,Category="Input")
	UInputMappingContext* MouseManager;


	UPROPERTY(EditAnywhere,Category="Input")
	UInputAction* MouseMove;


	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseZoom;

	void HandleMouseMove(const FInputActionValue& Value);

	void HandleMouseZoom(const FInputActionValue& Value);



	ACPP_AbbyCharacter* AbbyCharacterPawn;
	UCPP_MouseFunctionComp* GetMouseFunctionComp();

public:

	// access the meta human // need to be attached on bp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MetaHuman")
	AActor* TargetMetaHuman = nullptr;

	// This is ganna get the data of the Player Placeholder manager from the map, in the begin play
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MetaHuman")
	TArray<AActor*> PC_PlayerPlaceHolders;

	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UI")
	TSubclassOf<UCPP_UW_SpeedSlider> SpeedSliderWidget;

	// Set this to the BP widget in the editor, this ui is for the new slider
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="UI")
	TSubclassOf<UWCPP_AnimationTimeline> TimelineWidgetClass;

	
		
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UI")
	TSubclassOf<UCPP_UW_AnatomyViewSwitcher> AnatomyViewSwitcherWidgetClass;

		
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UI")
	TSubclassOf<UCPP_UW_PawnSwitcher> PawnSwitcherWidgetClass;

	// IHPC HUD from the Figma design (title bar, menu, analysis panel, timeline bar).
	// Set this to WBP_IHPC_HUD in BP_PlayerController > Class Defaults.
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UI")
	TSubclassOf<UCPP_UW_IHPCHud> MainHUDWidgetClass;

	// Draw order of the HUD: higher = on top. 10 keeps it above the other widgets (they use 0).
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UI")
	int32 MainHUDZOrder = 10;

	// The HUD that was created in BeginPlay, so the controller (or Blueprint) can reach it later,
	// e.g. GetMainHUD()->SelectTab(EIHPCNavTab::SavedVideos) to open the analysis panel from a key press.
	UFUNCTION(BlueprintPure, Category="UI")
	UCPP_UW_IHPCHud* GetMainHUD() const { return MainHUDWidget; }


private:
	UPROPERTY()
	UCPP_UW_SpeedSlider* SpeedSliderWidgetInstance{};



	// this ui is for the new slider
	UPROPERTY()
	UWCPP_AnimationTimeline* TimelineWidget = nullptr;

	// this ui is for the new slider
	UPROPERTY()
	UCPP_UW_AnatomyViewSwitcher* AnatomyViewSwitcherWidget = nullptr;


	UPROPERTY()
	UCPP_UW_PawnSwitcher* PawnSwitcherWidget = nullptr;

	// IHPC HUD instance (see MainHUDWidgetClass)
	UPROPERTY()
	UCPP_UW_IHPCHud* MainHUDWidget = nullptr;


	
	

	
	
};
