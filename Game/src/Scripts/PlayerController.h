#pragma once
#include <Dandelion.h>

enum PlayerState
{
	ps_Idle,
	ps_InAir,
	ps_WalkingLeft,
	ps_WalkingRight
};

static Key JUMP_KEY = Key::KeyW;
static Key LEFT_KEY = Key::KeyA;
static Key RIGHT_KEY = Key::KeyD;
static MouseButton SHOOT_BUTTON = MouseButton::MouseLeft;
static MouseButton RETURN_GRAVITY_BUTTON = MouseButton::MouseRight;

static f32 DEFAULT_JUMP_SPEED = 10.f;
static f32 DEFAULT_WALK_SPEED = 0.2f;
static f32 DEFAULT_BULLET_SPEED = 1.f;

static void HandleShoot(GameObject* parent)
{
	if (Dandelion::IsMouseButtonTriggered(SHOOT_BUTTON))
	{
		vec2 mousePos = Dandelion::GetMousePos();
		// convert mousePos to world space

		GameObject* bullet = Dandelion::Instantiate("bullet");
		vec2 bulletAxis = mousePos - parent->GetPosition();
		
		NORMALIZE_VEC2(bulletAxis);

		bullet->SetPosition(parent->GetPosition() + bulletAxis);
		bullet->GetBody()->body->ApplyForce(bulletAxis * DEFAULT_BULLET_SPEED);

	}

	else if (Dandelion::IsMouseButtonTriggered(RETURN_GRAVITY_BUTTON))
	{
		Dandelion::SetGravity(GRAVITY_MODE_DIRECTION, vec2(0, -1));
	}
}

const Script PlayerController = BEGIN_SCRIPT(PlayerController)

SCRIPT_VALUES()
{
	SCRIPT_VALUE(jumpSpeed, DEFAULT_JUMP_SPEED),
	SCRIPT_VALUE(walkSpeed, DEFAULT_WALK_SPEED),
	SCRIPT_VALUE(playerState, 1.f)
}

SCRIPT_REFS()
{
}

SCRIPT_UPDATE(float dt, GameObject* parent)
{
	PlayerState state = (PlayerState)(int)VALUE(playerState, PlayerController);
	Rigidbody* body = parent->GetBody()->body;

	auto gravityInfo = Dandelion::GetGravity();
	vec2 gravityDirection = gravityInfo.second;
	if (gravityInfo.first == GRAVITY_MODE_POINT)
	{
		gravityDirection -= parent->GetPosition();
		NORMALIZE_VEC2(gravityDirection);
	}

	vec2 jumpDirection = gravityDirection * -1.f;
	vec3 rightTemp = glm::cross(vec3(jumpDirection.x, jumpDirection.y, 0.f), vec3(0, 0, 1.f));
	vec2 rightDirection(rightTemp.x, rightTemp.y);
	NORMALIZE_VEC2(rightDirection);
	vec2 leftDirection = rightDirection * -1.f;

	HandleShoot(parent);

	switch (state)
	{
		case ps_Idle:
		{
			if (Dandelion::IsKeyTriggered(JUMP_KEY))
			{
				body->ApplyForce(jumpDirection * VALUE(jumpSpeed, PlayerController));
				VALUE(playerState, PlayerController) = (float)(int)(ps_InAir);
				Dandelion::PlaySound("on_jump");
			}
			else if (Dandelion::IsKeyPressed(LEFT_KEY))
			{
				body->ApplyForce(leftDirection * VALUE(walkSpeed, PlayerController));
				VALUE(playerState, PlayerController) = (float)(int)(ps_WalkingLeft);
			}
			else if (Dandelion::IsKeyPressed(RIGHT_KEY))
			{
				body->ApplyForce(rightDirection * VALUE(walkSpeed, PlayerController));
				VALUE(playerState, PlayerController) = (float)(int)(ps_WalkingRight);
			}
			break;
		}
		case ps_InAir:
		{
			if (Dandelion::IsKeyPressed(LEFT_KEY))
			{
				body->ApplyForce(leftDirection * VALUE(walkSpeed, PlayerController));
			}
			else if (Dandelion::IsKeyPressed(RIGHT_KEY))
			{
				body->ApplyForce(rightDirection * VALUE(walkSpeed, PlayerController));
			}
			break;
		}
		case ps_WalkingLeft:
		{
			if (Dandelion::IsKeyReleased(LEFT_KEY))
			{
				VALUE(playerState, PlayerController) = (float)(int)(ps_Idle);
			}
			else if (Dandelion::IsKeyPressed(RIGHT_KEY))
			{
				body->ApplyForce(rightDirection * VALUE(walkSpeed, PlayerController));
				VALUE(playerState, PlayerController) = (float)(int)(ps_WalkingRight);
			}
			else if (Dandelion::IsKeyTriggered(JUMP_KEY))
			{
				body->ApplyForce(jumpDirection * VALUE(jumpSpeed, PlayerController));
				VALUE(playerState, PlayerController) = (float)(int)(ps_InAir);
				Dandelion::PlaySound("on_jump");
			}
			else
			{
				body->ApplyForce(leftDirection * VALUE(walkSpeed, PlayerController));
			}
			break;
		}
		case ps_WalkingRight:
		{
			if (Dandelion::IsKeyReleased(RIGHT_KEY))
			{
				VALUE(playerState, PlayerController) = (float)(int)(ps_Idle);
			}
			else if (Dandelion::IsKeyPressed(LEFT_KEY))
			{
				body->ApplyForce(leftDirection * VALUE(walkSpeed, PlayerController));
				VALUE(playerState, PlayerController) = (float)(int)(ps_WalkingRight);
			}
			else if (Dandelion::IsKeyTriggered(JUMP_KEY))
			{
				body->ApplyForce(jumpDirection * VALUE(jumpSpeed, PlayerController));
				VALUE(playerState, PlayerController) = (float)(int)(ps_InAir);
				Dandelion::PlaySound("on_jump");
			}
			else
			{
				body->ApplyForce(rightDirection * VALUE(walkSpeed, PlayerController));
			}
			break;
		}
	}
}

SCRIPT_ON_COLLISION_ENTER(GameObject* parent, GameObject* other)
{
	std::pair<bool, vec2> gravityInfo = Dandelion::GetGravity();
	vec2 gravityDirection = gravityInfo.second;
	if (gravityInfo.first == GRAVITY_MODE_POINT)
	{
		gravityDirection -= parent->GetPosition();
		NORMALIZE_VEC2(gravityDirection);
	}


	PlayerState currentState = (PlayerState)(int)VALUE(playerState, PlayerController);

	if (currentState == ps_InAir)
	{
		//if (parent->GetPosition().y > other->GetPosition().y)
		Dandelion::PlaySound("on_land");
		vec2 collisionAxis = other->GetPosition() - parent->GetPosition();
		f32 dot = collisionAxis.Dot(gravityDirection);
		if (dot > 0.f)
		{
			if (Dandelion::IsKeyPressed(LEFT_KEY))
			{
				VALUE(playerState, PlayerController) = (float)(int)(ps_WalkingLeft);
			}
			else if (Dandelion::IsKeyPressed(RIGHT_KEY))
			{
				VALUE(playerState, PlayerController) = (float)(int)(ps_WalkingRight);
			}
			else
			{
				VALUE(playerState, PlayerController) = (float)(int)(ps_Idle);
			}
		}
	}
}

SCRIPT_ON_COLLISION_EXIT(GameObject* parent, GameObject* other)
{
	(void)parent;
	(void)other;
}

END_SCRIPT;
