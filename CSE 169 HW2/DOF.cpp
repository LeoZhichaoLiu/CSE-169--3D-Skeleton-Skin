//
//  DOF.cpp
//  CSE 169
//
//  Created by 刘智超 on 2021/1/9.
//  Copyright © 2021 刘智超. All rights reserved.
//

#include "DOF.hpp"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>

DOF::DOF () {
    
}

void DOF::setValue (float value) {
   
    if (value < this->min) {
        this->Value = this->min;
        
    } else if (value > this->max) {
        this->Value = this->max;
        
    } else {
       this->Value = value;
    }
}

float DOF::getValue () {
    return this->Value;
}

void DOF::setMinMax (float min, float max) {
    this->min = min;
    this->max = max;
}

