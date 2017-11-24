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
        const LinkPair& pair);
    
    /*
    Looks up the innovation number for the pair or creates a new
    one and returns it.
    */
    int getNeuronInnovation(
        const LinkPair& pair);

private:
    /*
    Generates innovation number, stores and returns it.
    */
    int createLinkInnovation(
        const LinkPair& pair);

    /*
    Generates innovation number, stores and returns it.
    */
    int createNeuronInnovation(
        const LinkPair& pair);
            
    std::map<LinkPair, int> linkInnovations;
    std::map<LinkPair, int> neuronInnovations;
    int linkInnovationNumber = -1;
    int neuronInnovationNumber = -1;
};
