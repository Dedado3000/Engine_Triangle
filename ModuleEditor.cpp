#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "SDL.h"
#include "Application.h"
#include "Application.h"
#include "GL\glew.h"
#include <iostream>
#include <string>
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

using namespace std;

ModuleEditor::ModuleEditor()
{
}

// Destructor
ModuleEditor::~ModuleEditor()
{
}

// Called before render is available
bool ModuleEditor::Init()
{
	/*
	*/
	return true;
}

bool ModuleEditor::Start()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->renderer->context);
	ImGui_ImplOpenGL3_Init("#version 440");

	return true;

}

update_status ModuleEditor::PreUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();



	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleEditor::Update()
{
	
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	//if (show_demo_window)
	//	ImGui::ShowDemoWindow(&show_demo_window);
	if (ImGui::BeginMainMenuBar())
	{

		if (ImGui::BeginMenu("desp"))
		{
			ImGui::EndMenu();
		}
	}
	ImGui::EndMainMenuBar();

	if (editorWindows)
	{

		if (ImGui::Begin("Timers"))
		{
			ImGui::SliderInt("max FPS", &App->fps, 10.0f, 120.0f);
			string fpsText = "Limiting to " + to_string(App->fps) + " means that each frame takes " + to_string(1000 / App->fps) + "ms";
			ImGui::Text(fpsText.c_str());
			ImGui::SliderFloat("game clock scale", &scale, 0.0f, 2.0f);


			char title[25];
			sprintf_s(title, 25, "Framerate %.1f", App->timer.fps[sizeof(App->timer.fps) - 1]);
			ImGui::PlotHistogram("##framerate", &App->timer.fps[0], 100, 0, title, 0.0f, 160.0f, ImVec2(310, 160));
		}
		ImGui::End();
	}



	if (ImGui::Begin("HardWare"))
	{
		ImGui::Text("My HardWare is");
	}
	ImGui::End();

	App->timer.SetScale(scale);

	return continueUpdate;
}

update_status ModuleEditor::PostUpdate()
{

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
	SDL_GL_MakeCurrent(App->window->window, App->renderer->context);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
	//ImGui_OpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return true;
}

