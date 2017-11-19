/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once

class ConnectionList;

struct Neuron {
    Float sum;
    Float out;
    Float bias = 0.0;
};

struct Link {
    Float weight;
    Neuron* from;
    Neuron* to;
};

/*
Phenotype - neural network. Assumes first output to indicate the
end of processing.
*/
class OutputNetwork {
public:
    /*
    Neuron activation function.
    */
    inline Float activationFunction(
        Float x);
    
    /*
    Clears the network and fills according to the scheme.
    */
    void create(
        const ConnectionList& scheme,
        int numInputs,
        int numOutputs,
        int depth);
    
    /*
    Processes the input and returns an output. Loops n times, or
    less when first output receives signal.
    */
    std::vector<Float> snapshot(
        Float* in,
        int rows);

private:
    std::vector<Link> links;
    std::map<int, Neuron> nodes;
    int numInputs;
    int numOutputs;
    int numIONodes;
    int depth;
};
