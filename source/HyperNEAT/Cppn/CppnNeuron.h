/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once

#include "Globals.h"



namespace vt {

/*
Represents a single node of CPPN.
*/
class CppnNeuron {
public:
    /*
    Default constructor.
    */
    CppnNeuron();

    /*
    Constructor.
    */
    CppnNeuron(
        Float y, 
        int functionNumber);

    /*
    Sets out to the activation function of sum.
    */
    void activate();

    Float sum;
    Float out;
    Float y;

private:
    int functionNumber;
};

#include "CppnNeuron.hpp"

}//namespace
