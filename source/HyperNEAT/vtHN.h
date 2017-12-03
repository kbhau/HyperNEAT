/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once

#include "Archive.h"
#include "Cppn.h"
#include "Globals.h"
#include "Population.h"
#include "Solution.h"
#include "Substrate.h"



namespace vt {

/*
API for the algorithm.
*/
class vtHN {
private:
    /*
    Data for computing the weight cost.
    */
    struct SolutionStats {
        int cppnLinks;
        int outputBiases;
        int outputLinks;
    };

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
    Solution getNext();

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

#include "vtHN.hpp"

}//namespace
