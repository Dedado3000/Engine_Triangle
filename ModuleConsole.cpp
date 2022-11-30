#include "ModuleConsole.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

ModuleConsole::ModuleConsole() {

}

ModuleConsole::~ModuleConsole() {

}

bool ModuleConsole::Init() {
	
	const char* log = "Welcome to the Console";
	consoleLog.push_back(log);
	return true;
}

update_status ModuleConsole::Update() {

	return UPDATE_CONTINUE;
}

bool ModuleConsole::CleanUp() {
	return true;
}

void ModuleConsole::AddToConsole(const char* log) {
	consoleLog.push_back(log);
}

void ModuleConsole::AddToConsole(std::string log) {
	const char* str = log.c_str();
	consoleLog.push_back(str);
}


void ModuleConsole::WriteInConsole() 
{
	for (const char* log : consoleLog)
	{
		ImGui::Text(log);
	}
}