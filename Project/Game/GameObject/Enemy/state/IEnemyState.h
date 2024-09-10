#pragma once
#include "Camera.h"
#include "WorldTransform.h"

class Enemy;

class IEnemyState
{
public:
	virtual ~IEnemyState() = default;

	virtual void Initialize(Enemy* named) = 0;

	virtual void Update(Enemy* named) = 0;

	virtual const WorldTransform& GetWorldTransform() const = 0;
};