/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once

#include "Ann.h"
#include "LabeledRow.h"
#include "vtHN.h"



/*
Performs benchmark on Iris dataset classification task.
*/
class TestIris {
public:
    /*
    Runs the benchmark for numEvaluations.
    */
    void perform(
        int numEvaluations);

private:
    /*
    Prints the current state of the run into the terminal.
    */
    void draw();

    /*
    Prepares the benchmark.
    */
    void init();

    /*
    Imports test data from file.
    */
    void importDataset();

    /*
    Performs the core computation.
    */
    void run();



    //core data
    Ann net;
    vtHN algo;
    Substrate s;
    std::vector<LabeledRow<4, 3>> trainingSet;
    std::vector<LabeledRow<4, 3>> testSet;
    int numOutputs;
    int depth;

    //stats
    Float fFitness;
    Float sFitness;
    Float vFitness;
    int fTestScore;
    int fValScore;
    int fEval;
    int sTestScore;
    int sValScore;
    int sEval;
    int vTestScore;
    int vValScore;
    int vEval;
    int maxTestScore;
    int maxValScore;
    int maxEval;
    int eval;
    int totalDots = 78;
};
