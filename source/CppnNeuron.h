/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once

#include "Globals.h"

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



inline
void CppnNeuron::activate()
{
    switch (functionNumber) {
    case 0:
        out = std::sin(sum);
        break;
    case 1:
        out = std::cos(sum);
        break;
    case 2:
        out = Globals::clamp(
            std::tan(sum),
            static_cast<Float>(-1.0e5),
            static_cast<Float>(1.0e5));
        break;
    case 3:
        out = std::tanh(sum);
        break;
    case 4:
        out = (static_cast<Float>(1.0) - std::exp(-sum))
            / (static_cast<Float>(1.0) + std::exp(-sum));
        break;
    case 5:
        out = std::exp(static_cast<Float>(-1.0f) * (sum*sum));
        break;
    case 6:
        out = static_cast<Float>(1.0f)
            - static_cast<Float>(2.0f) * (sum - std::floor(sum));
        break;
    case 7:
        out = (static_cast<int>(std::floor(sum)) % 2 == 0) ?
            static_cast<Float>(1.0) :
            static_cast<Float>(-1.0);
        break;
    case 8:
        out = (static_cast<int>(std::floor(sum)) % 2 == 0) ?
            static_cast<Float>(1.0)
            - static_cast<Float>(2.0) * (sum - std::floor(sum)) :
            static_cast<Float>(1.0)
            + static_cast<Float>(2.0) * (sum - std::floor(sum));
        break;
    case 9:
        out = -sum;
        break;
    default:
        std::string str = 
            "Bad function number in CPPN Neuron activation: ";
        str += std::to_string(functionNumber);
        throw std::runtime_error(str);
    }
    sum = 0.0;
}
