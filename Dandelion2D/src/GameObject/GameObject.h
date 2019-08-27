#pragma once

#include "Transform\Transform.h"
#include "Sprite\Sprite.h"
#include "Script\Script.h"

#include <Physics\Body\Body.h>
#include <Physics\Shapes\Shape.h>
#include <Physics\World\PhysicsWorld.h>
#include <Graphics\Graphics.h>

#include <vector>

enum Tag : int
{
	t_Player =        0b1,
	t_Platform =      0b10,
	t_Bullet =        0b100,
	t_DynamicObject = 0b1000,
	t_StaticObject =  0b10000,
	t_Collectable =   0b100000,
	t_Lava =          0b1000000,
};

class GameObject
{
public:

	GameObject(const string& name, const vec2& pos = vec2(),
		const vec4& color = vec4(1.f, 0.f, 0.f, 1.f)) :
		m_name(name),
		m_transform(pos),
		m_sprite(&m_transform, GraphicsEngine::Instance().GetShader(), color),
		m_body(nullptr),
		m_scripts(),
		m_isArchetype(false),
		m_tag(0),
		m_next(nullptr), m_prev(nullptr) {}

	~GameObject();

	const string& GetName() const { return m_name; }

	Transform* GetTransform() { return &m_transform; }
	const vec2& GetPosition() const { return m_transform.GetPosition(); }
	const vec2& GetScale() const { return m_transform.GetScale(); }
	f32 GetRotation() const { return m_transform.GetRotation(); }

	void SetPosition(const vec2& val) { m_transform.SetPosition(val); }
	void SetScale(const vec2& val) { m_transform.SetScale(val); }
	void SetRotation(const f32& val) { m_transform.SetRotation(val); }

	void Update(float dt);
	void Draw();
	void OnCollisionEnter(GameObject* other);
	void OnCollisionExit(GameObject* other);

	Sprite* GetSprite() { return &m_sprite; }
	void SetColor(const vec4& color) { m_sprite.SetColor(color); }
	const vec4& GetColor() const { return m_sprite.GetColor(); }

	Body* GetBody() { return m_body; }
	Body* AttachBody(ShapeType shape, PhysicsMaterialType material, bool calcMass = true)
	{
		m_body = Dandelion::CreatePhysicsBody(this, shape, material, calcMass);
		return m_body;
	}

	void DetachBody()
	{
		Dandelion::DeletePhysicsBody(m_body);
		m_body = nullptr;
	}

	void AttachScript(const Script& script)
	{
		m_scripts.insert_or_assign(script.name, script);
	}

	Script& GetScript(const string& name)
	{
		return m_scripts[name];
	}

	bool IsArchetype() const { return m_isArchetype; }

	void AddTag(Tag tag)
	{
		m_tag |= (int)tag;
	}

	void RemoveTag(Tag tag)
	{
		m_tag &= ~tag;
	}

	bool HasTag(Tag tag)
	{
		return m_tag & tag;
	}

private:
	string m_name;
	Transform m_transform;
	Sprite m_sprite;
	Body* m_body;
	std::unordered_map<string, Script> m_scripts;
	bool m_isArchetype;
	s32 m_tag;

	GameObject* m_next;
	GameObject* m_prev;

	friend class GameObjectManager;
	friend class GameObjectFactory;
};
