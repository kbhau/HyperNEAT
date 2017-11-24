/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once

#include "Archive.h"
#include "ConnectionList.h"
#include "Cppn.h"
#include "Globals.h"
#include "Population.h"
#include "Substrate.h"



/*
Data for modifying the fitness score.
*/
struct SolutionStats {
    int cppnLinks;
    int outputBiases;
    int outputLinks;
};

/*
API for the algorithm.
*/
class Neat {
public:
    /*
    Assigns fitness to solution at given id. Higher is better.
    */
    void evaluate(
        int id,
        Float fitness);
    
    /*
    Returns id of the last requested solution.
    */
    int getId();
    
    /*
    Returns next set of connections. When all are already taken,
    calls epoch.
    */
    ConnectionList getNext();

    /*
    Call after reading settings.
    */
    void initialise(
        const Substrate& substrate);

    /*
    Reads the algorithm settings from the file specified by path.
    */
    void readSettings(std::string path);

private:
    /*
    Helper function for cost evaluation.
    */
    Float getDiscountedFitness(
        Float fitness,
        int links,
        int thresholdMin,
        int thresholdMax,
        Float power);

    //components
    Archive archive;
    Cppn cppn;
    Population population;
    
    //Stats for cost computation.
    std::vector<SolutionStats> solutionStats;
    
    //Raw fitness multiplier.
    Float rawFitnessCoef;

    //Id of the next solution to return by getNext.
    int nextId;
};

#include "Neat.hpp"
