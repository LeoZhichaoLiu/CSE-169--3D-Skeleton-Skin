////////////////////////////////////////
// Window.cpp
////////////////////////////////////////

#include "Window.h"

////////////////////////////////////////////////////////////////////////////////

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "CSE 169 Starter";

// Objects to render
Cube * Window::cube;
Skeleton * Window::skeleton;
Skin * Window::skin;

Skeleton * Window::test;
Skeleton * Window::dragon;
bool Window::solid;

string Window::file;
string Window::file2;
bool Window::isSkel;
bool Window::isSkin;

PointLight * Window::SphereLight;
SpotLight * Window::SphereLight2;

bool Window::only_point;
bool Window::only_spot;
int Window::iterate = 0;

// Camera Properties
Camera* Cam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;
GLuint Window::lightProgram;


////////////////////////////////////////////////////////////////////////////////

// Constructors and desctructors 
bool Window::initializeProgram() {

	// Create a shader program with a vertex shader and a fragment shader.
	shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    lightProgram = LoadShaders("shaders/shader2.vert", "shaders/shader2.frag");

	// Check the shader program.
	if (!shaderProgram)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}
    
    if (!lightProgram) {
        std::cerr << "Failed to initialize light program" << std::endl;
        return false;
    }

	return true;
}

bool Window::initializeObjects()
{
	// Create a cube
	cube = new Cube();
	//cube = new Cube(glm::vec3(-1, 0, -2), glm::vec3(1, 1, 1));
    
    string prefix = "SKEL/";
    
    if (isSkel) {
      const char * input = (prefix + file).c_str();
      skeleton = new Skeleton (input);
    }
    
    if (isSkin && isSkel) {
      const char * input_skin = (prefix + file2).c_str();
      skin = new Skin (input_skin, skeleton);
        
    } else if (isSkin && !isSkel) {
      const char * input_skin = (prefix + file2).c_str();
      skin = new Skin (input_skin, skeleton);
    }
        
    
    SphereLight = new PointLight ("OBJ/sphere.obj");
    
    SphereLight2 = new SpotLight ("OBJ/sphere.obj");
    
    solid = true;
    only_point = true;
    only_spot = false;
    
    if (isSkel) {
        cout << "The Joint: " << skeleton->Joint_List[iterate]->name << "    The DOF Value: "
             << skeleton->Joint_List[iterate]->List1[0]->getValue() << " " << skeleton->Joint_List[iterate]->List1[1]->getValue()
             << " " << skeleton->Joint_List[iterate]->List1[2]->getValue() << endl;
    }

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete cube;
    delete test;
    delete dragon;

	// Delete the shader program.
	glDeleteProgram(shaderProgram);
    glDeleteProgram(lightProgram);
}

////////////////////////////////////////////////////////////////////////////////

// for the Window
GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// set up the camera
	Cam = new Camera();
	Cam->SetAspect(float(width) / float(height));

	// initialize the interaction variables
	LeftDown = RightDown = false;
	MouseX = MouseY = 0;

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	Cam->SetAspect(float(width) / float(height));
}

////////////////////////////////////////////////////////////////////////////////

