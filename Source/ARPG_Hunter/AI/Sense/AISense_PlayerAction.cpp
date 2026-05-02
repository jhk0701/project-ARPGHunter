// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Sense/AISense_PlayerAction.h"
#include "Perception/AIPerceptionSystem.h"

#include "AI/Sense/AISenseConfig_PlayerAction.h"
#include "AI/Sense/AISenseEvent_PlayerAction.h"

UAISense_PlayerAction::UAISense_PlayerAction()
{
}

void UAISense_PlayerAction::ReportEvent(UObject* _worldContext,
	uint8 _type,
	const FVector& _location,
	float _range,
	AActor* _instigator)
{
	UWorld* World = GEngine->GetWorldFromContextObject(_worldContext, EGetWorldErrorMode::LogAndReturnNull);

	if (nullptr == World)
		return;

	FAIPlayerActionStimulusEvent Event;
	Event.ActionType = _type;
	Event.Location = _location;
	Event.Range = _range;
	Event.Instigator = _instigator;

	UAIPerceptionSystem::OnEvent<FAIPlayerActionStimulusEvent, FAIPlayerActionStimulusEvent::FSenseClass>(World, Event);
}

void UAISense_PlayerAction::ReportEvent(UObject* _worldContext, 
	const FAIPlayerActionStimulusEvent& _event)
{
	UWorld* World = GEngine->GetWorldFromContextObject(_worldContext, EGetWorldErrorMode::LogAndReturnNull);

	if (nullptr == World)
		return;

	UAIPerceptionSystem::OnEvent<FAIPlayerActionStimulusEvent, FAIPlayerActionStimulusEvent::FSenseClass>(World, _event);
}

void UAISense_PlayerAction::RegisterEvent(const FAIPlayerActionStimulusEvent& Event)
{
	RegisteredEvents.Add(Event);
	RequestImmediateUpdate();
}

void UAISense_PlayerAction::RegisterEventsBatch(const TArray<FAIPlayerActionStimulusEvent>& Events)
{
	RegisteredEvents.Append(Events);
	RequestImmediateUpdate();
}

float UAISense_PlayerAction::Update()
{
	// 핵심 처리 함수 — 매 Sense 업데이트 틱마다 호출
	AIPerception::FListenerMap* ListenersMap = GetListeners();

	if (nullptr == ListenersMap)
		return SuspendNextUpdate;

	// 등록된 이벤트 처리
	for (const FAIPlayerActionStimulusEvent& Event : RegisteredEvents)
	{
		// 감지를 등록한 리스너들에 대해 순회
		for (TPair<FPerceptionListenerID, FPerceptionListener>& Pair : *ListenersMap)
		{
			FPerceptionListener& Listener = Pair.Value;

			// 리스너가 이 Sense를 등록했는지 확인
			if (!Listener.HasSense(GetSenseID()))
				continue;

			const AActor* ListenerActor = Listener.GetBodyActor();
			if (nullptr == ListenerActor)
				continue;

			// 발생 위치와 Listener의 거리 체크
			FVector DistVec = Event.Location - ListenerActor->GetActorLocation();
			if (DistVec.SquaredLength() < Event.Range * Event.Range)
			{
				// 지정한 범위 이내라면 Listener에게 이벤트 전달
				FAIStimulus Stimulus(
					*this,
					static_cast<float>(Event.ActionType), // 원래 자극 강도 : 현재는 공격 타입을 보내줄 것
					Event.Location,  // 자극 발생 지점
					ListenerActor->GetActorLocation() // 수신자 위치
				);

				Listener.RegisterStimulus(Event.Instigator, Stimulus);
			}
		}
	}

	RegisteredEvents.Reset();

	// 다음 업데이트까지 대기
	return SuspendNextUpdate;
}
