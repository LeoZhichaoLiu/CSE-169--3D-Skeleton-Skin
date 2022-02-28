#ifndef _CUBE_H_
#define _CUBE_H_

#include "core.h"
#include "Material.hpp"

////////////////////////////////////////////////////////////////////////////////

class Cube
{
private:
	GLuint VAO;
	GLuint VBO_positions, VBO_normals, EBO;

	glm::mat4 model;
	glm::vec3 color;

	// Cube Information
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

public:
    Cube(glm::vec3 cubeMin=glm::vec3(-1,-1,-1), glm::vec3 cubeMax=glm::vec3(1, 1, 1));
	~Cube();
    
    struct Material {
        public :
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
    };
    Material * material;

	void draw(const glm::mat4& viewProjMtx, GLuint shader, bool solid);
	void update(glm::mat4 model1);

	void spin(float deg);
};

////////////////////////////////////////////////////////////////////////////////

#endif
