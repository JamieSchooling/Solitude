#pragma once

#include "Core/EntryPoint.h"
#include "Core/Application.h"

class Editor : public Application
{
	void Bootstrap() override;
	void Initialise() override;
	void Run() override;
	void Shutdown() override;
};