/* Copyright (C) 2017 Wlodzimierz Samborski */

#include "pch.h"
#include "Ann.h"

#include "ConnectionList.h"

using namespace std;



void Ann::create(
    const ConnectionList& scheme,
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
        links[i].from = &(nodes[get<0>(scheme.links[i])]);
        links[i].to = &(nodes[get<1>(scheme.links[i])]);
        links[i].weight = get<2>(scheme.links[i]);
    }

    //add biases
    for (auto& kv : scheme.biases) {
        nodes[kv.first].bias = kv.second;
    }
}
