//
//  joint.cpp
//  CSE 169
//
//  Created by 刘智超 on 2021/1/9.
//  Copyright © 2021 刘智超. All rights reserved.
//

#include "Joint.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>

Joint::Joint () {
    List1.push_back(new DOF());
    List1.push_back(new DOF());
    List1.push_back(new DOF());
}

void Joint::Load (Tokenizer &t) {
    
    char temp [256];
    
    t.GetToken(temp);
    this->name = temp;
    
    // Find {, then try to get all date in {}
    t.FindToken("{");
    
    while (1) {
        t.GetToken (temp);
        
        // If the prefix is offset, then get the data of Offset.
        if (strcmp(temp,"offset") == 0) {
            Offset.x = t.GetFloat();
            Offset.y = t.GetFloat();
            Offset.z = t.GetFloat();
            
        } else if (strcmp(temp, "boxmin") == 0) {
            boxMin.x = t.GetFloat();
            boxMin.y = t.GetFloat();
            boxMin.z = t.GetFloat();
        
        } else if (strcmp(temp, "boxmax") == 0) {
            boxMax.x = t.GetFloat();
            boxMax.y = t.GetFloat();
            boxMax.z = t.GetFloat();
            
        } else if (strcmp(temp, "rotxlimit") == 0) {
            List1[0]->setMinMax (t.GetFloat(), t.GetFloat());
            
        } else if (strcmp(temp, "rotylimit") == 0) {
            List1[1]->setMinMax (t.GetFloat(), t.GetFloat());
        
        } else if (strcmp(temp, "rotzlimit") == 0) {
            List1[2]->setMinMax (t.GetFloat(), t.GetFloat());
            
        } else if (strcmp(temp, "pose") == 0) {
            float x = t.GetFloat();
            float y = t.GetFloat();
            float z = t.GetFloat();
            
            List1[0]->setValue (x);
            List1[1]->setValue (y);
            List1[2]->setValue (z);
        
        // If the prefix is balljoint, create a new joint as its children.
        } else if (strcmp(temp, "balljoint") == 0) {
            Joint * jnt = new Joint ();
            jnt -> Load(t);
            jnt -> cube = new Cube (jnt->boxMin, jnt->boxMax);
            AddChild(jnt);
        
        // Reach the end of {}.
        } else if (strcmp(temp,"}") == 0) {
            return;
            
        } else {
            t.SkipLine();
        }
    }
    
    //cube = new Cube (boxMin, boxMax);
}

void Joint::Update (glm::mat4 parent) {
    
    L = glm::translate (glm::mat4(1.0), Offset) * glm::rotate(glm::radians(57.2958f * List1[2]->getValue()), glm::vec3(0.0f, 0.0f, 1.0f)) *
        glm::rotate(glm::radians(57.2958f * List1[1]->getValue()), glm::vec3(0.0f, 1.0f, 0.0f)) *
        glm::rotate(glm::radians(57.2958f * List1[0]->getValue()), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::mat4(1.0f);

    
    W = parent * L;
    cube -> update (W);
    
    for (int i = 0; i < List2.size(); i++) {
        List2[i]->Update(W);
    }
}

void Joint::Draw (const glm::mat4& viewProjMtx, GLuint shader, bool solid) {
    
    
    cube -> draw (viewProjMtx, shader, solid);
    
    for (int i = 0; i < List2.size(); i++) {
        List2[i]->Draw(viewProjMtx, shader, solid);
    }
}

void Joint::AddChild (Joint * jnt) {
    // Push the child joint into the list.
    this->List2.push_back (jnt);
}

void Joint::Scale (double factor) {
    L = glm::scale(glm::vec3(factor, factor, factor)) * L;
    for (int i = 0; i < List2.size(); i++) {
        List2[i]->Scale (factor);
    }
}

void Joint::printOffset () {
    std::cout << boxMin.x << " " << boxMin.y << " " << boxMin.z << endl;
}
