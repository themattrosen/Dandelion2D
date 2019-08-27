#pragma once

#include <Core\Singleton.h>
#include <Core\Memory\Allocator.h>
#include <Physics\Shapes\Shape.h>
#include <Physics\Shapes\AABB.h>
#include <Physics\Shapes\Circle.h>
#include <Physics\Body\Body.h>
#include <Physics\Manifold.h>
#include <vector>
#include <set>

struct Circle;
struct AABB;
struct Body;
struct Rigidbody;
class Transform;


class PhysicsWorld : public Singleton<PhysicsWorld>
{
public:
	void Init();
	void Update(float dt);
	void Exit();

	Body* CreateBody(GameObject* gameObject, Transform* transform, ShapeType shape, PhysicsMaterialType material, bool calcMass);

	void SetGravity(bool mode, const vec2& gravity);
	std::pair<bool, vec2> GetGravity() const;

	void DeleteBody(Body* body);
	PhysicsWorld();

private:
	friend class Singleton<PhysicsWorld>;
	
	void UpdateKinematics(f32 dt);
	void UpdateCollision(f32 dt);
	void ResolveCollision(const Manifold& m);

	std::vector<Shape*> m_colliders;
	std::vector<Rigidbody*> m_rigidbodies;
	std::set<Manifold> m_contacts;
	PoolAllocator<AABB> m_aabbPool;
	PoolAllocator<Circle> m_circlePool;
	PoolAllocator<Rigidbody> m_rigidbodyPool;
	PoolAllocator<Body> m_bodyPool;

	// gravity is:
	// true: a direction (m_gravity is a normalized direction vector)
	// false: towards a point (m_gravity is a point in space that objects fall towards)
	bool m_gravityMode;
	vec2 m_gravity;
};

#define GRAVITY_MODE_DIRECTION true
#define GRAVITY_MODE_POINT false

#define ROTATIONAL_DRAG 8.f

namespace Dandelion
{
	DANDELION_API Body* CreatePhysicsBody(GameObject* gameObject, ShapeType shape, PhysicsMaterialType material, bool calcMass);
	DANDELION_API void DeletePhysicsBody(Body* body);

	DANDELION_API void SetGravity(bool mode, const vec2& gravity);
	DANDELION_API std::pair<bool, vec2> GetGravity();
}