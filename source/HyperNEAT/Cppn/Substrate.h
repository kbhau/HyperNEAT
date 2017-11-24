/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once

class Substrate {
public:
    //Holds possible neuron coordinates for the CPPN to work on.
    std::vector<std::pair<Float, Float>> coords;
    
    //Assuming order Inputs, Outputs, Hiddens
    int numInputs;
    int numIONodes;
};
