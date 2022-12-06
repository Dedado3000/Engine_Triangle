#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleConsole.h"
#include "SDL.h"
#include "Application.h"
#include "Application.h"
#include "GL\glew.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

#include "ModuleRenderExercise.h"

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

	geometryLoaded = "";
	textureLoaded = "";
	triangleCount = "";
	textureSize = "";
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

		if (ImGui::Button("exit"))  
			return UPDATE_STOP;

		if (ImGui::MenuItem("About"))
		{
			App->console->AddToConsole("Visit the github page: https://github.com/Dedado3000/Engine_Triangle");
			ShellExecute(0, 0, "https://github.com/Dedado3000/Engine_Triangle", 0, 0, SW_SHOW);
		}


		if (ImGui::MenuItem("Editor"))
			editorWindows = !editorWindows;
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



		if (ImGui::Begin("HardWare & Software"))
		{
			ImGui::Text("Hardware");
			//CPU
			string sCpu = "CPU number: " + to_string(SDL_GetCPUCount());
			ImGui::Text(sCpu.c_str());
			//Ram
			int ramM = SDL_GetSystemRAM();
			int ramG = ramM / 1024;
			string sRam = "System Ram: " + to_string(ramG) + "." + to_string(ramM - ramG * 1024) + " GB";
			ImGui::Text(sRam.c_str());
			//GPU
			//ImGui::Text(glGetString(GL_RENDERER));
			string sGpu = SDL_GetCurrentVideoDriver();
			sGpu = "GPU: " + sGpu;
			ImGui::Text(sGpu.c_str());
			//Video Usage
			//int vUs = 0;
			//string sURam = "Ram Usage: " + to_string(vUs);
			//ImGui::Text(sURam.c_str());

			ImGui::Text("Caps: ");
			string caps ="";
			if (SDL_Has3DNow()) caps+="3DNow,";
			if (SDL_HasAltiVec()) caps += "Altivec,";
			if (SDL_HasAVX()) caps += "AVX,";
			if (SDL_HasAVX2()) caps += "AVX2,";
			if (SDL_HasMMX()) caps += "MMX,";
			if (SDL_HasRDTSC()) caps += "RDTSC,";
			if (SDL_HasSSE()) caps += "SSE,";
			if (SDL_HasSSE2()) caps += "SSE2,";
			if (SDL_HasSSE3()) caps += "SSE3,";
			if (SDL_HasSSE41()) caps += "SSE41,";			
			if (SDL_HasSSE()) caps += "SSE42,";
			
			ImGui::SameLine();
			ImGui::TextWrapped(caps.c_str());

			ImGui::NewLine();
			ImGui::Text("Software");
			//SDL version
			
			SDL_version* ver = new SDL_version();
			SDL_VERSION(ver);
			string sSdl = "SDL Version: Version " + to_string(ver->major) + "." + to_string(ver->minor) + "." + to_string(ver->patch);
			ImGui::TextWrapped(sSdl.c_str());

			//OPENGL version
			const GLubyte * open = glGetString(GL_VERSION);
			std::string sOpen((char*)open);
			sOpen = "OpenGL Version: " + sOpen;
			ImGui::TextWrapped(sOpen.c_str());
			//Assimp Version
			string sAssimp = "Assimp Version: assimp-5.2.5";
			ImGui::TextWrapped(sAssimp.c_str());


		}
		ImGui::End();

		//b. Configuration for all variables on each module (renderer, window, input and textures)

		if (ImGui::Begin("Modules"))
		{
			const Uint8* keyboard = App->input->keyboard;
			ImGui::Text("---Windows Module---");
			ImGui::SliderInt("Width", &App->window->width, 100, App->window->maxWidth);
			ImGui::SliderInt("Heigth", &App->window->height, 100, App->window->maxHeight);
			ImGui::Text("---Camera Module---");
			ImGui::SliderInt("Movement Speed", &App->camera->movementSpeed, 1, 10);
			ImGui::SliderInt("Rotation Speed", &App->camera->rotationSpeed, 1, 10);
			ImGui::Text("Camera mode");
			if (App->camera->freeLook == 1)
			{
				if (ImGui::Button("Normal Mode"))
					App->camera->freeLook = -1;
			}
			else
				if (ImGui::Button("Inverted Mode"))
					App->camera->freeLook = 1;
		
		}
		ImGui::End();


	}

	if (ImGui::Begin("Console"))
	{
		App->console->WriteInConsole();
	}
	ImGui::End();

	//Model Properties
	if (ImGui::Begin("Properties"))
	{
		App->renderExercise->chargedModel.GetGeometry(geometryLoaded, textureLoaded, triangleCount, textureSize);
		string geo = "Geometry name: " + geometryLoaded;
		string count = "Currently loaded: " + triangleCount + " triangles" ;
		string tex = "Texture Name: " + textureLoaded;
		string size = "with a size of: " + textureSize + " pixels";



		ImGui::Text(geo.c_str());
		ImGui::Text(count.c_str());
		ImGui::Text(tex.c_str());
		ImGui::Text(size.c_str());


		ImGui::Text("Position:");
		ImGui::SliderFloat("p.X", &App->renderExercise->chargedModel.meshScene.position.x, -20.0f, 20.0f);
		ImGui::SliderFloat("p.Y", &App->renderExercise->chargedModel.meshScene.position.y, -20.0f, 20.0f);
		ImGui::SliderFloat("p.Z", &App->renderExercise->chargedModel.meshScene.position.z, -20.0f, 20.0f);

		ImGui::Text("Scale:");
		ImGui::SliderFloat("s.X", &App->renderExercise->chargedModel.meshScene.scale.x, 0.25f, 4.0f);
		ImGui::SliderFloat("s.Y", &App->renderExercise->chargedModel.meshScene.scale.y, 0.25f, 4.0f);
		ImGui::SliderFloat("s.Z", &App->renderExercise->chargedModel.meshScene.scale.z, 0.25f, 4.0f);


		if (ImGui::Button("reset"))
		{
			App->renderExercise->chargedModel.meshScene.position.x = 0;
			App->renderExercise->chargedModel.meshScene.position.y = 0;
			App->renderExercise->chargedModel.meshScene.position.z = 0;
			App->renderExercise->chargedModel.meshScene.scale.x = 1;
			App->renderExercise->chargedModel.meshScene.scale.y = 1;
			App->renderExercise->chargedModel.meshScene.scale.z = 1;
		}
		if (ImGui::Button("reset camera"))
		{
			App->camera->Relocate();
			App->camera->FocusPoint();
		}
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

