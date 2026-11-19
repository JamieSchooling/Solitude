#include "Core/Editor.h"

#include <Core/EventSystem.h>
#include <Rendering/Window.h>

Application* CreateApplication()
{
	return new Editor();
}

void Editor::Initialise()
{
	{
		AddSubsystem<EventSystem>(UpdatePhase::Always);
	}

	{
		WindowProps props;
		props.Title = "Solitude Editor";
		AddSubsystem<Window>(UpdatePhase::Always, props);
	}
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
