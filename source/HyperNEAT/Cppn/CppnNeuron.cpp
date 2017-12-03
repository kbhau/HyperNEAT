/* Copyright (C) 2017 Wlodzimierz Samborski */

#include "pch.h"
#include "CppnNeuron.h"

using namespace std;
using namespace vt;



CppnNeuron::CppnNeuron() :
y(static_cast<Float>(-1.0)),
functionNumber(-2)
{}

CppnNeuron::CppnNeuron(
    Float y_, 
    int functionNumber_) :
y(y_),
functionNumber(functionNumber_)
{}
