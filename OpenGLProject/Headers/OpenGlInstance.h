/*
	Class to store an opengl instance
*/

#include "Headers.h"

class GLInstance {
private:

public:
	//Window information and reference
	int W_HEIGHT = 600;
	int W_WIDTH = 800;
	GLFWwindow * window;

	//Constructor and destructor
	GLInstance();
	~GLInstance();

	//Callback to change the frame buffer size
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	//Function to process input each frame
	void processInput(GLFWwindow* window);
	//Initialize an instance
	void initialize();
	//Initialize glad
	void initGlad();
	//Create a new window of provided size
	void createWindow(const char* windowname = "IceCaps", int height = 600, int width = 800);
};