#pragma once

#include <Core\Definitions.h>
#include <functional>
#include <unordered_map>

class GameObject;

using ScriptUpdate = std::function<void(float dt, GameObject* parent)>;
using ScriptOnCollisionEnter = std::function<void(GameObject* thisObject, GameObject* other)>;
using ScriptOnCollisionExit = std::function<void(GameObject* thisObject, GameObject* other)>;

struct Script
{
	string name;
	std::unordered_map<string, f32> numVariables;
	std::unordered_map<string, void*> refVariables;
	ScriptUpdate update;
	ScriptOnCollisionEnter onCollisionEnter;
	ScriptOnCollisionExit onCollisionExit;
};

// script creation macros
#define BEGIN_SCRIPT(name) { #name,  

#define SCRIPT_VALUES() 

#define SCRIPT_VALUE(name, value) { #name, value }

#define SCRIPT_REFS() , 

#define SCRIPT_REF(name, ref_ptr) { #name, ref_ptr }

#define SCRIPT_UPDATE , [&]

#define SCRIPT_ON_COLLISION_ENTER , [&]

#define SCRIPT_ON_COLLISION_EXIT , [&]

#define END_SCRIPT }

// script internal macros
#define VALUE(name, scriptName) parent->GetScript(#scriptName).numVariables[#name]
#define REF(name, scriptName) parent->GetScript(#scriptName).refVariables[#name] 

/*
	usage:
	Script exampleScript = BEGIN_SCRIPT(ExampleScript)
	
	SCRIPT_VALUES()
	{
		SCRIPT_VALUE(val, 0.2f),
		SCRIPT_VALUE(example, 1000.f)
	}

	SCRIPT_REFS()
	{
		SCRIPT_REF(exampleRef, obj)
	}

	SCRIPT_UPDATE(float dt, GameObject* parent)
	{
		std::cout << "Example script!" << std::endl;
		std::cout << "dt: " << dt << std::endl;
		std::cout << "val: " << VALUE(val, exampleScript) << std::endl;
		++VALUE(val, exampleScript);
	}

	SCRIPT_ON_COLLISION_ENTER(GameObject* parent, GameObject* other) 
	{
		std::cout << parent->GetName() << " started collision with " << other->GetName() << std::endl;
	}

	SCRIPT_ON_COLLISION_EXIT(GameObject* parent, GameObject* other)
	{
		std::cout << parent->GetName() << " stopped collision with " << other->GetName() << std::endl;
	}
	END_SCRIPT;
	
	object->AttachScript(exampleScript);
	

*/
