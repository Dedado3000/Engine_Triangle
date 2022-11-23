#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleDummy : public Module
{
	bool Init()
	{
		LOG_APP("Dummy Init!");
		return true;
	}

	bool CleanUp()
	{
		LOG_APP("Dummy CleanUp!");
		return true;
	}
};