// Fill out your copyright notice in the Description page of Project Settings.

#include "GameplaySettings.h"

GameplaySettings* GameplaySettings::instance = NULL;

GameplaySettings::GameplaySettings()
{
	playerInitialVelocity = 100.f;
	playerMaxVelocity = 250.f;
	//playerMaxVelocity = 300.f;

	initialObstacleInterval = 3.f;
	maxObstacleInterval = 2.f;

	initialSeconds = 0.f;
	maxSeconds = 100.f;

	initialLifeSpan=40.f;
	maxLifeSpan=initialLifeSpan*playerInitialVelocity/playerMaxVelocity;

	time = 0.f;
}

void GameplaySettings::start() {
	time = 0.f;
}

void GameplaySettings::update(float DeltaTime)
{
	time += DeltaTime;
	time = FMath::Min(maxSeconds, time);
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

float GameplaySettings::lerpTimeSpan() {
	return Utils::map(time, initialSeconds, maxSeconds, initialLifeSpan, maxLifeSpan);
}

GameplaySettings::~GameplaySettings()
{
}
