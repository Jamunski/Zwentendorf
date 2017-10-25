// Fill out your copyright notice in the Description page of Project Settings.

#include "Soul.h"


ASoul::ASoul()
{
}

float ASoul::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float healthPoints = GetHealthPoints();

	if (DamageAmount > 0.001f)
	{
		ApplyDamage(DamageAmount);

		if (healthPoints <= 0.0f)
		{
			HandleDeath();
		}
	}

	return healthPoints;
}

void ASoul::HandleDeath()
{
	//Logic goes here...

	OnDeath();
}
