#include "PhysicsWorld.h"
#include <GameObject\Transform\Transform.h>
#include <GameObject\GameObject.h>
#include <Physics\Shapes\AABB.h>
#include <Physics\Shapes\Circle.h>

static PhysicsWorld& PW = PhysicsWorld::Instance();

PhysicsWorld::PhysicsWorld() :
	m_colliders(),
	m_rigidbodies(),
	m_aabbPool(MAX_GAME_OBJECTS + MAX_GAME_OBJECT_ARCHETYPES, false),
	m_circlePool(MAX_GAME_OBJECTS + MAX_GAME_OBJECT_ARCHETYPES, false),
	m_rigidbodyPool(MAX_GAME_OBJECTS + MAX_GAME_OBJECT_ARCHETYPES, false),
	m_bodyPool(MAX_GAME_OBJECTS + MAX_GAME_OBJECT_ARCHETYPES, false),
	m_gravityMode(GRAVITY_MODE_DIRECTION),
	m_gravity{0.f, -1.f}
{}

void PhysicsWorld::UpdateKinematics(f32 dt)
{
	size_t size = m_rigidbodies.size();
	for (size_t i = 0; i < size; ++i)
	{
		Rigidbody* body = m_rigidbodies[i];
		if (body->parent->gameObject->IsArchetype())
			continue;

		body->RecalculateMass();
		if (body->isStatic) continue;
		Transform* transform = body->parent->transform;
		
		// apply gravity
		float gravityScaleMultiplier = (body->gravityScale * body->mass * 0.1f);
		if (m_gravityMode == GRAVITY_MODE_DIRECTION)
		{
			body->ApplyImpulse(m_gravity * gravityScaleMultiplier, vec2());
		}
		else if (m_gravityMode == GRAVITY_MODE_POINT)
		{
			vec2 direction = m_gravity - transform->GetPosition();
			NORMALIZE_VEC2(direction);
			body->ApplyImpulse(direction * gravityScaleMultiplier, vec2());
		}

		vec2 t = transform->GetPosition();
		vec2 v = body->velocity;
		vec2 a = body->acceleration;

		body->oldPosition = t;
		v += a * dt;
		v += body->forceAccum * body->GetInverseMass() * dt;
		t += v * dt;

		f32 av = body->angularVelocity;
		if (fabs(av) > 0.f)
		{
			if (av > 0.f) av -= ROTATIONAL_DRAG;
			else av += ROTATIONAL_DRAG;
		}

		f32 r = transform->GetRotation();
		r += av * dt;

		body->angularVelocity = av;
		body->velocity = v;
		transform->SetPosition(t);
		transform->SetRotation(r);

		body->forceAccum = vec2();
	}
}

static void UpdateShapeAttributes(Shape* shape, const Transform* t)
{
	shape->position = t->GetPosition();
	vec2 scale = t->GetScale();
	switch (shape->type)
	{
		case st_AABB:
		{
			AABB* box = dynamic_cast<AABB*>(shape);
			box->height = scale.y;
			box->width = scale.x;
			break;
		}
		case st_Circle:
		{
			Circle* circle = dynamic_cast<Circle*>(shape);
			circle->radius = (scale.x < scale.y) ? scale.x : scale.y;
			break;
		}
	}
}

void PhysicsWorld::UpdateCollision(f32 dt)
{
	size_t size = m_colliders.size();
	
	// bad broadphase
	/*m_contacts.clear();
	for (size_t i = 0; i < size; ++i)
	{
		Shape* A = m_colliders[i];
		if (A->body->gameObject->IsArchetype())
			continue;

		for (size_t j = i + 1; j < size; ++j)
		{
			Shape* B = m_colliders[j];
			if (B->body->gameObject->IsArchetype())
				continue;

			Manifold m;
			m.A = A->body;
			m.B = B->body;
			m_contacts.insert(m);
		}
	}*/

	//for (auto iter = m_contacts.begin(); iter != m_contacts.end(); ++iter)
	for(size_t i = 0; i < size; ++i)
	{
		Shape* A = m_colliders[i];
		if (A->body->gameObject->IsArchetype())
			continue;
		for (size_t j = i + 1; j < size; ++j)
		{
			Shape* B = m_colliders[j];
			if (B->body->gameObject->IsArchetype())
				continue;

			//Manifold next = *iter;
			Manifold next;
			next.A = A->body;
			next.B = B->body;

			bool wereColliding = next.A->shape->justCollided || next.B->shape->justCollided;

			UpdateShapeAttributes(next.A->shape, next.A->transform);
			UpdateShapeAttributes(next.B->shape, next.B->transform);

			if (next.A->shape->CollidesWith(next.B->shape, next))
			{
				if (next.A->body && next.B->body &&
					!next.A->shape->isTrigger && !next.B->shape->isTrigger)
				{
					ResolveCollision(next);
				}

				next.A->shape->justCollided = true;
				next.B->shape->justCollided = true;

				if (!wereColliding)
				{
					next.A->gameObject->OnCollisionEnter(next.B->gameObject);
					next.B->gameObject->OnCollisionEnter(next.A->gameObject);
				}
			}
			else
			{
				next.A->shape->justCollided = false;
				next.B->shape->justCollided = false;

				if (wereColliding)
				{
					next.A->gameObject->OnCollisionExit(next.B->gameObject);
					next.B->gameObject->OnCollisionExit(next.A->gameObject);
				}
			}
		}
	}
}

