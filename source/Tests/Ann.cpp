/* Copyright (C) 2017 Wlodzimierz Samborski */

#include "pch.h"
#include "Ann.h"

#include "Solution.h"

using namespace std;
using namespace vt;



void Ann::create(
    const Solution& scheme,
    int numInputs_,
    int numOutputs_,
    int depth_)
{
    depth = depth_;
    numInputs = numInputs_;
    numOutputs = numOutputs_;
    numIONodes = numInputs + numOutputs;
    links.clear();
    nodes.clear();

    //create io nodes
    for (int i=0; i<numIONodes; ++i) {
        nodes[i] = {Neuron{}};
    }

    //add links and related nodes
    links.resize(scheme.links.size());
    for (int i=0; i<links.size(); ++i) {
        links[i].from = &(nodes[scheme.links[i].fromId]);
        links[i].to = &(nodes[scheme.links[i].toId]);
        links[i].weight = scheme.links[i].weight;
    }

    //add biases
    for (auto& bias : scheme.biases) {
        nodes[bias.id].bias = bias.value;
    }
}
