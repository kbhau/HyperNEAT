/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once

#include "pch.h"



/*
An output network scheme. Used to transport the solution out of
the CPPN.
*/
struct Solution {
private:
    /*
    Bias value of a node.        
    */
    struct SolutionBias {
        Float value;
        int id;
    };

    /*
    Connection between two nodes.
    */
    struct SolutionLink {
        Float weight;
        int fromId;
        int toId;
    };

public:
    std::vector<SolutionBias> biases;
    std::vector<SolutionLink> links;
};
