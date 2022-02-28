#ifndef _WINDOW_H_
#define _WINDOW_H_

#include "main.h"
#include "Cube.h"
#include "shader.h"
#include "Camera.h"
#include "Skeleton.hpp"
#include "Skin.hpp"
#include "Joint.hpp"
#include "DOF.hpp"
#include "PointLight.hpp"
#include "SpotLight.hpp"

////////////////////////////////////////////////////////////////////////////////

class Window
{
public:
	// Window Properties
	static int width;
	static int height;
	static const char* windowTitle;
    static string file;
    static string file2;
    static bool only_point;
    static bool only_spot;
    static bool isSkel;
    static bool isSkin;
    static int iterate;

	// Objects to render
	static Cube* cube;
    static Skeleton* skeleton;
    static Skin* skin;
    static Skeleton* test;
    static Skeleton* dragon;
    static PointLight* SphereLight;
    static SpotLight* SphereLight2;
    
    static bool solid;

	// Shader Program 
	static GLuint shaderProgram;
    static GLuint lightProgram;

	// Act as Constructors and desctructors 
	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();

	// for the Window
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);

	// update and draw functions
	static void idleCallback();
	static void displayCallback(GLFWwindow*);

	// helper to reset the camera
	static void resetCamera();

	// callbacks - for interaction
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void mouse_callback(GLFWwindow* window, int button, int action, int mods);
	static void cursor_callback(GLFWwindow* window, double currX, double currY);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

};

////////////////////////////////////////////////////////////////////////////////

#endif
