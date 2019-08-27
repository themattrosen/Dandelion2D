#include "GameObject.h"

GameObject::~GameObject()
{
	m_scripts.clear();
	if (m_body)
		DetachBody();
}

void GameObject::Update(float dt)
{
	for (auto& script : m_scripts)
	{
		script.second.update(dt, this);
	}
}

void GameObject::Draw()
{
	m_sprite.Draw();
}

void GameObject::OnCollisionEnter(GameObject * other)
{
	for (auto& script : m_scripts)
	{
		script.second.onCollisionEnter(this, other);
	}
}

void GameObject::OnCollisionExit(GameObject * other)
{
	for (auto& script : m_scripts)
	{
		script.second.onCollisionExit(this, other);
	}
}
