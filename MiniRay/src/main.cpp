#include <iostream>
#include <string>
//->
//!=
//~>
//++
//==>
//==
bool g_ApplicationRunning = true;

struct application_specification {
	std::string name = "Default name";
	uint32_t width = 1600;
	uint32_t height = 900;
};

class application {
public:
	application(const application_specification& app_spec = application_specification()) :m_specification(app_spec)
	{
		init();
	};

	~application()
	{
		shutdown();
	}
	void run()
	{
		m_running = true;
		while (m_running) {
			//std::cout << "Running from: " << argv[0] << "\n";
			std::cout << "\nHello World\ntype 'exit' or 'restart'\n";
			std::string input;
			std::cin >> input;
			if (input == "exit") {
				m_running = false;
				g_ApplicationRunning = false;
			}
			else if (input == "restart") {
				std::cout << "Restarting";
				close();
			}
			else
			{
				std::cerr << "input was: " << input << "\nEnter valid response. Starting over...\n";
			}
		}
	};

	void close()
	{
		m_running = false;//run depends on this
	};

private:
	void init()
	{
		//do pre setup
	};
	void shutdown()
	{
	}
	;
private:
	bool m_running = false;
	//GLFWwindow* WindowHandle=nullptr;
	application_specification m_specification;
};

//to be implemented by client
application* create_application(int argc, char* argv[]) {
	application_specification spec;
	spec.name = "cmd test app";
	auto app = new application(spec);
	return app;
}

//supposed to be separated to a entrypoint.cpp
//extern application* create_application(int arc, char* argv[])

int main(int argc, char* argv[])
{
	//g_applicationrunnning=false will completely shutdown app, using close() of app will close app instance but the int main loop will recreate another instance of app and run it(restart)
	while (g_ApplicationRunning)
	{
		auto app = create_application(argc, argv);
		app->run();
		delete app;
	}
	return 0;
}