// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/ChooseNextWaypoint.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Classes/AIController.h"
#include "Public/PatrollingGuard.h" // todo remove coupling

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	// get all patrolling points
	AAIController *aiController = OwnerComp.GetAIOwner();
	APawn *controlledPawn = aiController->GetPawn();
	APatrollingGuard *patrollingGuard = Cast<APatrollingGuard>(controlledPawn);
	TArray<AActor*> patrolPoints = patrollingGuard->patrolPointsCpp;

	// set next waypoint
	UBlackboardComponent *blackBoardComponent = OwnerComp.GetBlackboardComponent();
	int32 index = blackBoardComponent->GetValueAsInt(indexKey.SelectedKeyName);
	blackBoardComponent->SetValueAsObject(wayPointKey.SelectedKeyName, patrolPoints[index]);

	// cycle index
	auto nextIndex = ( index + 1 ) % patrolPoints.Num();
	blackBoardComponent->SetValueAsInt(indexKey.SelectedKeyName, nextIndex);

	//UE_LOG(LogTemp, Warning, TEXT("waypoint index: %i"), index);

	return EBTNodeResult::Succeeded;
}



