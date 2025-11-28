// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_PlayerLocationIfSeen.h"

#include "ShooterAI.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_PlayerLocationIfSeen::UBTService_PlayerLocationIfSeen()
{
	NodeName = TEXT("Update Playerlocation if seen");
}

void UBTService_PlayerLocationIfSeen::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AShooterAI* OwnerController = Cast<AShooterAI>(OwnerComp.GetAIOwner());
	AShooterSamCharacter* Player = OwnerController->PlayerCharacter;
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();

	if (OwnerController && Player && BlackboardComponent)
	{
		if (OwnerController->LineOfSightTo(Player))
		{
			OwnerController->SetFocus(Player);
			BlackboardComponent->SetValueAsVector(GetSelectedBlackboardKey(), Player->GetActorLocation());
		}
		else
		{
			OwnerController->ClearFocus(EAIFocusPriority::Gameplay);
			BlackboardComponent->ClearValue(GetSelectedBlackboardKey());
		}
	}
}
