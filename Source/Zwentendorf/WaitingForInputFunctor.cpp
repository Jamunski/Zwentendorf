// Fill out your copyright notice in the Description page of Project Settings.

#include "WaitingForInputFunctor.h"

#include "SoulPlayerController.h"

bool UWaitingForInputFunctor::operator()(ASoulPlayerController* thisPlayerController)
{
	UE_LOG(LogInput, Log, TEXT("InputReceivedWhileWaiting: %s, ControllerID: %d"), *thisPlayerController->GetName(), thisPlayerController->GetLocalPlayer()->GetControllerId());

	return Delegate_WaitingForInput.ExecuteIfBound(thisPlayerController);
}


