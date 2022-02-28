//
//  Joint.hpp
//  CSE 169
//
//  Created by 刘智超 on 2021/1/9.
//  Copyright © 2021 刘智超. All rights reserved.
//

#ifndef Joint_hpp
#define Joint_hpp

#include <stdio.h>
#include "DOF.hpp"
#include "Tokenizer.h"
#include "Cube.h"

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

class Joint {
    
  public:
    Joint ();
    ~Joint ();
    
    // Local matrix
    glm::mat4 L = glm::mat4 (1.0f);
    // World matrix
    glm::mat4 W = glm::mat4 (1.0f);
    Cube * cube;
    
    // Name of the joint
    std::string name;
    
    glm::vec3 Offset = glm::vec3(0, 0, 0);
    glm::vec3 boxMin = glm::vec3(-0.1, -0.1, -0.1);
    glm::vec3 boxMax = glm::vec3(0.1, 0.1, 0.1);
    
    std::vector <DOF *> List1;
    std::vector <Joint *> List2;
    
    void Load (Tokenizer &t);
    void Update (glm::mat4 parent);
    void AddChild (Joint * jnt);
    void Draw (const glm::mat4& viewProjMtx, GLuint shader, bool solid);
    void printOffset ();
    void Scale (double factor);
    
};

#endif /* Joint_hpp */
