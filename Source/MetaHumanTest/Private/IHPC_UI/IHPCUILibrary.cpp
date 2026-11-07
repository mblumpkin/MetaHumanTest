// IHPCUILibrary.cpp - see IHPCUILibrary.h for what each function is for.

#include "IHPC_UI/IHPCUILibrary.h"

#include "AnimationTimelineComp.h"   // existing project component that scrubs the avatar animation
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"

UAnimationTimelineComp* UIHPCUILibrary::FindAvatarTimeline(const UObject* WorldContextObject)
{
	// Player 0's pawn is the avatar the user is looking at (same pawn the existing
	// WCPP_AnimationTimeline widget talks to).
	APawn* Pawn = UGameplayStatics::GetPlayerPawn(WorldContextObject, 0);
	return Pawn ? Pawn->FindComponentByClass<UAnimationTimelineComp>() : nullptr;
}

bool UIHPCUILibrary::SeekAvatarTimeline(const UObject* WorldContextObject, float NormalizedTime, bool bPauseAfterSeek)
{
	UAnimationTimelineComp* Timeline = FindAvatarTimeline(WorldContextObject);
	if (!Timeline)
	{
		UE_LOG(LogTemp, Warning, TEXT("[IHPC UI] No AnimationTimelineComp on the player pawn - cannot jump to %.2f"), NormalizedTime);
		return false;
	}

	// Stop playback first so the timeline does not immediately move away from the moment.
	if (bPauseAfterSeek)
	{
		Timeline->Pause();
	}
	Timeline->SetNormalizedTime(FMath::Clamp(NormalizedTime, 0.f, 1.f));
	return true;
}

float UIHPCUILibrary::GetAvatarTimelineTime(const UObject* WorldContextObject)
{
	const UAnimationTimelineComp* Timeline = FindAvatarTimeline(WorldContextObject);
	return Timeline ? Timeline->GetNormalizedTime() : -1.f;
}

FLinearColor UIHPCUILibrary::FigmaColor(const FString& Hex, float Opacity)
{
	// Figma colours are sRGB. Building an FColor and converting it to FLinearColor
	// does the sRGB -> linear conversion Unreal expects, so the colour on screen matches Figma.
	FLinearColor Color = FLinearColor(FColor::FromHex(Hex));
	Color.A = FMath::Clamp(Opacity, 0.f, 1.f);
	return Color;
}

FLinearColor UIHPCUILibrary::SeverityToColor(float Severity)
{
	// Colours sampled from the bar at the top of the Figma video cards.
	struct FStop { float At; const TCHAR* Hex; };
	static const FStop Stops[] = {
		{ 0.00f, TEXT("70BF44") },   // green
		{ 0.45f, TEXT("F2DE1C") },   // yellow
		{ 0.75f, TEXT("F7A21F") },   // orange
		{ 1.00f, TEXT("ED1C24") },   // red
	};

	const float S = FMath::Clamp(Severity, 0.f, 1.f);
	for (int32 i = 1; i < UE_ARRAY_COUNT(Stops); ++i)
	{
		if (S <= Stops[i].At)
		{
			const float Alpha = (S - Stops[i - 1].At) / (Stops[i].At - Stops[i - 1].At);
			return FLinearColor::LerpUsingHSV(FigmaColor(Stops[i - 1].Hex), FigmaColor(Stops[i].Hex), Alpha);
		}
	}
	return FigmaColor(Stops[UE_ARRAY_COUNT(Stops) - 1].Hex);
}