void PhysicsWorld::ResolveCollision(const Manifold& pair)
{
	// Complex resolution for rotations later
	/*{
		Transform* tA = pair.A->transform;
		Transform* tB = pair.B->transform;
		Rigidbody* rA = pair.A->body;
		Rigidbody* rB = pair.B->body;

		f32 invMassSum = rA->GetInverseMass() + rB->GetInverseMass();
		if (!invMassSum) return;

		vec2 contactVecA = pair.contactPoint - tA->GetPosition();
		vec2 contactVecB = pair.contactPoint - tB->GetPosition();

		vec2 vpA = rA->velocity + vec2::Cross(rA->angularVelocity / 180.f * 3.141593f, contactVecA);
		vec2 vpB = rB->velocity + vec2::Cross(rB->angularVelocity / 180.f * 3.141593f, contactVecB);
		vec2 vRelativeP = vpB - vpA;

		// only take average restitution
		f32 e = (rA->restitution + rB->restitution) / 2.f;

		f32 crossA = vec2::Cross(contactVecA, pair.normal);
		f32 crossB = vec2::Cross(contactVecB, pair.normal);
		f32 torqueSum = crossA * crossB * rA->GetInverseInertia() +
			crossB * crossB * rB->GetInverseInertia();
		float j = -(1.f + e) * vec2::Dot(vRelativeP, pair.normal);

		j /= invMassSum + torqueSum;

		// friction + torque
		vec2 tangent = (vRelativeP - vec2::Dot(vRelativeP, pair.normal) * pair.normal);
		NORMALIZE_VEC2(tangent);

		f32 jt = -(1.f + 0) * vec2::Dot(vRelativeP, tangent);

		f32 TcrossA = vec2::Cross(contactVecA, tangent);
		f32 TcrossB = vec2::Cross(contactVecB, tangent);
		f32 tanTorqueSum = TcrossA * TcrossA * rA->GetInverseInertia() +
			TcrossB * TcrossB * rB->GetInverseInertia();

		jt /= invMassSum + tanTorqueSum;
		f32 mu = (rA->staticFriction + rB->staticFriction) / 2.f + 0.01f;
		vec2 frictionImpulse;
		if (abs(jt) < abs(j * mu))
		{
			frictionImpulse = jt * tangent;
		}
		else
		{
			mu = (rA->dynamicFriction + rB->dynamicFriction) / 2.f;
			frictionImpulse = mu * j * tangent;
		}

		vec2 impulse = j * pair.normal;
		f32 massSum = rA->mass + rB->mass;
		f32 ratio = rA->mass / massSum;

		rA->ApplyImpulse(ratio * -1.f * impulse, contactVecA);
		rA->ApplyImpulse(frictionImpulse * -1.f, contactVecA);

		ratio = rB->mass / massSum;
		rB->ApplyImpulse(ratio * impulse, contactVecB);
		rB->ApplyImpulse(frictionImpulse, contactVecB);

		// object a is penetrating into object b
		// moving in a direction opposite to the normal
		f32 penetrationPerInvMass = pair.penetration / invMassSum;
		tA->SetPosition(tA->GetPosition() - penetrationPerInvMass * rA->GetInverseMass() * pair.normal);
		tB->SetPosition(tB->GetPosition() + penetrationPerInvMass * rB->GetInverseMass() * pair.normal);
	}*/

	// Easy resolution without rotation
	{
		Rigidbody* A = pair.A->body;
		Rigidbody* B = pair.B->body;
		Transform* tA = pair.A->transform;
		Transform* tB = pair.B->transform;

		vec2 rv = A->velocity - B->velocity;
		f32 velAlongNormal = vec2::Dot(rv, pair.normal);

		if (velAlongNormal > 0) return;

		f32 aInvMass = A->GetInverseMass();
		f32 bInvMass = B->GetInverseMass();
		f32 invMassSum = aInvMass + bInvMass;
		if (!invMassSum) return;

		f32 e = (A->restitution + B->restitution) / 2.f;
		f32 j = -(1.f + e) * velAlongNormal;
		
		j /= (invMassSum);

		vec2 tangent = rv - velAlongNormal * pair.normal;
		NORMALIZE_VEC2(tangent);

		f32 jt = -1.f * vec2::Dot(rv, tangent);
		jt /= invMassSum;

		f32 mu = MAX(A->staticFriction, B->staticFriction);

		vec2 frictionImpulse;
		if (std::abs(jt) < j * mu)
		{
			frictionImpulse = jt * tangent;
		}
		else
		{
			mu = MAX(A->dynamicFriction, B->dynamicFriction);
			frictionImpulse = -1.f * j * tangent * mu;
		}

		vec2 impulse = pair.normal * j;
		A->ApplyImpulse(impulse, vec2());
		B->ApplyImpulse(impulse * -1.f, vec2());

		A->ApplyImpulse(frictionImpulse);
		B->ApplyImpulse(frictionImpulse * -1.f);

		// object a is penetrating into object b
		// moving in a direction opposite to the normal
		const f32 percent = 0.3f; // could be between 0.2 and 0.8
		const f32 slop = 0.085f; // usually between 0.01 and 0.1
		vec2 correction = MAX(abs(pair.penetration) - slop, 0.f) / invMassSum * percent * pair.normal;
		tA->SetPosition(tA->GetPosition() + correction * aInvMass);
		tB->SetPosition(tB->GetPosition() - correction * bInvMass);
	}
}

