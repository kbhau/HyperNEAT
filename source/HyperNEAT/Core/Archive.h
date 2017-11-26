/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once

#include "genes.h"

/*
Keeps the history of innovations.
*/
class Archive {
public:
    /*
    Removes all contents.
    */
    void clear();

    /*
    Looks up the innovation number for the pair or creates a new
    one and returns it.
    */
    int getLinkInnovation(
        const Connection& pair);
    
    /*
    Looks up the innovation number for the pair or creates a new
    one and returns it.
    */
    int getNeuronInnovation(
        const Connection& pair);

private:
    /*
    Generates innovation number, stores and returns it.
    */
    int createLinkInnovation(
        const Connection& pair);

    /*
    Generates innovation number, stores and returns it.
    */
    int createNeuronInnovation(
        const Connection& pair);
            
    std::map<Connection, int> linkInnovations;
    std::map<Connection, int> neuronInnovations;
    int linkInnovationNumber = -1;
    int neuronInnovationNumber = -1;
};
