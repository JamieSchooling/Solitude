#pragma once

#include "Solitude.h"

#include "Core/Subsystem.h"

#include <string>

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

struct GLFWwindow;

struct SLTD_API WindowProps : public SubsystemParams
{
	const char* Title = "Solace";
	int Width = 1280;
	int Height = 720;
};

enum class CursorMode
{
	Visible,
	Hidden,
	Disabled
};

class SLTD_API Window : public SingletonSubsystem<Window>
{
public:
	void Start(const SubsystemParams & params) override;
	void Shutdown() override;

	void PreAppUpdate() override;
	void FinaliseAppUpdate() override;

	bool IsOpen();
	void SetTitle(std::string title);
	void Close();

	inline float GetWidth() const { return m_width; }
	inline float GetHeight() const { return m_height; }
	inline float GetAspectRatio() const { return m_aspectRatio; }
	inline float CurrentTime() const { return m_currentTime; }
	inline float DeltaTime() const { return m_deltaTime; }

	void SetCursorMode(CursorMode mode);

	ImGuiContext* InitImGui();

	inline GLFWwindow* GetGLFWInstance() { return m_glfwInstance; }
private:
	GLFWwindow* m_glfwInstance;
	const char* m_title;
	float m_width;
	float m_height;
	float m_aspectRatio;
	float m_currentTime;
	float m_prevFrameTime;
	float m_deltaTime;
};