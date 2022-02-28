//
//  sketeton.cpp
//  CSE 169
//
//  Created by 刘智超 on 2021/1/9.
//  Copyright © 2021 刘智超. All rights reserved.
//

#include "Skeleton.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>

Skeleton::Skeleton (const char *file) {
    Load (file);
}

Skeleton::~Skeleton () {}

void Skeleton::Load (const char *file) {
    Tokenizer token;
    
    // Open the file, and find the predix balljoint to get the root's data.
    token.Open (file);
    token.FindToken ("balljoint");
    
    // Create a root joint with the rest of data.
    root = new Joint ();
    root->Load (token);
    root -> cube = new Cube (root->boxMin, root->boxMax);
    
    token.Close ();
    
    //cout << root->List2.size() << endl;
    JointOrder (root);
    
    /*
    for (int i = 0; i< Joint_List.size(); i++) {
        cout << Joint_List[i]->Offset.x << " " << Joint_List[i]->Offset.y << " " << Joint_List[i]->Offset.z << endl;
    }
    */
}

void Skeleton::JointOrder (Joint * ref) {
    
    Joint_List.push_back (ref);
    //cout << ref->List2.size() << endl;
    for (int i = 0; i < ref->List2.size(); i++) {
        JointOrder (ref->List2[i]);
    }
}

void Skeleton::Update () {
    root -> Update (glm::mat4(1));
}

void Skeleton::Draw (const glm::mat4& viewProjMtx, GLuint shader, bool solid) {
    root -> Draw (viewProjMtx, shader, solid);
}

glm::mat4 Skeleton::GetWorldMatrix (int joint_index) {
    return Joint_List[joint_index]->W;
}

void Skeleton::Scale (double factor) {
    root -> Scale (factor);
}

