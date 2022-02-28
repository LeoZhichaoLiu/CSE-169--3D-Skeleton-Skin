//
//  DOF.hpp
//  CSE 169
//
//  Created by 刘智超 on 2021/1/9.
//  Copyright © 2021 刘智超. All rights reserved.
//

#ifndef DOF_hpp
#define DOF_hpp

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

class DOF {
    
  public:
    DOF ();
    ~DOF ();
    
    float Value = 0;
    float min = -10000;
    float max = 10000;
    
    void setValue (float value);
    float getValue ();
    void setMinMax (float min, float max);
    

};

#endif /* DOF_hpp */
