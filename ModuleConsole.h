#pragma once
#include "Module.h"
#include "Globals.h"

#include <iostream>
#include <vector>
#include <string>


class ModuleConsole : public Module
{
public:
	ModuleConsole();
	~ModuleConsole();

	bool Init();
	update_status Update();
	bool CleanUp();

	void AddToConsole(const char* log);
	void AddToConsole(std::string log);
	void WriteInConsole();


	std::vector<const char*> consoleLog = {};
};

