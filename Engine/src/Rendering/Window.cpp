#include "Rendering/Window.h"

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <iostream>

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
}

void Window::FinaliseAppUpdate()
{
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
