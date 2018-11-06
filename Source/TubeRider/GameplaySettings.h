// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class Utils {
public:
	static float map(float value,
		float istart, float istop,
		float ostart, float ostop)
	{
		return ostart + (ostop - ostart) * ((value - istart) / (istop - istart));
	}
};

/**
 *
 */
class TUBERIDER_API GameplaySettings
{
public:
	static GameplaySettings* GetSettings() {
		if (instance == NULL) {
			instance = new GameplaySettings();
		}
		return instance;
	}
	~GameplaySettings();

	float lerpPlayerVelocity();
	float lerpObstacleInterval();
	float lerpDifficulty();
	float lerpTimeSpan();

	void update(float DeltaTime);
	void start();


	float playerInitialVelocity;
	float playerMaxVelocity;

	float initialObstacleInterval;
	float maxObstacleInterval;

	float initialSeconds;
	float maxSeconds;

	float initialLifeSpan;
	float maxLifeSpan;

private:
	GameplaySettings();
	static GameplaySettings* instance;

	float time;
};
