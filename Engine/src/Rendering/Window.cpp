#include "Rendering/Window.h"

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Core/Event.h"
#include "Core/EventSystem.h"

void Window::Start(const SubsystemParams& params)
{
	const WindowProps& props = static_cast<const WindowProps&>(params);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_DEPTH_BITS, 24);

	m_glfwInstance = glfwCreateWindow(props.Width, props.Height, props.Title, NULL, NULL);
	glfwSetWindowUserPointer(m_glfwInstance, static_cast<void*>(this));

	m_title = props.Title;
	m_width = props.Width;
	m_height = props.Height;
	m_aspectRatio = m_width / m_height;

	if (m_glfwInstance == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(m_glfwInstance);

	if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}

	glViewport(0, 0, props.Width, props.Height);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glfwSetWindowCloseCallback(m_glfwInstance, [](GLFWwindow* window)
	{
		Event e;
		e.Type = EventType::WindowClose;
		EventSystem::Get().DispatchEvent(e);
		if (e.Handled)
		{
			glfwSetWindowShouldClose(window, GLFW_FALSE);
		}
	});

	glfwSetFramebufferSizeCallback(m_glfwInstance, [](GLFWwindow* window, int width, int height)
	{
		auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		glViewport(0, 0, width, height);
		self->m_width = width;
		self->m_height = height;
		self->m_aspectRatio = self->m_width / self->m_height;

		Event e;
		e.Type = EventType::WindowResize;
		e.WindowResizeArgs.Width = width;
		e.WindowResizeArgs.Height = height;
		EventSystem::Get().DispatchEvent(e);
	});

	glfwSetWindowPosCallback(m_glfwInstance, [](GLFWwindow* window, int x, int y)
	{
		Event e;
		e.Type = EventType::WindowMove;
		e.WindowMoveArgs.PosX = x;
		e.WindowMoveArgs.PosY = y;
		EventSystem::Get().DispatchEvent(e);
	});

	glfwSetWindowFocusCallback(m_glfwInstance, [](GLFWwindow* window, int focused)
	{
		Event e;
		e.Type = focused == GLFW_TRUE ? EventType::WindowFocus : EventType::WindowFocusLost;
		EventSystem::Get().DispatchEvent(e);
	});

	glfwSetKeyCallback(m_glfwInstance, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Event e;
		e.Type = EventType::InputKey;
		e.InputKeyArgs.Key = key;
		e.InputKeyArgs.Action = action;
		EventSystem::Get().DispatchEvent(e);
	});

	glfwSetMouseButtonCallback(m_glfwInstance, [](GLFWwindow* window, int button, int action, int mods)
	{
		Event e;
		e.Type = EventType::InputMouseButton;
		e.InputMouseButtonArgs.Button = button;
		e.InputMouseButtonArgs.Action = action;
		EventSystem::Get().DispatchEvent(e);
	});

	glfwSetCursorPosCallback(m_glfwInstance, [](GLFWwindow* window, double xpos, double ypos)
	{
		Event e;
		e.Type = EventType::InputMouseMove;
		e.InputMouseMoveArgs.MouseX = xpos;
		e.InputMouseMoveArgs.MouseY = ypos;
		EventSystem::Get().DispatchEvent(e);
	});
}

void Window::Shutdown()
{
	glfwTerminate();
}

void Window::PreAppUpdate()
{
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_currentTime = glfwGetTime();
	m_deltaTime = m_currentTime - m_prevFrameTime;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Window::FinaliseAppUpdate()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	m_prevFrameTime = m_currentTime; 
	glFinish();
	glfwSwapBuffers(m_glfwInstance);
	glfwPollEvents();
}

bool Window::IsOpen()
{
	return !glfwWindowShouldClose(m_glfwInstance);
}

void Window::SetTitle(std::string title)
{
	glfwSetWindowTitle(m_glfwInstance, title.c_str());
}

void Window::Close()
{
	glfwSetWindowShouldClose(m_glfwInstance, GLFW_TRUE);
}

void Window::SetCursorMode(CursorMode mode)
{
	switch (mode)
	{
	case CursorMode::Visible:
		glfwSetInputMode(m_glfwInstance, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;
	case CursorMode::Hidden:
		glfwSetInputMode(m_glfwInstance, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		break;
	case CursorMode::Disabled:
		glfwSetInputMode(m_glfwInstance, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		break;
	}
}

ImGuiContext* Window::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGuiContext* ctx = ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(m_glfwInstance, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	return ctx;
}