// update and draw functions
void Window::idleCallback()
{
	// Perform any updates as necessary. 
	Cam->Update();
    
    if (isSkel) {
      skeleton->Update();
    }
    
    if (isSkin) {
      skin->Update();
    }

	//cube->update();
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	

	// Render the object.
	glUseProgram(lightProgram);
    glUniform1i(glGetUniformLocation(lightProgram, "only_point"), only_point);
    glUniform1i(glGetUniformLocation(lightProgram, "only_spot"), only_spot);
    
    SphereLight->draw(Cam->GetViewProjectMtx(), lightProgram);
    SphereLight2->draw(Cam->GetViewProjectMtx(), lightProgram);
    
    if (isSkel && !isSkin) {
       skeleton->Draw(Cam->GetViewProjectMtx(), lightProgram, solid);
    }
    
    if (isSkin) {
      skin->Draw(Cam->GetViewProjectMtx(), lightProgram);
    }
    
	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

////////////////////////////////////////////////////////////////////////////////

// helper to reset the camera
void Window::resetCamera() 
{
	Cam->Reset();
	Cam->SetAspect(float(Window::width) / float(Window::height));
}

////////////////////////////////////////////////////////////////////////////////

// callbacks - for Interaction 
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */
	
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key) 
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;

		case GLFW_KEY_R:
			resetCamera();
			break;
        
        case GLFW_KEY_7:
            solid = true;
            break;
                
        case GLFW_KEY_8:
            solid = false;
            break;
                
        case GLFW_KEY_1:
            only_point = true;
            only_spot = false;
            break;
        
        case GLFW_KEY_2:
            only_point = false;
            only_spot = true;
            break;
        
        case GLFW_KEY_3:
            only_point = true;
            only_spot = true;
            break;
        
        case GLFW_KEY_4:
            only_point = false;
            only_spot = false;
            break;
                
        case GLFW_KEY_UP: {
            if (iterate > 0) {
               iterate--;
            }
            
            if (isSkel) {
                cout << "The Joint: " << skeleton->Joint_List[iterate]->name << "    The DOF Value: "
                     << skeleton->Joint_List[iterate]->List1[0]->getValue() << " " << skeleton->Joint_List[iterate]->List1[1]->getValue()
                      << " " << skeleton->Joint_List[iterate]->List1[2]->getValue() << endl;
            }
            
            break;
        }
        
        case GLFW_KEY_DOWN: {
            if (iterate < skeleton->Joint_List.size()-1) {
               iterate++;
            }
            
            if (isSkel) {
               cout << "The Joint: " << skeleton->Joint_List[iterate]->name << "    The DOF Value: "
                    << skeleton->Joint_List[iterate]->List1[0]->getValue() << " " << skeleton->Joint_List[iterate]->List1[1]->getValue()
                    << " " << skeleton->Joint_List[iterate]->List1[2]->getValue() << endl;
            }
            
            break;
        }
        
        case GLFW_KEY_RIGHT: {
            float free_1 = skeleton->Joint_List[iterate]->List1[0]->getValue();
            float free_2 = skeleton->Joint_List[iterate]->List1[1]->getValue();
            float free_3 = skeleton->Joint_List[iterate]->List1[2]->getValue();
            skeleton->Joint_List[iterate]->List1[0]->setValue(free_1 + 0.1);
            skeleton->Joint_List[iterate]->List1[1]->setValue(free_2 + 0.1);
            skeleton->Joint_List[iterate]->List1[2]->setValue(free_3 + 0.1);
            
            if (isSkel) {
               cout << "The Joint: " << skeleton->Joint_List[iterate]->name << "    The DOF Value: "
                    << skeleton->Joint_List[iterate]->List1[0]->getValue() << " " << skeleton->Joint_List[iterate]->List1[1]->getValue()
                    << " " << skeleton->Joint_List[iterate]->List1[2]->getValue() << endl;
            }
            
            break;
        }
                
        case GLFW_KEY_LEFT: {
            float free_1 = skeleton->Joint_List[iterate]->List1[0]->getValue();
            float free_2 = skeleton->Joint_List[iterate]->List1[1]->getValue();
            float free_3 = skeleton->Joint_List[iterate]->List1[2]->getValue();
            skeleton->Joint_List[iterate]->List1[0]->setValue(free_1 - 0.1);
            skeleton->Joint_List[iterate]->List1[1]->setValue(free_2 - 0.1);
            skeleton->Joint_List[iterate]->List1[2]->setValue(free_3 - 0.1);
            
            if (isSkel) {
                cout << "The Joint: " << skeleton->Joint_List[iterate]->name << "    The DOF Value: "
                     << skeleton->Joint_List[iterate]->List1[0]->getValue() << " " << skeleton->Joint_List[iterate]->List1[1]->getValue()
                     << " " << skeleton->Joint_List[iterate]->List1[2]->getValue() << endl;
            }
            
            break;
        }
                
		default:
			break;
		}
	}
}

void Window::mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		LeftDown = (action == GLFW_PRESS);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		RightDown = (action == GLFW_PRESS);
	}
}


void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    //skeleton -> Scale(pow (1.1, yoffset));
}

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {

	int maxDelta = 100;
	int dx = glm::clamp((int)currX - MouseX, -maxDelta, maxDelta);
	int dy = glm::clamp(-((int)currY - MouseY), -maxDelta, maxDelta);

	MouseX = (int)currX;
	MouseY = (int)currY;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if (LeftDown) {
		const float rate = 1.0f;
		Cam->SetAzimuth(Cam->GetAzimuth() + dx * rate);
		Cam->SetIncline(glm::clamp(Cam->GetIncline() - dy * rate, -90.0f, 90.0f));
	}
	if (RightDown) {
		const float rate = 0.005f;
		float dist = glm::clamp(Cam->GetDistance() * (1.0f - dx * rate), 0.01f, 1000.0f);
		Cam->SetDistance(dist);
	}
}

////////////////////////////////////////////////////////////////////////////////
