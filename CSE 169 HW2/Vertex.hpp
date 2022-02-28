//
//  Vertex.hpp
//  CSE 169
//
//  Created by 刘智超 on 2021/1/22.
//  Copyright © 2021 刘智超. All rights reserved.
//

#ifndef Vertex_hpp
#define Vertex_hpp

#include <stdio.h>

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

class Vertex {

  public:
    Vertex ();
    ~Vertex ();
    
    int num = 0;
    
    glm::vec3 point;
    glm::vec3 normal;
    float Joint_Weight[INT_MAX];
    
};

#endif /* Vertex_hpp */
