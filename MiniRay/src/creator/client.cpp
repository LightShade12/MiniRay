#include "application/core/application.h"
#include "entrypoint.h"
#include "../extern/imgui/imgui.h"
#include "../extern/imgui/imgui_internal.h"
#include <iostream>
#include "glm/glm.hpp"
#include "application/console/console.h"
#include "application/outputlogger/outputlogger.h"
#include "application/core/intern/image/image.h"
#include "opengl/glutils.h"
#include "application/EditorLayer/EditorLayer.h"
#include "application/nodeinspector/nodeinspector.h"
#include "application/attributemanager/attributemanager.h"
#include "application/devwindow/devwindow.h"

//to be implemented by client

application* create_application(int argc, char* argv[]) {
	std::cout << "Running from: " << argv[0] << "\n";
	
	application_specification spec;

	spec.name = "Miniray";
	spec.iconpath = "resources/vgui/appicon.png";
	auto app = new application(spec);

	app->PushLayer<EditorLayer>();
	app->PushLayer<DevWindowLayer>();

	return app;
}