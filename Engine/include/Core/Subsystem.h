#pragma once

#include "Solitude.h"

struct SLTD_API SubsystemParams {};

class SLTD_API Subsystem
{
public:
	virtual void Start(const SubsystemParams& params) = 0;
	virtual void Shutdown() = 0;

	virtual void PreAppUpdate() {}
	virtual void OnAppUpdate() {}
	virtual void PostAppUpdate() {}
	virtual void FinaliseAppUpdate() {}
protected:
	Subsystem() = default;
	~Subsystem() = default;
};

template<typename Derived>
class SingletonSubsystem : public Subsystem
{
public:
	static Derived& Get()
	{
		static Derived sInstance;
		return sInstance;
	}
protected:
	SingletonSubsystem() = default;
	~SingletonSubsystem() = default;
};