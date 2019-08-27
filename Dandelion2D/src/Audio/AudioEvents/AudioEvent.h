#pragma once
#include <Core\Definitions.h>

enum AudioEventType
{
	aet_PlayEvent,
	aet_StopEvent,
};

struct AudioEvent
{
	AudioEventType type;
	string name;

	struct
	{
		bool isLooped = false;
	} params;
};