// Fill out your copyright notice in the Description page of Project Settings.


#include "Collectable.h"

ACollectable::ACollectable()
{
	this->Tags.Emplace("Collectable");
	type = ECollectableType::Collect_Treasure;
	achievementId = -1;
}


