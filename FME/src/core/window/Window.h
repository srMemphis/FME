#pragma once
#include <memory>
#include <string>


struct platformcontext;

class Window
{
public:
	Window();
	~Window();

private:

	std::string m_title;
	uint32_t m_width;
	uint32_t m_height;
	
	std::unique_ptr<platformcontext> m_context;
};
