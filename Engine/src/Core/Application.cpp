#include "Core/Application.h"

void Application::ExecuteLifecycle()
{
	Initialise();
	Run();
	Shutdown();
}

bool Application::ShouldUpdate(const RegisteredSubsystem& subsystem) const
{
	return true;
}

void Application::PreUpdate()
{
	for (auto subsystem : m_subsystems)
	{
		if (ShouldUpdate(subsystem))
		{
			subsystem.Instance->PreAppUpdate();
		}
	}
}

void Application::Update()
{
	for (auto subsystem : m_subsystems)
	{
		if (ShouldUpdate(subsystem))
		{
			subsystem.Instance->OnAppUpdate();
		}
	}
}

void Application::PostUpdate()
{
	for (auto subsystem : m_subsystems)
	{
		if (ShouldUpdate(subsystem))
		{
			subsystem.Instance->PostAppUpdate();
		}
	}
}

void Application::FinaliseUpdate()
{
	for (auto subsystem : m_subsystems)
	{
		if (ShouldUpdate(subsystem))
		{
			subsystem.Instance->FinaliseAppUpdate();
		}
	}
}