// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AISense.h"
#include "AISense_PlayerAction.generated.h"

struct FAIPlayerActionStimulusEvent;

/**
 * 플레이어의 특정 동작을 감지하는 용도
 */
UCLASS(ClassGroup = AI, DisplayName = "Player Action Sense")
class ARPG_HUNTER_API UAISense_PlayerAction : public UAISense
{
	GENERATED_BODY()

public:
	UAISense_PlayerAction();

	static void ReportEvent(
		UObject* _worldContext,
		uint8 _type,
		const FVector& _location,
		float _range,
		AActor* _instigator
	);
	static void ReportEvent(UObject* _worldContext, const FAIPlayerActionStimulusEvent& _event);

	// OnEvent를 통해 등록시킬 때 사용할 메서드
	// perception System에서 OnEvent를 통해서 호출할 건데, 이때 템플릿 메서드에 따라 인터페이스를 구현
	void RegisterEvent(const FAIPlayerActionStimulusEvent& Event);
	void RegisterEventsBatch(const TArray<FAIPlayerActionStimulusEvent>& Events);

protected:
	// 자극 처리 핵심 로직
	virtual float Update() override;

private:
	// 등록된 이벤트
	TArray<FAIPlayerActionStimulusEvent> RegisteredEvents;
};
