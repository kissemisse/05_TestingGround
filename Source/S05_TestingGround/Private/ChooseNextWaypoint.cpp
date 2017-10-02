// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/ChooseNextWaypoint.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Classes/AIController.h"
#include "Public/PatrolRouteComponent.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	// get all patrolling points
	// get controller (AI)
	APawn *controlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	// get the patrolling points from the controller
	UPatrolRouteComponent *patrolRouteComponent = controlledPawn->FindComponentByClass<UPatrolRouteComponent>();
	
	// ensure that we have patrolComponents
	if (!ensure(patrolRouteComponent)) { return EBTNodeResult::Failed; }
	
	// fetch the array from the patrolComponent
	TArray<AActor*> patrolPoints = patrolRouteComponent->getPatrolPoints();
	
	if (patrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("A guard is missing patrol points"));
		return EBTNodeResult::Aborted;
	}

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



