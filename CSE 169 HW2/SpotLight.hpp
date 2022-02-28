//
//  SpotLight.hpp
//  CSE 167
//
#ifndef SpotLight_hpp
#define SpotLight_hpp

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <iostream>
#include <vector>
#include "Material.hpp"
#include <string>


class SpotLight {
   
   private:
      std::vector<glm::vec3> points;
      std::vector<glm::vec3> normal;
      std::vector<glm::ivec3> face;

      GLuint VAO, VBO[2];
      GLuint EBO;
      GLfloat pointSize;
      glm::mat4 model;
      glm::vec3 color;
      glm::vec3 position;
    
   public:
    
   struct Material {
       
    public:
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
    };
    Material* material;
    
    SpotLight(std::string objFilename);
    ~SpotLight();
    
    void draw(const glm::mat4& viewProj, GLuint shader);
    void update();
    void rotate(float degree, glm::vec3 axis);
    void move (double yoffset);
};

#endif /* Light_hpp */
