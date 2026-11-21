#include "Core/Editor.h"

#include <Core/EventSystem.h>
#include <Rendering/Window.h>

#include "Core/ProjectManager.h"

Application* CreateApplication()
{
	return new Editor();
}

void Editor::Bootstrap()
{
	{
		AddSubsystem<EventSystem>(UpdatePhase::Always);
	}

	{
		WindowProps props;
		props.Title = "Solitude Editor - Project Manager";
		AddSubsystem<Window>(UpdatePhase::Always, props);
	}

	ImGuiContext* ctx = Window::Get().InitImGui();

	ImGui::SetCurrentContext(ctx);

	std::filesystem::path projectPath;

	/*for (auto arg : args)
	{
		if (ProjectManager::IsProjectFile(arg))
		{
			projectPath = arg;
		}
	}*/

	ProjectManager projectManager;

	projectManager.Initialise(projectPath);

	while (!projectManager.IsProjectLoaded() && Window::Get().IsOpen())
	{
		PreUpdate();
		Update();
		projectManager.Update();
		PostUpdate();
		FinaliseUpdate();
	}
}

void Editor::Initialise()
{
}

void Editor::Run()
{
	while (Window::Get().IsOpen())
	{
		PreUpdate();
		Update();
		PostUpdate();
		FinaliseUpdate();
	}
}

void Editor::Shutdown()
{
	RemoveSubsystem<Window>();
	RemoveSubsystem<EventSystem>();
}
