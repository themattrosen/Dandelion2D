#pragma once

#include <Core\Definitions.h>
#include <Core\Math\Vector.h>
#include <GameObject\Transform\Transform.h>

class GameObject;
class Transform;
struct Shape;
struct Body;
struct Rigidbody;

struct Body
{
	Body(GameObject* parent, Transform* _transform, Shape* _shape, Rigidbody* _body) :
		gameObject(parent),
		transform(_transform),
		shape(_shape),
		body(_body)
	{}
	GameObject* gameObject;
	Transform* transform;
	Shape* shape;
	Rigidbody* body;
};

enum PhysicsMaterialType
{
	pmt_Custom = 0,
	pmt_Rock,
	pmt_Wood,
	pmt_Metal,
	pmt_Ice,
	pmt_BouncyBall,
	pmt_SuperBall,
	pmt_Pillow,
	pmt_Static,
};

struct PhysicsMaterial
{
	PhysicsMaterialType type;
	f32 restitution;
	f32 density;
};

const PhysicsMaterial substanceList[] = {
	{ pmt_Custom,	  0.3f, 0.0f },
	{ pmt_Rock,       0.6f, 0.1f },  // Rock       Density : 0.6  Restitution : 0.1
	{ pmt_Wood,       0.3f, 0.2f },  // Wood       Density : 0.3  Restitution : 0.2
	{ pmt_Metal,      1.2f, 0.05f }, // Metal      Density : 1.2  Restitution : 0.05
	{ pmt_Ice,        0.6f, 0.1f },  // Ice        Density : 1.2  Restitution : 0.05
	{ pmt_BouncyBall, 0.3f, 0.8f },  // BouncyBall Density : 0.3  Restitution : 0.8
	{ pmt_SuperBall,  0.3f, 0.95f }, // SuperBall  Density : 0.3  Restitution : 0.95
	{ pmt_Pillow,     0.1f, 0.2f },  // Pillow     Density : 0.1  Restitution : 0.2
	{ pmt_Static,     0.0f, 0.4f }  // Static     Density : 0.0  Restitution : 0.4
};

struct Rigidbody
{
	Rigidbody(Body* _parent, PhysicsMaterialType _type, bool calcMass = false) :
		oldPosition(), velocity(), acceleration(), angularVelocity(),
		type(_type),
		restitution(substanceList[type].restitution),
		density(substanceList[type].density),
		autoCalcMass(calcMass),
		mass(0.f), inertia(0.f),
		gravityScale(1.f),
		staticFriction(0.f),
		dynamicFriction(0.f),
		isStatic(false),
		canRotate(false),
		parent(_parent)
	{
		RecalculateMass();
	}

	void RecalculateMass()
	{
		if (autoCalcMass)
		{
			Transform* t = parent->transform;
			vec2 size = t->GetScale();
			if (!isStatic)
			{
				// mass = density * volume
				mass = density * size.x * size.y;
				inertia = mass * (size.x * size.x + size.y * size.y) / 12.f;
			}
			else
			{
				mass = 0.f;
				inertia = 0.f;
			}

		}
	}

	void ApplyImpulse(const vec2& force, const vec2& contactVec = vec2())
	{
		velocity += force * GetInverseMass();
		if (canRotate)
			angularVelocity += GetInverseInertia() * vec2::Cross(contactVec, force) * 180.f / 3.141593f;
	}

	void ApplyForce(const vec2& force)
	{
		forceAccum += force;
	}

	float GetInverseMass() const { return mass == 0.f ? 0.f : 1.f / mass; }
	float GetInverseInertia() const { return inertia == 0.f ? 0.f : 1.f / inertia; }

	vec2 oldPosition;
	vec2 velocity;
	vec2 acceleration;
	f32 angularVelocity;

	vec2 forceAccum;

	PhysicsMaterialType type;
	f32 restitution;
	f32 density;
	bool autoCalcMass;
	f32 mass;
	f32 inertia;
	f32 gravityScale;
	f32 staticFriction;
	f32 dynamicFriction;
	bool isStatic;
	bool canRotate;
	Body* parent;
	u32 __id;
};
