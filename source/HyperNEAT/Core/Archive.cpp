/* Copyright (C) 2017 Wlodzimierz Samborski */

#include "../../pch.h"
#include "Archive.h"



void Archive::clear()
{
    linkInnovations.clear();
    neuronInnovations.clear();
    linkInnovationNumber = -1;
    neuronInnovationNumber = -1;
}

int Archive::createLinkInnovation(
        const LinkPair& pair)
{
    auto id = ++linkInnovationNumber;
    linkInnovations[pair] = id;
    return id;
}

int Archive::createNeuronInnovation(
        const LinkPair& pair)
{
    auto id = ++neuronInnovationNumber;
    neuronInnovations[pair] = id;
    return id;
}

int Archive::getLinkInnovation(
    const LinkPair& pair)
{
    auto it = linkInnovations.find(pair);

    //pair not found
    if (it == linkInnovations.end())
        return createLinkInnovation(pair);
    
    //return pair innovation number
    return it->second;
}

int Archive::getNeuronInnovation(
    const LinkPair& pair)
{
    auto it = neuronInnovations.find(pair);
    
    //pair not found
    if (it == neuronInnovations.end())
        return createNeuronInnovation(pair);
    
    //return pair innovation number
    return it->second;
}
