/* Copyright (C) 2017 Wlodzimierz Samborski */

#include "pch.h"
#include "TestXor.h"

using namespace std;

void TestXor::draw()
{
    auto totalDots = 78;

    //top line - 2
    for (int i=0; i<80; ++i)
        cout << "=";
    cout << "\n\n";
    //2

    //run stats - 5
    cout << " ] RUN STATS\n";
    cout << " ] Eval. no.:\t" << bestEvaluation << "\n";
    cout << " ] Test score:\t" << bestScore
        << " / " << 4 << "\n";
    cout << " ] Fitness:\t" << bestFitness << "\n\n";
    //7

    //overall stats - 5
    cout << " ] BENCHMARK STATS:\n";
    cout << " ] Solved:\t" << solved << "\n";
    cout << " ] Unsolved:\t" << unsolved << "\n";
    cout << " ] Av. evals:\t"
        << static_cast<Float>(evaluationSum)
            / static_cast<Float>(solved + unsolved) << "\n\n";
    //12
    
    //newlines - 1
    cout << "\n";
    //13

    //progress bar - 3
    cout << " ] RUN PROGRESS:\t" << evaluation
        << " / " << evaluationMax << "\n\n";
    auto dots = evaluation / (evaluationMax / totalDots);
    cout << "[";
    for (int i=0; i<dots; ++i)
        cout << "X";
    for (int i=0; i<totalDots-dots; ++i)
        cout << "-";
    cout << "]\n";
    //16

    //newlines - 2
    cout << "\n\n";
    //18

    //progress bar - 3
    cout << " ] BENCHMARK PROGRESS:\t" << runNum
        << " / " << runs << "\n\n";
    dots = totalDots *
        (static_cast<Float>(runNum) / static_cast<Float>(runs));
    cout << "[";
    for (int i=0; i<dots; ++i)
        cout << "X";
    for (int i=0; i<totalDots-dots; ++i)
        cout << "-";
    cout << "]\n";
    //21
    
    //newlines - 1
    cout << "\n";
    //22

    //bottom line - 1
    for (int i=0; i<80; ++i)
        cout << "=";
    cout << "\n";
    //23
}

void TestXor::init()
{
    //set up the substrate
    s.numInputs = 2;
    s.numIONodes = 1 + s.numInputs;
    s.coords = {
        //input
        make_pair(-1.0, -1.0),
        make_pair(1.0, -1.0),

        //output
        make_pair(0.0, 1.0),

        //layer 1
        make_pair(-1.0, -0.5),
        make_pair(-0.5, -0.5),
        make_pair(0.0, -0.5),
        make_pair(0.5, -0.5),
        make_pair(1.0, -0.5),

        //layer 2
        make_pair(-1.0, 0.0),
        make_pair(-0.5, 0.0),
        make_pair(0.0, 0.0),
        make_pair(0.5, 0.0),
        make_pair(1.0, 0.0),

        //layer 3
        make_pair(-1.0, 0.5),
        make_pair(-0.5, 0.5),
        make_pair(0.0, 0.5),
        make_pair(0.5, 0.5),
        make_pair(1.0, 0.5)
    };

    //read parameters
    algo.readSettings("../settings/xor.txt");

    //magic number
    Globals::setSeed(42);

    //benchmark variables
    evaluationSum = 0;
    solved = 0;
    unsolved = 0;
}

void TestXor::perform(
    int numRuns)
{
    //initialise
    runs = numRuns;
    init();

    //run tests
    for (runNum=1; runNum<=numRuns; ++runNum) {
        run();
    }

    //print results - counters adjusted because of
    //stopping conditions
    runNum = numRuns;
    if (evaluation > evaluationMax)
        --evaluation;
    draw();
}

void TestXor::run()
{
    algo.initialise(s);
    bestFitness = 0.0;
    bestEvaluation = 0;
    bestScore = 0;

    //run test
    for (evaluation = 1;
        evaluation <= evaluationMax;
        ++evaluation)
    {
        //get next solution
        auto cl = move(algo.getNext());

        //create network
        net.create(
            cl,
            s.numInputs,
            s.numIONodes - s.numInputs,
            5);
        
        //evaluate
        auto error = static_cast<Float>(0.0);
        auto err2 = static_cast<Float>(0.0);
        auto score = 0;
        array<Float, 1> out;
        for (int row=0; row<testSet.size(); ++row) {
            net.snapshot<2, 1>(testSet[row].data, out);

            error =
                out[0] - testSet[row].label[0];
            err2 += error * error;
            
            if ((testSet[row].label[0] == 0.0 && out[0] < 0.5)
            || (testSet[row].label[0] == 1.0 && out[0] >= 0.5))
                ++score;
        }

        //map error to (0, 1], higher is better
        auto fitness = static_cast<Float>(1.0)
            + fabs(err2)
            / -(fabs(err2) + static_cast<Float>(1.0));

        //pass rating
        algo.evaluate(algo.getId(), fitness);

        //update stats
        if (score == 4) {
            ++solved;                
            evaluationSum += evaluation;
            bestEvaluation = evaluation;
            bestFitness = fitness;
            bestScore = score;
            draw();
            //saveOut();
            break;
        }
        
        if (fitness > bestFitness) {
            bestEvaluation = evaluation;
            bestFitness = fitness;
            bestScore = score;
            draw();
        }
        
        if (evaluation == evaluationMax) {
            ++unsolved;
            draw();
            break;
        } else if (evaluation % (evaluationMax / totalDots) == 0)
        {
            draw();
        }
    }//for evaluation
}

void TestXor::saveOut()
{
    //create file
    string filename = "out/xor";
    filename += to_string(runNum);
    filename += ".txt";
    ofstream f(filename, ofstream::trunc);

    //write contents
    f << "Everything < 0.5 is 0, everything else is 1.\n";
    array<Float, 1> out;
    for (int row=0; row<testSet.size(); ++row) {
        net.snapshot<2, 1>(testSet[row].data, out);
        f << "XOR( " << testSet[row].data[0] << ", "
            << testSet[row].data[1] << " ) = "
            << out[0] << "\n";
    }

    //close and check
    f.close();
    if (!f)
        throw runtime_error("Error writing XOR output file.");
}
