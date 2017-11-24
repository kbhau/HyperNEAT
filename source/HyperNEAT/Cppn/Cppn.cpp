/* Copyright (C) 2017 Wlodzimierz Samborski */

#include "pch.h"
#include "Cppn.h"

#include "Globals.h"

using namespace std;

void Cppn::createFromGenotype(
    const Genotype& genes)
{
    //clear old data
    links.clear();
    nodes.clear();

    //create io neurons
    for (int i=0; i<numIONodes; ++i) {
        createNeuronIfAbsent(i, genes);
    }

    //create structure
    for (auto& link : genes.linkGenes) {
        if (!link.enabled)
            continue;
        
        createNeuronIfAbsent(link.link.from, genes);
        createNeuronIfAbsent(link.link.to, genes);
        links.push_back(CppnSynapse{
            link.weight,
            &nodes[link.link.from],
            &nodes[link.link.to]
        });
    }

    //find depth
    vector<Float> yValues;
    yValues.reserve(10);
    for (auto& node : nodes) {
        bool found = false;
        for (auto y : yValues)
            if (y == node.second.y) {
                found = true;
                break;
            }
        if (!found)
            yValues.push_back(node.second.y);
    }
    depth = yValues.size();
}
    
void Cppn::generateConnections()
{
    //reset old connections
    output.biases.clear();
    output.links.clear();

    //regular connections
    for (int i=0; i<substrate.coords.size(); ++i) {
        for (int j=0; j<substrate.coords.size(); ++j) {
            Float cppnInput[5] = {
                1.0,
                substrate.coords[i].first,
                substrate.coords[i].second,
                substrate.coords[j].first,
                substrate.coords[j].second
            };
            process(cppnInput);
            if (nodes[5].out > 0.0) {
                output.links.push_back(
                    make_tuple(i, j,
                        Globals::clamp(
                            nodes[6].out,
                            static_cast<Float>(-3.0),
                            static_cast<Float>(3.0))));
            }
        }
    }

    //biases
    for (int i=0; i<substrate.coords.size(); ++i) {
        Float cppnInput[5] = {
            1.0,
            substrate.coords[i].first,
            substrate.coords[i].second,
            0.0,
            0.0
        };
        process(cppnInput);
        if (nodes[7].out > 0.0) {
            output.biases.push_back(
                make_pair(i, nodes[8].out));
        }
    }
}

void Cppn::process(Float* inputs)
{
    //set initial states
    for (auto& kv : nodes) {
        if (kv.first < numInputs) {
            kv.second.out = inputs[kv.first];
            kv.second.sum = 0.0;
        } else {
            kv.second.out = 0.0;
            kv.second.sum = 0.0;
        }
    }
    
    //step depth-1 times
    for (int i=1; i<depth; ++i) {
        for (auto& link : links) {
            link.impulse();
        }
        for (auto& kv : nodes) {
            if (kv.first > 0
                && (kv.first < numInputs
                    || kv.first >= numIONodes))
            {
                if (kv.second.y != static_cast<Float>(-1.0))
                    kv.second.activate();
            }
        }
    }

    //activate outputs
    for (int i=5; i<9; ++i) {
        nodes[i].activate();
    }
}

const ConnectionList& Cppn::query(
    const Genotype& genes)
{
    createFromGenotype(genes);
    generateConnections();
    return output;
}

void Cppn::setSubstrate(
    const Substrate& substrate_)
{
    substrate = substrate_;
}
