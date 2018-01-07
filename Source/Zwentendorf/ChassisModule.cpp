// Fill out your copyright notice in the Description page of Project Settings.

#include "ChassisModule.h"


AChassisModule::AChassisModule()
	: AModule()
{
}

float AChassisModule::TakeDamage(const float damage)
{
	HealthPoints -= damage;

	return HealthPoints;
}

