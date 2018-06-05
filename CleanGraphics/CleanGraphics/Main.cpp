#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui\imgui.h>
#include <imgui\imgui_impl_glfw_gl3.h>

#include "global.h"
#include "MyGlWindow.h"

MyGlWindow *win;

bool lbutton_down;
bool rbutton_down;
bool mbutton_down;
double m_lastMouseX;
double m_lastMouseY;
double cx, cy;

void window_size_callback(GLFWwindow* window, int width, int height)
{
	win->setSize(width, height);
	win->setAspect(width / (float)height);

}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}


static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	cx = xpos;
	cy = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		m_lastMouseX = xpos;
		m_lastMouseY = ypos;
	}

	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		if (GLFW_PRESS == action)
			lbutton_down = true;
		else if (GLFW_RELEASE == action)
			lbutton_down = false;
	}

	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		if (GLFW_PRESS == action)
			rbutton_down = true;
		else if (GLFW_RELEASE == action)
			rbutton_down = false;
	}

	else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
		if (GLFW_PRESS == action)
			mbutton_down = true;
		else if (GLFW_RELEASE == action)
			mbutton_down = false;
	}
}


void mouseDragging(double width, double height)
{

	if (lbutton_down) {
		float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
		float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
		win->m_viewer->rotate(fractionChangeX, fractionChangeY);
	}
	else if (mbutton_down) {
		float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
		float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
		win->m_viewer->zoom(fractionChangeY);
	}
	else if (rbutton_down) {
		float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
		float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
		win->m_viewer->translate(-fractionChangeX, -fractionChangeY, 1);
	}



	m_lastMouseX = cx;
	m_lastMouseY = cy;

}



int main(int ac, char **av)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
	{
		std::cerr << "OpenGL Initialisation failed" << std::endl;
		return (84);
	}


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	int width = 800;
	int height = 800;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "OpenGL FrameWork", NULL, NULL);


	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	ImGui_ImplGlfwGL3_Init(window, true);

	glfwMakeContextCurrent(window);


	/* Make the window's context current */

	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		//Problem: glewInit failed, something is seriously wrong.
		std::cout << "glewInit failed, aborting." << std::endl;
		return 0;
	}


	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION));


	glfwSwapInterval(1);  //enable vsync

	win = new MyGlWindow(width, height, av[1]);

	bool show_test_window = true;
	bool show_another_window = false;


	double previousTime = glfwGetTime();
	int frameCount = 0;
	double lastTime;



	glfwSetWindowSizeCallback(window, window_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetKeyCallback(window, key_callback);


	glfwSetWindowTitle(window, "myGlWindow");



	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		ImGui_ImplGlfwGL3_NewFrame();
		//ImGui::SliderFloat("Light X", &global::lightPosition.x, 0.0f, 20.0f);
		//ImGui::SliderFloat("Light Y", &global::lightPosition.y, 0.0f, 20.0f);
		//ImGui::SliderFloat("Light Z", &global::lightPosition.z, 0.0f, 200.0f);
		//ImGui::SliderFloat("Linear attenuation", &global::Linear, 0.0f, 0.1f);
		//ImGui::SliderFloat("Quadratic attenuation", &global::Quadratic, 0.0f, 2.0f);
		ImGui::SliderFloat("Gamma Correction", &global::gammaCorrection, 0, 10);
		ImGui::Checkbox("Use HDR", &global::useHDR);

		// Rendering
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);


		glClearColor(0.2, 0.2, 0.2, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		win->draw();
		ImGui::Render();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
		glfwWaitEvents();
		if (!ImGui::IsMouseHoveringAnyWindow())
			mouseDragging(display_w, display_h);

	}

	ImGui_ImplGlfwGL3_Shutdown();

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}