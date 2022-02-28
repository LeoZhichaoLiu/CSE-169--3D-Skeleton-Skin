//
//  Skin.cpp
//  CSE 169
//
//  Created by 刘智超 on 2021/1/22.
//  Copyright © 2021 刘智超. All rights reserved.
//

#include "Skin.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>


Skin::Skin (const char *file, Skeleton * skeleton2) {
    this -> skeleton = skeleton2;
    Load (file);
    
    material = new Material ();
    
    material -> ambient = glm::vec3 (0.215, 0.4745, 0.215);
    material -> diffuse = glm::vec3 (0.07568, 0.71424, 0.07568);
    material -> specular = glm::vec3 (0.533, 0.727811, 0.633);
    material -> shininess = 0.6;
}

Skin::~Skin () {}

void Skin::Load (const char *file) {
    Tokenizer token;
    
    token.Open (file);
    
    // First record the position values in class Vertex
    token.FindToken ("positions");
    numVert = token.GetInt ();
    token.FindToken ("{");
    for (int i = 0; i< numVert; i++) {
        Vertex * v = new Vertex();
        points.push_back (v);
        v->point.x = token.GetFloat();
        v->point.y = token.GetFloat();
        v->point.z = token.GetFloat();
    }
    token.FindToken ("}");
    
    // Second record the normal values in class Vertex
    token.FindToken ("normals");
    numVert = token.GetInt();
    token.FindToken ("{");
    for (int i = 0; i< numVert; i++) {
        points[i]->normal.x = token.GetFloat();
        points[i]->normal.y = token.GetFloat();
        points[i]->normal.z = token.GetFloat();
    }
    token.FindToken ("}");
    
    // Then store the weight for each joint in class Vertex
    token.FindToken ("skinweights");
    numVert = token.GetInt();
    token.FindToken ("{");
    for (int i = 0; i< numVert; i++) {
        int numAttach = token.GetInt();
        for (int j = 0; j < numAttach; j++) {
            int index = token.GetInt();
            points[i]->Joint_Weight[index] = token.GetFloat();
        }
    }
    token.FindToken ("}");
    
    // Store the triangles (face) in class Triangle
    token.FindToken ("triangles");
    int numTri = token.GetInt();
    token.FindToken ("{");
    for (int i = 0; i< numTri; i++) {
        glm::ivec3 face;
        face.x = token.GetInt();
        face.y = token.GetInt();
        face.z = token.GetInt();
        indices.push_back (face);
    }
    token.FindToken ("}");
    
    // Store binding matrix in vector binding, for each joint;
    token.FindToken ("bindings");
    numBind = token.GetInt();
    
    //cout << numBind << endl;
    
    token.FindToken ("{");
    for (int i = 0; i< numBind; i++) {
        token.FindToken ("matrix");
        token.FindToken ("{");
        glm::mat4 b;
        b[0][0] = token.GetFloat();
        b[0][1] = token.GetFloat();
        b[0][2] = token.GetFloat();
        b[0][3] = 0.0;
        b[1][0] = token.GetFloat();
        b[1][1] = token.GetFloat();
        b[1][2] = token.GetFloat();
        b[1][3] = 0.0;
        b[2][0] = token.GetFloat();
        b[2][1] = token.GetFloat();
        b[2][2] = token.GetFloat();
        b[2][3] = 0.0;
        b[3][0] = token.GetFloat();
        b[3][1] = token.GetFloat();
        b[3][2] = token.GetFloat();
        b[3][3] = 1.0;
        binding.push_back (b);
        token.FindToken ("}");
    }
    token.FindToken ("}");
    token.Close ();
}

void Skin::Update () {

        
    for (int i = 0; i < binding.size(); i++) {
        if (skeleton != NULL) {
            if (Skin_Matrix.size() == binding.size()) {
                Skin_Matrix[i] = ( skeleton->GetWorldMatrix(i) * glm::inverse(binding[i]) );
           } else {
                Skin_Matrix.push_back (skeleton->GetWorldMatrix(i) * glm::inverse(binding[i]));
           }
        } else {
             if (Skin_Matrix.size() == binding.size()) {
                 Skin_Matrix[i] = ( glm::mat4(1) * glm::inverse(binding[i]) );
             } else {
                 Skin_Matrix.push_back ( glm::mat4(1) * glm::inverse(binding[i]));
             }
        }
    }
    
    for (int i = 0; i < points.size(); i++) {
        glm::vec4 temp = glm::vec4(points[i]->point.x, points[i]->point.y, points[i]->point.z, 1.0);
        glm::vec4 res = glm::vec4(0.0, 0.0, 0.0, 0.0);
        for (int j = 0; j < binding.size(); j++) {
            float factor = points[i]->Joint_Weight[j];
            res = res + factor * (Skin_Matrix[j] * temp);
        }
    
        if (positions.size() == points.size()) {
            positions[i] = (glm::vec3(res.x, res.y, res.z));
        } else {
            positions.push_back (glm::vec3(res.x, res.y, res.z));
        }
    }
    
    
    for (int i = 0; i < points.size(); i++) {
        glm::vec4 temp = glm::vec4(points[i]->normal.x, points[i]->normal.y, points[i]->normal.z, 0.0);
        glm::vec4 res = glm::vec4(0.0, 0.0, 0.0, 0.0);
        for (int j = 0; j < binding.size(); j++) {
           float factor = (points[i]->Joint_Weight[j]);
           res += factor  * glm::transpose(glm::inverse (Skin_Matrix[j])) * temp;
        }
        
        glm::vec3 final_normal = glm::vec3(res.x, res.y, res.z);
        final_normal = final_normal / glm::length (final_normal);
        
        if (normals.size() == points.size()) {
            normals[i] = (final_normal);
        } else {
            normals.push_back (final_normal);
        }
    }
    
    
    // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_positions);
    glGenBuffers(1, &VBO_normals);

    // Bind to the VAO.
    glBindVertexArray(VAO);

    // Bind to the first VBO - We will use it to store the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Bind to the second VBO - We will use it to store the normals
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Generate EBO, bind the EBO to the bound VAO and send the data
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Unbind the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
}

void Skin::Draw (const glm::mat4& viewProjMtx, GLuint shader) {
    
    // actiavte the shader program
    glUseProgram(shader);

    // get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);
    glUniform3fv(glGetUniformLocation(shader, "ambient"), 1, glm::value_ptr(material->ambient));
    glUniform3fv(glGetUniformLocation(shader, "diffuse"), 1, glm::value_ptr(material->diffuse));
    glUniform3fv(glGetUniformLocation(shader, "specular"), 1, glm::value_ptr(material->specular));
    glUniform1f(glGetUniformLocation(shader, "shininess"), material->shininess);

    // Bind the VAO
    glBindVertexArray(VAO);
    
    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, sizeof(glm::ivec3) * indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
    
}
