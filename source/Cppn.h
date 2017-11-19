/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once

#include "ConnectionList.h"
#include "CppnNeuron.h"
#include "CppnSynapse.h"
#include "genes.h"
#include "Genotype.h"
#include "Substrate.h"

/*
CPPN will be used to generate weights for the output network.
For the 2D representation of the network there will be:
 - 5 inputs: bias, x1, y1, x2, y2,
 - 4 outputs: weightExpression, weight, biasExpression, bias
Regular weigths:
 - from: x1, y1
 - to: x2, y2
 - if weightExpression > 0 then create link with value weight
Biases:
 - position: x1, y1
 - x2 = y2 = 0
 - if biasExpression > 0 then create bias with value bias
*/
class Cppn {
public:
    /*
    Returns the amount of links. Useful for weight cost.
    */
    int getLinksAmount();

    /*
    Sets the substrate to be used for generating connections.
    */
    void setSubstrate(
        const Substrate& substrate);

    /*
    Queries the CPPN for connection weights.
    */
    const ConnectionList& query(
        const Genotype& genes);


        
private:
    /*
    Creates nodes and links from the given genotype.
    */
    void createFromGenotype(
        const Genotype& genes);

    /*
    Helper function for adding neurons from genotype.
    */
    void createNeuronIfAbsent(
        int neuronId,
        const Genotype& genes);
    
    /*
    Produces a collection of synapses for the given substrate.
    */
    void generateConnections();

    /*
    Resets neurons and fires neurons *depth* times.
    */
    void process(Float* inputs);



    //IO
    ConnectionList output;
    Substrate substrate;

    //Neurons of the CPPN. Format: innovation number, neuron data.
    std::map<int, CppnNeuron> nodes;
    std::vector<CppnSynapse> links;

    //Fire depth-1 times.
    int depth;

    //inputs & outputs
    int numInputs = 5;
    int numOutputs = 4;
    int numIONodes = 9;
};



inline
void Cppn::createNeuronIfAbsent(
    int neuronId,
    const Genotype& genes)
{
    if (nodes[neuronId].y == -1.0) {
        auto& node = genes.getNeuron(neuronId);
        nodes[neuronId] = std::move(CppnNeuron(
            node.y,
            node.functionNumber));
    }
}

inline
int Cppn::getLinksAmount()
{
    return links.size();
}
