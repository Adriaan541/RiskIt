// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterAI.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Slate/SGameLayerManager.h"

void AShooterAI::BeginPlay()
{
	Super::BeginPlay();

	
	// PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
}

void AShooterAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*
	if (PlayerPawn)
	{
		if (LineOfSightTo(PlayerPawn))
		{
			HadLineOfSight = true;
			SetFocus(PlayerPawn);
			MoveToActor(PlayerPawn, 1000.0f);
		}
		else if (HadLineOfSight)
		{
			LastPlayerLocation = PlayerPawn->GetActorLocation();
			MoveToLocation(LastPlayerLocation);
			ClearFocus(EAIFocusPriority::Gameplay);
			HadLineOfSight = false;
		}
	}
	*/
}

void AShooterAI::StartBehaviourTree(AShooterSamCharacter* Player)
{
	if (EnemyAIBehaviourTree)
	{
		ThisCharacter = Cast<AShooterSamCharacter>(GetPawn());
		
		if (Player)
		{
			PlayerCharacter = Player;
		}
		RunBehaviorTree(EnemyAIBehaviourTree);

		UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();

		if (BlackboardComponent)
		{
			BlackboardComponent->SetValueAsVector("StartLocation", ThisCharacter->GetActorLocation());
		}
	}
}

void AShooterAI::AlertCharacter()
{
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent(); PlayerCharacter)
	{
		SetFocus(PlayerCharacter);
		BlackboardComponent->SetValueAsVector("LastKnownPlayerLocation", PlayerCharacter->GetActorLocation());
	}
}
