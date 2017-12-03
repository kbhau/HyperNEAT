/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once

#include "CppnNeuron.h"



namespace vt {

/*
Represents a synaptic connection between two neurons.
*/
class CppnSynapse {
public:
    /*
    Transfers the input to the output.
    */
    void impulse();

    //members
    Float weight;
    CppnNeuron* from;
    CppnNeuron* to;
};

#include "CppnSynapse.hpp"

}//namespace
