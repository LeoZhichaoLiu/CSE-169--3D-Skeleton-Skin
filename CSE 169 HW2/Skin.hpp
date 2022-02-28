//
//  Skin.hpp
//  CSE 169
//
//  Created by 刘智超 on 2021/1/22.
//  Copyright © 2021 刘智超. All rights reserved.
//

#ifndef Skin_hpp
#define Skin_hpp

#include <stdio.h>
#include "Joint.hpp"
#include "Tokenizer.h"
#include "Vertex.hpp"
#include "Skeleton.hpp"

#include "Material.hpp"

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

using namespace std;

class Skin {

  public:
    Skin (const char *file, Skeleton * skeleton2);
    ~Skin ();
    
    struct Material {
        public :
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shininess;
    };
    Material * material;
    
    Skeleton * skeleton;
    std::vector <Vertex *> points;
    std::vector <glm::mat4> binding;
    std::vector <glm::mat4> Skin_Matrix;
    
    int numVert;
    int numBind;
    int times = 0;
    
    GLuint VAO;
    GLuint VBO_positions, VBO_normals, EBO;

    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 color = glm::vec3(1.0f, 0.95f, 0.1f); 

    // triangles Information
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::ivec3> indices;
    
    void Load (const char *file);
    void Update ();
    void Draw (const glm::mat4& viewProjMtx, GLuint shader);
    
};

#endif /* Skin_hpp */
