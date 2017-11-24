/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once

#include "Genotype.h"



/*
Used for clustering genotypes during epoch.
*/
class Species {
public:
    /*
    Constructor provided because compiler is a whining crybaby.
    */
    Species();

    /*
    Creates and copies the given genotype for compatibility
    measurement.
    */
    Species(
        const Genotype& first);

    /*
    Computes adjusted fitness for every member.
    */
    void computeAdjustedFitness();
    
    /*
    Queries members for improvements. If none available,
    increments the counter. Updates the champion. Returns if the
    species is stagnant for too long.
    */
    bool isStagnant();

    /*
    Fills parents with best performing members with respect to
    survival threshold and empties members.
    */
    void selectParents();

    std::vector<Genotype*> members;
    std::vector<Genotype> parents;
    Genotype champion;
    Genotype first;

    Float adjustedFitness = -1.0;
    Float best = -1.0;
    int stagnantFor = 0;
    int numOffspring = 0;
};
