#include "AppUtils/AppUtils.h"

void AppInit(GLFWwindow*& winobj, const char* name, const char* icon_path, float width, float height) {
	glfwInit();

	const char* glsl_version = "#version 460";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	//glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

	winobj = glfwCreateWindow(width, height, name, NULL, NULL);

	/* Make the window's context current */
	glfwMakeContextCurrent(winobj);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);//glad init

	GLFWimage images[1];
	images[0].pixels = stbi_load(icon_path, &images[0].width, &images[0].height, 0, 4); //rgba channels
	glfwSetWindowIcon(winobj, 1, images);
	stbi_image_free(images[0].pixels);

	//some imgui init stuff-------------
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();

	ImGui_ImplGlfw_InitForOpenGL(winobj, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	glViewport(0, 0, width, height);
}

void AppTerminate(GLFWwindow*& winobj) {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(winobj);
	glfwTerminate();
}

void AppImguiFrameStart() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void AppImguiFrameEnd() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

//void AppImguiAddImage(GLuint texID) {
//	ImVec2 pos = ImGui::GetCursorScreenPos();
//	ImVec2 ws = ImGui::GetContentRegionAvail();
//	ImGui::GetWindowDrawList()->AddImage(
//		(void*)texID,
//		ImVec2(ImGui::GetCursorScreenPos()),
//		ImVec2(ImGui::GetCursorScreenPos().x + ws.x,
//			ImGui::GetCursorScreenPos().y + ws.y), ImVec2(0, 1), ImVec2(1, 0));
//}