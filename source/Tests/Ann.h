/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once

namespace vt {
    class Solution;
}

/*
Phenotype - neural network. Assumes first output to indicate the
end of processing.
*/
class Ann {
private:
    /*
    Nodes of the network.
    */
    struct Neuron {
        Float sum;
        Float out;
        Float bias = 0.0;
    };

    /*
    Connections of the network.
    */
    struct Link {
        Float weight;
        Neuron* from;
        Neuron* to;
    };

public:
    /*
    Neuron activation function.
    */
    Float activationFunction(
        Float x);
    
    /*
    Clears the network and fills according to the scheme.
    */
    void create(
        const vt::Solution& scheme,
        int numInputs,
        int numOutputs,
        int depth);
    
    /*
    Processes the input and returns an output. Loops n times, or
    less when first output receives signal.
    */
    template<
        int tNumInputs,
        int tNumOutputs>
    void snapshot(
        std::array<Float, tNumInputs>& in,
        std::array<Float, tNumOutputs>& out);

private:
    std::vector<Link> links;
    std::map<int, Neuron> nodes;
    int numInputs;
    int numOutputs;
    int numIONodes;
    int depth;
};

#include "Ann.hpp"
