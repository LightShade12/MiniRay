#pragma once
extern application* create_application(int argc, char* argv[]);
bool g_ApplicationRunning = true;

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
