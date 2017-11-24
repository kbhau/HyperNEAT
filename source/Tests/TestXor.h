/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once

#include "Ann.h"
#include "LabeledRow.h"
#include "Neat.h"



/*
Performs benchmark on XOR logical function.
*/
class TestXor {
public:
    /*
    Attempts to solve XOR problem numRuns times and computes
    average evaluations.
    */
    void perform(
        int numRuns);

private:
    /*
    Outputs a 80x24 informational screen to the console.
    */
    void draw();

    /*
    Prepares test data.
    */
    void init();

    /*
    Runs a single test.
    */
    void run();

    /*
    Saves the output of the current network to a file. Used to
    manually check the passing solutions.
    */
    void saveOut();

    //objects
    Ann net;
    Neat algo;
    Substrate s;
    std::vector<LabeledRow<2, 1>> testSet = {
        {{0.0, 0.0}, {0.0}},
        {{0.0, 1.0}, {1.0}},
        {{1.0, 0.0}, {1.0}},
        {{1.0, 1.0}, {0.0}}
    };

    //benchmark vars
    int evaluationSum = 0;
    int runNum;
    int runs;
    int solved = 0;
    int unsolved = 0;

    //run vars
    Float bestFitness;
    int bestEvaluation;
    int bestScore;
    int evaluation;
    int evaluationMax = 20000;
    int totalDots = 78;
};
