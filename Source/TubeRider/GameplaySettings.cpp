// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplaySettings.h"

GameplaySettings* GameplaySettings::instance = NULL;

GameplaySettings::GameplaySettings()
{
	playerInitialVelocity = 100.f;
	playerMaxVelocity = 300.f;

	initialObstacleInterval = 3.f;
	maxObstacleInterval = .1f;

	initialSeconds = 0.f;
	maxSeconds = 100.f;
}

void GameplaySettings::update(float DeltaTime)
{
	time += DeltaTime;
}

float GameplaySettings::lerpDifficulty() {
	return FMath::Min(1.f, time / maxSeconds);
}

float GameplaySettings::lerpPlayerVelocity() {
	return Utils::map(time, initialSeconds, maxSeconds, playerInitialVelocity, playerMaxVelocity);
}

float GameplaySettings::lerpObstacleInterval() {
	return Utils::map(time, initialSeconds, maxSeconds, initialObstacleInterval, maxObstacleInterval);
}

GameplaySettings::~GameplaySettings()
{
}