void PhysicsWorld::Init()
{
	m_colliders.reserve(MAX_GAME_OBJECTS * 2);
	m_rigidbodies.reserve(MAX_GAME_OBJECTS);
}

void PhysicsWorld::Update(float dt)
{
	UpdateKinematics(dt);
	UpdateCollision(dt);
}

void PhysicsWorld::Exit()
{
	m_colliders.clear();
	m_colliders.shrink_to_fit();

	m_rigidbodies.clear();
	m_rigidbodies.shrink_to_fit();
}

Body* PhysicsWorld::CreateBody(GameObject* gameObject, Transform* transform, ShapeType shapeType, PhysicsMaterialType material, bool calcMass)
{
	Body* obj = m_bodyPool.Construct(gameObject, transform, nullptr, nullptr);
	Shape* shape = nullptr;
	Rigidbody* rigidBody = nullptr;
	vec2 size = transform->GetScale();
	switch (shapeType)
	{
	case st_AABB:
		shape = (Shape*)m_aabbPool.Construct(obj, size.x, size.y, transform->GetPosition());
		break;
	case st_Circle:
		shape = (Shape*)m_circlePool.Construct(obj, size.x / 2.f, transform->GetPosition());
		break;
	}
	shape->__id = (u32)m_colliders.size();
	m_colliders.push_back(shape);
	obj->shape = shape;

	rigidBody = m_rigidbodyPool.Construct(obj, material, calcMass);
	rigidBody->__id = (u32)m_rigidbodies.size();
	m_rigidbodies.push_back(rigidBody);
	obj->body = rigidBody;

	return obj;
}

void PhysicsWorld::SetGravity(bool mode, const vec2 & gravity)
{
	m_gravityMode = mode;
	m_gravity = gravity;

	if (mode == GRAVITY_MODE_DIRECTION)
	{
		NORMALIZE_VEC2(m_gravity);
	}
}

std::pair<bool, vec2> PhysicsWorld::GetGravity() const
{
	return std::make_pair(m_gravityMode, m_gravity);
}

void PhysicsWorld::DeleteBody(Body * body)
{
	if (body->shape)
	{
		// remove from active list
		u32 id = body->shape->__id;
		u32 swapID = m_colliders.back()->__id;
		
		std::swap(m_colliders[id], m_colliders[swapID]);
		m_colliders[id]->__id = id;
		m_colliders[swapID] = nullptr;
		m_colliders.pop_back();

		switch (body->shape->type)
		{
		case st_AABB:
			m_aabbPool.Free(dynamic_cast<AABB*>(body->shape));
			break;
		case st_Circle:
			m_circlePool.Free(dynamic_cast<Circle *>(body->shape));
			break;
		}
	}

	if (body->body)
	{
		// remove from active list
		u32 id = body->body->__id;
		u32 swapID = m_rigidbodies.back()->__id;

		std::swap(m_rigidbodies[id], m_rigidbodies[swapID]);
		m_rigidbodies[id]->__id = id;
		m_rigidbodies[swapID] = nullptr;
		m_rigidbodies.pop_back();

		m_rigidbodyPool.Free(body->body);
	}

	m_bodyPool.Free(body);
}

Body * Dandelion::CreatePhysicsBody(GameObject* gameObject, ShapeType shape, PhysicsMaterialType material, bool calcMass)
{
	return PW.CreateBody(gameObject, gameObject->GetTransform(), shape, material, calcMass);
}

void Dandelion::DeletePhysicsBody(Body * body)
{
	PW.DeleteBody(body);
}

void Dandelion::SetGravity(bool mode, const vec2 & gravity)
{
	PW.SetGravity(mode, gravity);
}

std::pair<bool, vec2> Dandelion::GetGravity()
{
	return PW.GetGravity();
}
