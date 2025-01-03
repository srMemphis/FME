#include "pch.h"

#include "core/window/Window.h"

struct platformcontext
{
	int i = 1;
};

Window::Window()
{
	m_context = std::make_unique<platformcontext>();
}

Window::~Window()
{
}
