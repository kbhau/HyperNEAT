/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once

#include "CppnNeuron.h"

/*
Represents a synaptic connection between two neurons.
*/
class CppnSynapse {
public:
    /*
    Transfers the input to the output.
    */
    void impulse();



    Float weight;
    CppnNeuron* from;
    CppnNeuron* to;
};

inline
void CppnSynapse::impulse()
{
    to->sum += from->out * weight;
}
