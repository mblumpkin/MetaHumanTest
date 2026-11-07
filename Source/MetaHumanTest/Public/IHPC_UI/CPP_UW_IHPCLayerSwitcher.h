// CPP_UW_IHPCLayerSwitcher.h
// -----------------------------------------------------------------------------
// The LAYER SWITCHER (opacity buttons) at the bottom right of the screen.
// Figma (frame "iPad Pro 12.9" - 1"):
//   * "ihpc ui rttd 2" - three glossy round buttons: Body (layers icon), Muscles, Skeleton.
//                        Selected = dark navy circle + white icon, unselected = light blue + navy icon.
//                        The skeleton button has a small orange badge.
//   * "Rectangle 24"   - the see-through pill behind them (#1D3C55 at 26 %, 204 x 65, radius 35.5).
//
// It REPLACES the top-left placeholder "WB_AnatomyViewSwitcher" (MetaHuman / Muscle / Skeleton buttons + slider)
// and keeps its functionality: this class is a CHILD of the teammate's UCPP_UW_AnatomyViewSwitcher, so all of
// the existing opacity code runs unchanged (finding the parts tagged WholePart / SkeletonPart / FacePart /
// HairPart on the pawn, lining the skeleton up with the body, setting the "Fade" material parameter, and
// swapping the hair from strands to cards below 0.9). This class only adds the new look and decides which
// "Fade" value each button asks for. It talks to the parent through the parent's slider (kept hidden in the
// Widget Blueprint) - exactly what dragging the old placeholder slider did.
//
// The real layers (muscle mesh etc.) are being built by another team member. When they are ready they can:
//   * change the Fade value of each button (Class Defaults -> IHPC | Layers), and/or
//   * listen to "On Layer Changed" (Blueprint or C++) and switch meshes there.
//
// Widget Blueprint: /Game/UserInterface/IHPC/Widgets/WBP_IHPC_LayerSwitcher
//   Needs Buttons "BodyLayer_Btn", "MusclesLayer_Btn", "SkeletonLayer_Btn", optional Image "SkeletonBadge",
//   and (for the parent class) a Button "AnatomyMetaHuman_Btn" and a Slider "AnatomyViewSwitcher_Slider",
//   both Collapsed.
// Created by CPP_PlayerController (BP_PlayerController -> Anatomy View Switcher Widget Class).
// -----------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "CPP_UW_AnatomyViewSwitcher.h"
#include "IHPC_UI/IHPCTypes.h"
#include "CPP_UW_IHPCLayerSwitcher.generated.h"

class UButton;
class UImage;
class UTexture2D;

/** Fired when a layer button is clicked. Fade = the value sent to the avatar's "Fade" material parameter. */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FIHPCLayerChanged, EIHPCAnatomyLayer, Layer, float, Fade);

UCLASS(Abstract)
class METAHUMANTEST_API UCPP_UW_IHPCLayerSwitcher : public UCPP_UW_AnatomyViewSwitcher
{
	GENERATED_BODY()

public:
	// ---- Layers: what each button does ------------------------------------------------------------

	/** Button that looks selected when the game starts (Figma frame 1: Body). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IHPC|Layers")
	EIHPCAnatomyLayer DefaultLayer = EIHPCAnatomyLayer::Body;

	/** Also APPLY the default layer's Fade when the game starts. Off = the avatar starts as it is in the level
	 *  (like the old placeholder, which did nothing until clicked). */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IHPC|Layers")
	bool bApplyDefaultLayerOnStart = false;

