#pragma once
#include <string>

struct GLFWwindow;

struct application_specification {
	std::string name = "Default name";
	std::string iconpath = "resources/app/appicon.png";
	uint32_t width = 1600;
	uint32_t height = 900;
};

class application {
public:
	application(const application_specification& app_spec = application_specification());

	~application();

	void run();

	void close();

private:
	void init();

	void shutdown();

	;
private:
	int num = 0;
	bool m_running = false;
	GLFWwindow* WindowHandle = nullptr;
	application_specification m_specification;
};

application* create_application(int argc, char* argv[]);