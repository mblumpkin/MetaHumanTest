// CPP_UW_IHPCLayerSwitcher.cpp - the Body / Muscles / Skeleton buttons. See the header for the Figma mapping.

#include "IHPC_UI/CPP_UW_IHPCLayerSwitcher.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/Slider.h"
#include "Engine/Texture2D.h"
#include "GameFramework/Pawn.h"

void UCPP_UW_IHPCLayerSwitcher::NativePreConstruct()
{
	Super::NativePreConstruct();

	SelectedLayer = DefaultLayer;
	RefreshVisuals();
}

void UCPP_UW_IHPCLayerSwitcher::NativeConstruct()
{
	// The parent hooks its slider -> OnSliderValueChanged2 and its button -> OnClick_SpawnAndPossess here.
	Super::NativeConstruct();

	if (BodyLayer_Btn)
	{
		BodyLayer_Btn->OnClicked.AddUniqueDynamic(this, &UCPP_UW_IHPCLayerSwitcher::HandleBodyClicked);
	}
	if (MusclesLayer_Btn)
	{
		MusclesLayer_Btn->OnClicked.AddUniqueDynamic(this, &UCPP_UW_IHPCLayerSwitcher::HandleMusclesClicked);
	}
	if (SkeletonLayer_Btn)
	{
		SkeletonLayer_Btn->OnClicked.AddUniqueDynamic(this, &UCPP_UW_IHPCLayerSwitcher::HandleSkeletonClicked);
	}

	SelectedLayer = DefaultLayer;
	RefreshVisuals();

	if (bApplyDefaultLayerOnStart)
	{
		ApplyFadeToAvatar(GetFadeForLayer(DefaultLayer));
	}
}

void UCPP_UW_IHPCLayerSwitcher::HandleBodyClicked()     { SelectLayer(EIHPCAnatomyLayer::Body); }
void UCPP_UW_IHPCLayerSwitcher::HandleMusclesClicked()  { SelectLayer(EIHPCAnatomyLayer::Muscles); }
void UCPP_UW_IHPCLayerSwitcher::HandleSkeletonClicked() { SelectLayer(EIHPCAnatomyLayer::Skeleton); }

void UCPP_UW_IHPCLayerSwitcher::SelectLayer(EIHPCAnatomyLayer Layer)
{
	SelectedLayer = Layer;
	RefreshVisuals();

	const float Fade = GetFadeForLayer(Layer);
	ApplyFadeToAvatar(Fade);
	OnLayerChanged.Broadcast(Layer, Fade);
}

float UCPP_UW_IHPCLayerSwitcher::GetFadeForLayer(EIHPCAnatomyLayer Layer) const
{
	switch (Layer)
	{
	case EIHPCAnatomyLayer::Muscles:  return MusclesFade;
	case EIHPCAnatomyLayer::Skeleton: return SkeletonFade;
	case EIHPCAnatomyLayer::Body:
	default:                          return BodyFade;
	}
}

bool UCPP_UW_IHPCLayerSwitcher::CanDriveAvatarLayers() const
{
	const APawn* Pawn = GetOwningPlayerPawn();
	if (!Pawn)
	{
		return false;
	}

	// The parent's code uses these three parts without checking for null, and prints each tagged mesh's
	// asset name, so every tagged mesh must also have a mesh asset.
	bool bWhole = false, bSkeleton = false, bFace = false;
	TArray<USkeletalMeshComponent*> Meshes;
	Pawn->GetComponents<USkeletalMeshComponent>(Meshes);
	for (const USkeletalMeshComponent* Mesh : Meshes)
	{
		if (!Mesh)
		{
			continue;
		}
		const bool bTagged = Mesh->ComponentHasTag(TEXT("WholePart")) || Mesh->ComponentHasTag(TEXT("SkeletonPart"))
			|| Mesh->ComponentHasTag(TEXT("FacePart"));
		if (bTagged && !Mesh->GetSkeletalMeshAsset())
		{
			return false;
		}
		bWhole    |= Mesh->ComponentHasTag(TEXT("WholePart"));
		bSkeleton |= Mesh->ComponentHasTag(TEXT("SkeletonPart"));
		bFace     |= Mesh->ComponentHasTag(TEXT("FacePart"));
	}
	return bWhole && bSkeleton && bFace;
}

void UCPP_UW_IHPCLayerSwitcher::ApplyFadeToAvatar(float Fade)
{
	if (!AnatomyViewSwitcher_Slider)
	{
		return;
	}
	if (!CanDriveAvatarLayers())
	{
		if (!bWarnedMissingParts)
		{
			bWarnedMissingParts = true;
			UE_LOG(LogTemp, Warning, TEXT("[IHPC] Layer switcher: the current pawn has no skeletal meshes tagged "
				"WholePart / SkeletonPart / FacePart, so the opacity was not changed (only the buttons)."));
		}
		return;
	}

	// Same as dragging the old placeholder slider to this value: the parent's OnSliderValueChanged2 runs.
	// USlider::SetValue only notifies when the value changes, so re-send it when it is already there
	// (e.g. after switching pawns).
	if (FMath::IsNearlyEqual(AnatomyViewSwitcher_Slider->GetValue(), Fade))
	{
		AnatomyViewSwitcher_Slider->OnValueChanged.Broadcast(Fade);
	}
	else
	{
		AnatomyViewSwitcher_Slider->SetValue(Fade);
	}
}

void UCPP_UW_IHPCLayerSwitcher::SetSkeletonBadgeVisible(bool bVisible)
{
	bShowSkeletonBadge = bVisible;
	RefreshVisuals();
}

void UCPP_UW_IHPCLayerSwitcher::RefreshVisuals()
{
	ApplyButtonLook(BodyLayer_Btn, SelectedLayer == EIHPCAnatomyLayer::Body ? BodySelectedTexture : BodyUnselectedTexture);
	ApplyButtonLook(MusclesLayer_Btn, SelectedLayer == EIHPCAnatomyLayer::Muscles ? MusclesSelectedTexture : MusclesUnselectedTexture);
	ApplyButtonLook(SkeletonLayer_Btn, SelectedLayer == EIHPCAnatomyLayer::Skeleton ? SkeletonSelectedTexture : SkeletonUnselectedTexture);

	if (SkeletonBadge)
	{
		// The badge is only decoration: it must not catch the click meant for the skeleton button.
		SkeletonBadge->SetVisibility(bShowSkeletonBadge ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}

void UCPP_UW_IHPCLayerSwitcher::ApplyButtonLook(UButton* Button, UTexture2D* Texture) const
{
	if (!Button || !Texture)
	{
		return;   // keep whatever look the Widget Blueprint has
	}

	auto MakeBrush = [&](float Tint)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(Texture);
		Brush.ImageSize = ButtonSize;
		Brush.DrawAs = ESlateBrushDrawType::Image;
		Brush.TintColor = FSlateColor(FLinearColor(Tint, Tint, Tint, 1.f));
		return Brush;
	};

	FButtonStyle Style = Button->GetStyle();
	Style.SetNormal(MakeBrush(1.f));
	Style.SetHovered(MakeBrush(HoveredTint));
	Style.SetPressed(MakeBrush(PressedTint));
	Style.SetDisabled(MakeBrush(0.5f));
	Style.SetNormalPadding(FMargin(0.f));
	Style.SetPressedPadding(FMargin(0.f));
	Button->SetStyle(Style);
	Button->SetBackgroundColor(FLinearColor::White);
}
