/* Copyright (C) 2017 Wlodzimierz Samborski */

#include "pch.h"
#include "OutputNetwork.h"

#include "ConnectionList.h"

using namespace std;



inline Float OutputNetwork::activationFunction(
    Float x)
{
    return //max(static_cast<Float>(0.0), x);
        x / (static_cast<Float>(0.25) + abs(x));
}

void OutputNetwork::create(
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

vector<Float> OutputNetwork::snapshot(
    Float* in,
    int rows)
{
    //stores outputs for all the data rows
    vector<Float> out(numOutputs * rows, 0.0);

    //process all rows
    for (int row=0; row<rows; ++row) {
        //set clean
        for (auto& kv : nodes) {
            if (kv.first < numInputs)
                kv.second.out = in[row * numInputs + kv.first];
            else
                kv.second.out = 0.0;
            kv.second.sum = 0.0;
        }
        
        //process
        for (int i=1; i<depth; ++i) {
            //propagate
            for (auto& l : links) {
                l.to->sum += l.from->out * l.weight;
            }

            //fire
            for (auto& kv : nodes) {
                if (kv.first < numInputs
                || kv.first >= numIONodes) {
                    kv.second.out = activationFunction(
                        kv.second.sum + kv.second.bias);
                    kv.second.sum = 0.0;
                }
            }
        }

        //save output
        for (int i=0; i<numOutputs; ++i) {
            out[row * numOutputs + i] = activationFunction(
                nodes[numInputs + i].sum 
                + nodes[numInputs + i].bias);
        }
    }

    return out;
}
