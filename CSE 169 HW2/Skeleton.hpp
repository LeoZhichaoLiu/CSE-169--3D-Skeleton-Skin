//
//  Skeleton.hpp
//  CSE 169
//
//  Created by 刘智超 on 2021/1/9.
//  Copyright © 2021 刘智超. All rights reserved.
//

#ifndef Skeleton_hpp
#define Skeleton_hpp

#include <stdio.h>
#include "Joint.hpp"
#include "Tokenizer.h"

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

class Skeleton {

  public:
    Skeleton (const char *file);
    ~Skeleton ();
    
    Joint * root;
    std::vector <Joint *> Joint_List;
    
    void Load (const char *file);
    void Update ();
    void Draw (const glm::mat4& viewProjMtx, GLuint shader, bool solid);
    void Scale (double factor);
    glm::mat4 GetWorldMatrix (int joint_index);
    void JointOrder (Joint * ref);
    
};

#endif /* Skeleton_hpp */