	/** Fade for the Body button. 1 = body fully visible (hair back to strands). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IHPC|Layers", meta = (ClampMin = "0", ClampMax = "1"))
	float BodyFade = 1.0f;

	/** Fade for the Muscles button. There is no muscle layer yet, so for now it makes the body half see-through;
	 *  change it (or use On Layer Changed) once the muscle layer exists. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IHPC|Layers", meta = (ClampMin = "0", ClampMax = "1"))
	float MusclesFade = 0.5f;

	/** Fade for the Skeleton button. 0.2 = what the placeholder's MetaHuman button did (body faded, skeleton shows). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IHPC|Layers", meta = (ClampMin = "0", ClampMax = "1"))
	float SkeletonFade = 0.2f;

	// ---- Look (textures made from the Figma pictures by Tools/FigmaUI/make_ui_textures.py) ----------

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IHPC|Style")
	TObjectPtr<UTexture2D> BodySelectedTexture;          // T_IHPC_LS_Body_On

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IHPC|Style")
	TObjectPtr<UTexture2D> BodyUnselectedTexture;        // T_IHPC_LS_Body_Off

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IHPC|Style")
	TObjectPtr<UTexture2D> MusclesSelectedTexture;       // T_IHPC_LS_Muscles_On

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IHPC|Style")
	TObjectPtr<UTexture2D> MusclesUnselectedTexture;     // T_IHPC_LS_Muscles_Off

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IHPC|Style")
	TObjectPtr<UTexture2D> SkeletonSelectedTexture;      // T_IHPC_LS_Skeleton_On (not drawn in Figma, rebuilt)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IHPC|Style")
	TObjectPtr<UTexture2D> SkeletonUnselectedTexture;    // T_IHPC_LS_Skeleton_Off

	/** Size each round button is drawn at (Figma: 46 x 46). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IHPC|Style")
	FVector2D ButtonSize = FVector2D(46.f, 46.f);

	/** Brightness while the mouse is over a button / while it is pressed (same as the timeline buttons). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IHPC|Style")
	float HoveredTint = 1.15f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IHPC|Style")
	float PressedTint = 0.8f;

	/** Show the small orange badge on the skeleton button (Figma shows it). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IHPC|Style")
	bool bShowSkeletonBadge = true;

	// ---- Events / calls --------------------------------------------------------------------------

	/** Broadcast every time a layer button is clicked (also when the avatar has no taggable parts). */
	UPROPERTY(BlueprintAssignable, Category = "IHPC|Events")
	FIHPCLayerChanged OnLayerChanged;

	/** Same as clicking a button: highlights it and applies its Fade to the avatar. */
	UFUNCTION(BlueprintCallable, Category = "IHPC|Layers")
	void SelectLayer(EIHPCAnatomyLayer Layer);

	UFUNCTION(BlueprintPure, Category = "IHPC|Layers")
	EIHPCAnatomyLayer GetSelectedLayer() const { return SelectedLayer; }

	UFUNCTION(BlueprintPure, Category = "IHPC|Layers")
	float GetFadeForLayer(EIHPCAnatomyLayer Layer) const;

	UFUNCTION(BlueprintCallable, Category = "IHPC|Layers")
	void SetSkeletonBadgeVisible(bool bVisible);

	/** True when the current pawn has the parts the opacity code needs (skeletal meshes tagged WholePart,
	 *  SkeletonPart and FacePart). Without them the parent code would crash, so the buttons then only change
	 *  the highlight and fire On Layer Changed. */
	UFUNCTION(BlueprintPure, Category = "IHPC|Layers")
	bool CanDriveAvatarLayers() const;

protected:
	virtual void NativePreConstruct() override;   // runs in the UMG designer too, so the buttons show their look
	virtual void NativeConstruct() override;      // in game: parent binds its slider/button, we bind ours

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BodyLayer_Btn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> MusclesLayer_Btn;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> SkeletonLayer_Btn;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UImage> SkeletonBadge;

private:
	UFUNCTION()
	void HandleBodyClicked();

	UFUNCTION()
	void HandleMusclesClicked();

	UFUNCTION()
	void HandleSkeletonClicked();

	/** Sends the Fade value to the parent's opacity code (through its hidden slider). */
	void ApplyFadeToAvatar(float Fade);

	/** Gives every button its selected / unselected picture and shows or hides the badge. */
	void RefreshVisuals();

	void ApplyButtonLook(UButton* Button, UTexture2D* Texture) const;

	EIHPCAnatomyLayer SelectedLayer = EIHPCAnatomyLayer::Body;

	bool bWarnedMissingParts = false;
};
