#pragma once

#include "Solitude.h"

#include <vector>

#include "Subsystem.h"

enum class UpdatePhase
{
	Always,
	Simulation
};

struct RegisteredSubsystem
{
	Subsystem* Instance;
	UpdatePhase Phase;
};

class SLTD_API Application
{
public:
	void ExecuteLifecycle();

	virtual void Initialise() = 0;
	virtual void Run() = 0;
	virtual void Shutdown() = 0;

protected:
	template<typename T, typename = std::enable_if_t<std::is_base_of<SingletonSubsystem<T>, T>::value>>
	void AddSubsystem(UpdatePhase phase, const SubsystemParams& params = {})
	{
		T& subsystem = T::Get();
		RegisteredSubsystem system;
		system.Instance = &subsystem;
		system.Phase = phase;
		subsystem.Start(params);
		m_subsystems.push_back(system);
	}

	template<typename T, typename = std::enable_if_t<std::is_base_of<SingletonSubsystem<T>, T>::value>>
	void RemoveSubsystem()
	{
		T& subsystem = T::Get();
		auto it = std::find_if(m_subsystems.begin(), m_subsystems.end(),
			[&subsystem](const RegisteredSubsystem& s)
		{
			return s.Instance == &subsystem;
		}
		);
		if (it != m_subsystems.end())
		{
			subsystem.Shutdown();
			m_subsystems.erase(it);
		}
	}

	virtual bool ShouldUpdate(const RegisteredSubsystem& subsystem) const;
	void PreUpdate();
	void Update();
	void PostUpdate();
	void FinaliseUpdate();
private:
	std::vector<RegisteredSubsystem> m_subsystems;
};

extern SLTD_API Application* CreateApplication();