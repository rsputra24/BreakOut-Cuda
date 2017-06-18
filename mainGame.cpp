

//#include "gl3w\gl3w.h"
//#include <GLFW\glfw3.h>
#include "mainGame.h"
#include <gl\glew.h>
#include <iostream>
#include <assert.h>
#include "math.h"
//#include <imgui\imgui_impl_glfw_gl3.h>
#if defined(__APPLE__) || defined(MACOSX) 
#else
	#include <gl\freeglut.h>
#endif


mainGame::mainGame() {

}
mainGame::~mainGame() {
	ImGui_ImplSdlGL3_Shutdown();
}

//Breakout game;


// Define the display function
void myDisplay()
{
	//game.display();
}

// Define the reshape function
void myReshape(int width, int height)
{
	//game.reshape(width, height);
}

// Define the mouse click events
void myMouseClick(int button, int state, int x, int y)
{
	//game.mouseClick(button, state, x, y);
}

// Define the mouse drag events
void myMouseMove(int x, int y)
{
	//game.mouseMove(x, y);
}

// Define keystroke events
void myKeyStroke(unsigned char key, int x, int y)
{
	//game.keyStroke(key, x, y);
}

void mySpecialKeyStroke(int key, int x, int y)
{
	//game.specialKeyPos(key, x, y);
}

#undef main

// The main function here
/*int main(int argc, char ** argv)
{ 
	// Init glut
	glutInit(&argc, argv);
	

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
	// Init display mode

	// Init window size, position, title
	glutInitWindowSize(WINWIDTH, WINHEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(WINTITLE);
	
    
	// Init game
	game.init();

	// Draw scene
	glutDisplayFunc(myDisplay);
	// Handle reshape
	glutReshapeFunc(myReshape);
	// Handle mouse clicks
	glutMouseFunc(myMouseClick);
	// Handle mouse motion
//	glutMotionFunc(myMouseMove);
    glutPassiveMotionFunc(myMouseMove);
	// Handle keyboard strokes
	glutKeyboardFunc(myKeyStroke);
	// specify keyboard special key input events
	glutSpecialFunc(mySpecialKeyStroke);
	// Enter the opengl event processing loop

	glutMainLoop();



	return 0;
}
*/

void mainGame::OnUserDefinedEvent(SDL_Event& evt) {
	ImGui_ImplSdlGL3_ProcessEvent(&evt);
}

static void error_callback(int error, const char* description)
{
	fprintf(stderr, "Error %d: %s\n", error, description);
}

int main(int, char**)
{
	//glewInit();
	//ImGui_ImplSdlGL3_Init(window);
	Engine::Breakout &breakout = mainGame();
	breakout.startGame("Filkom PETJAH!!!", 800, 600, true, WindowFlag::WINDOWED, 60, 1);
	
	return 0;
	// Contoh untuk glfw window
	/*// Setup window
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		return 1;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui OpenGL3 example", NULL, NULL);
	glfwMakeContextCurrent(window);
	gl3wInit();

	// Setup ImGui binding
	ImGui_ImplGlfwGL3_Init(window, true);

	// Load Fonts
	// (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
	//ImGuiIO& io = ImGui::GetIO();
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	bool play_scene = false;
	bool score_scene = false;
	bool exit_scene = false;

	ImVec4 clear_color = ImColor(0.2f, 0.2f, 0.2f);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		ImGui_ImplGlfwGL3_NewFrame();

		// 1. Show a simple window
		// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
		{

			static float f = 0.0f;
			ImGui::Begin("Filkom Petjah");
			ImGui::Text("Hello, world!");
			//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
			//ImGui::ColorEdit3("clear color", (float*)&clear_color);
			if (ImGui::Button("Mulai Bermain")) play_scene ^= 1;
			if (ImGui::Button("Lihat Skor")) score_scene ^= 1;
			if (ImGui::Button("Lihat Skor")) exit_scene ^= 1;
			ImGui::End();
		}

		// 2. Show another simple window, this time using an explicit Begin/End pair
		if (play_scene)
		{
			ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
			ImGui::Begin("Another Window", &score_scene);
			ImGui::Text("Hello");
			ImGui::End();
		}

		// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
		if (score_scene)
		{
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
			ImGui::ShowTestWindow(&play_scene);
		}

		if (exit_scene) {
			ImGui::Shutdown();
		}

		// Rendering
		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		ImGui::Render();
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplGlfwGL3_Shutdown();
	glfwTerminate();

	return 0;*/
}