/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once

#include "Genotype.h"
#include "Species.h"



/*
Class responsible for management of all the genotypes.
*/
class Population {
public:
    /*
    Returns reference to the genotype at id.
    */
    const Genotype& getGenotype(
        int id);

    /*
    Creates a population of minimal genotypes and sets up
    variables.
    */
    void initialise();

    /*
    Applies fitness measure to the genotype number id.
    */
    void rateGenotype(
        Float fitness,
        int id);

    /*
    Selects best performers and uses them to produce next
    generation.
    Use after computing fitness for all organisms.
    */
    void stepGeneration();

private:
    //containers
    std::vector<Genotype> population;
    std::map<int, Species> species;

    //members
    Float best;
    Float similarityThreshold;
    int speciesId;
    int stagnantFor;
};
