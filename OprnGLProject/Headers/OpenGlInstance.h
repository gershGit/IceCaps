#include "Headers.h"

class GLInstance {
private:
	
public:
	GLFWwindow * window;
	GLInstance();
	~GLInstance();

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void processInput(GLFWwindow* window);
	void initialize();
	void initGlad();
	void createWindow(const char* windowname = "IceCaps", int height = 600, int width = 800);
};