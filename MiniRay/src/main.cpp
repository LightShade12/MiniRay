#include "application/application.h"
#include "application/entrypoint.h"
//rename file to other than main.cpp
//to be implemented by client
application* create_application(int argc, char* argv[]) {
	application_specification spec;
	spec.name = "cmd test app";
	auto app = new application(spec);
	return app;
}