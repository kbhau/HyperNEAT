/* Copyright (C) 2017 Wlodzimierz Samborski */

/*
TODO ASAP:
 - Done.
*/

#include "pch.h"
#include "Neat.h"
#include "OutputNetwork.h"

using namespace std;

/*
Grows a network that can solve XOR logical function. Runs 10
tests.
*/
void testXor()
{
    cout << "========= XOR TEST =========" << endl << endl;

    Substrate s;
    s.numInputs = 2;
    s.numIONodes = 1 + s.numInputs;
    s.coords = {
        make_pair(-1.0, -1.0),
        make_pair(1.0, -1.0),

        make_pair(0.0, 1.0),

        make_pair(-0.333, -0.333),
        make_pair(0.333, -0.333),
        make_pair(-0.333, 0.333),
        make_pair(0.333, 0.333)
    };

    Neat algo;
    algo.readSettings("../settings/xor.txt");
    OutputNetwork net;
    Float in[] = {
        0.0, 0.0,
        0.0, 1.0,
        1.0, 0.0,
        1.0, 1.0
    };
    Float labels[] = {
        0.0,
        1.0,
        1.0,
        0.0
    };
    auto sumEvals = 0;
    auto solved = 0;
    auto unsolved = 0;
    for (int i=0; i<10; ++i) {
        //initialisations
        cout << "\n\n\n=== TEST " << i+1 << " ===\n\n";
        algo.initialise(s);
        auto pass = false;
        auto best = static_cast<Float>(0.0);
        auto evaluation = 0;

        //run test
        while (!pass) {
            ++evaluation;

            //get next net scheme
            auto cl = move(algo.getNext());

            //create network
            net.create(
                cl,
                s.numInputs,
                s.numIONodes - s.numInputs,
                4);
            auto good = 0;
                
            //evaluate
            vector<Float> netOut = move(net.snapshot(in, 4));
            auto error = static_cast<Float>(0.0);
            auto err2 = static_cast<Float>(0.0);
            for (int j=0; j<4; ++j) {
                if ((labels[j] == 0.0 && netOut[j] < 0.5)
                || (labels[j] == 1.0 && netOut[j] >= 0.5))
                    ++good;
                error = labels[j] - netOut[j];
                err2 += error * error;
            }

            //map error to (0, 1], higher is better
            auto fitness = static_cast<Float>(1.0)
                + abs(err2)
                / -(abs(err2) + static_cast<Float>(1.0));

            //pass rating
            algo.evaluate(algo.getId(), fitness);

            //print findings
            if (good == 4) {
                ++solved;                
                cout << "\nSolution found.\n" << endl;
                cout << "Evals:\t\t" << evaluation << endl;
                cout << "Fitness:\t" << fitness << endl;
                sumEvals += evaluation;
                pass = true;
            } else if (fitness > best) {
                best = fitness;
                cout << evaluation << ":\t" << best << endl;
            } else if (evaluation >= 19999) {
                cout << "\nNo solution" << endl;
                ++unsolved;
                break;
            }
        }//while !pass
    }//for i<100

    //print results
    cout << "\n\n===== RESULTS =====\n\n";
    cout << "Successful:\t" << solved << endl;
    cout << "Unsuccessful:\t" << unsolved << endl;
    cout << "Av. evals:\t" <<
        static_cast<Float>(sumEvals) / static_cast<Float>(solved)
        << endl << endl;
    cout << "===================\n\n";
}//testXor

/*
Runs test on Iris dataset.
*/
void testIris()
{
    /*
    Holds test statistics for the Iris dataset test. Passed to a
    display function.
    */
    struct IrisStats {
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
    };

    /*
    Reads Iris dataset into passed vectors.
    */
    auto readIris = [](
        vector<Float>& data,
        vector<Float>& labels)
    {
        string line;

        //open file
        ifstream iris("../testData/iris.txt");
        if (!iris.is_open())
            throw runtime_error("Error while opening Iris data.");

        //process contents
        auto pos = 0;
        while (getline(iris, line)) {
            for (int i=0; i<4; ++i) {
                pos = line.find(',');
                data.push_back(stod(line.substr(0, pos)));
                line = line.substr(pos+1);
            }
            if (line == "Iris-setosa") {
                labels.push_back(1.0);
                labels.push_back(0.0);
                labels.push_back(0.0);
            } else if (line == "Iris-versicolor") {
                labels.push_back(0.0);
                labels.push_back(1.0);
                labels.push_back(0.0);
            } else if (line == "Iris-virginica") {
                labels.push_back(0.0);
                labels.push_back(0.0);
                labels.push_back(1.0);
            } else {
                throw logic_error("Error reading Iris data.");
            }
        }

        if (iris.bad())
            throw runtime_error("Error while reading Iris dataset.");

        return labels.size() / 3;
    };//readIris

    cout << "\n\n========= IRIS TEST =========" << endl << endl;

    Substrate s;
    s.numInputs = 4;
    s.numIONodes = 3 + s.numInputs;
    auto numOutputs = s.numIONodes - s.numInputs;
    s.coords = {
        //in
        make_pair(-1.0, -1.0),
        make_pair(-0.333, -1.0),
        make_pair(0.333, -1.0),
        make_pair(1.0, -1.0),

        //out
        make_pair(-1.0, 1.0),
        make_pair(0.0, 1.0),
        make_pair(1.0, 1.0),

        //layer1
        make_pair(-1.0, -0.5),
        make_pair(-0.75, -0.5),
        make_pair(-0.5, -0.5),
        make_pair(-0.25, -0.5),
        make_pair(0.0, -0.5),
        make_pair(0.25, -0.5),
        make_pair(0.5, -0.5),
        make_pair(0.75, -0.5),
        make_pair(1.0, -0.5),

        //layer2
        make_pair(-1.0, 0.0),
        make_pair(-0.75, 0.0),
        make_pair(-0.5, 0.0),
        make_pair(-0.25, 0.0),
        make_pair(0.0, 0.0),
        make_pair(0.25, 0.0),
        make_pair(0.5, 0.0),
        make_pair(0.75, 0.0),
        make_pair(1.0, 0.0),
        
        //layer3
        make_pair(-1.0, 0.5),
        make_pair(-0.75, 0.5),
        make_pair(-0.5, 0.5),
        make_pair(-0.25, 0.5),
        make_pair(0.0, 0.5),
        make_pair(0.25, 0.5),
        make_pair(0.5, 0.5),
        make_pair(0.75, 0.5),
        make_pair(1.0, 0.5),
    };
    auto depth = 3;
    
    //prepare test data
    vector<Float> in;
    vector<Float> labels;
    auto numRows = readIris(in, labels);    //will be 150

    //get validation set
    vector<Float> inv;
    vector<Float> labelsv;
    for (int i=0; i<150; ++i) {
        if ((i < 5)
        || (i >= 50 && i < 55)
        || (i >= 100 && i < 105))
        {
            for (int j=0; j<s.numInputs; ++j) {
                inv.push_back(in[i*s.numInputs + j]);
            }
            for (int j=0; j<numOutputs; ++j) {
                labelsv.push_back(labels[i*numOutputs + j]);
            }
        }
    }

    //erase validation data
    for (int i=149; i>=0; --i) {
        if ((i < 5)
        || (i >= 50 && i < 55)
        || (i >= 100 && i < 105))
        {
            //back to front to avoid working on data that is
            //already moved
            for (int j=s.numInputs-1; j>=0; --j) {
                in.erase(in.begin() + (i*s.numInputs + j));
            }
            for (int j=numOutputs-1; j>=0; --j) {
                labels.erase(
                    labels.begin() + (i*s.numInputs + j));
            }
        }
    }
    auto numRowsv = 15;
    numRows -= numRowsv;
    
    //initialisations
    OutputNetwork net;
    Neat algo;
    algo.readSettings("../settings/iris.txt");
    algo.initialise(s);

    /*
    Changed the seed when tackling a bug in this test. It was
    producing 100% scores after 1-12 evaluations. Bug was picking
    option 1 by default unless the net was sure about other
    options. Rest of code didn't change so apparently it works...
    */
    Globals::setSeed(42);   //default produces 100% solution
                            //immediately O.o - when defaulting
    IrisStats stats;
    stats.maxEval = 50000;
    stats.maxTestScore = numRows;
    stats.maxValScore = numRowsv;
    stats.fFitness = static_cast<Float>(0.0);
    stats.fTestScore = 0;
    stats.fValScore = 0;
    stats.sFitness = static_cast<Float>(0.0);
    stats.sTestScore = 0;
    stats.sValScore = 0;
    stats.vFitness = static_cast<Float>(0.0);
    stats.vTestScore = 0;
    stats.vValScore = 0;
    auto totalDots = 78;
    
    /*
    Function prints the current state of the run into the
    terminal.
    */
    auto draw = [&stats, &totalDots]()
    {
        //top line - 2
        for (int i=0; i<80; ++i)
            cout << "=";
        cout << endl << endl;
        //2

        //stats a - 6
        cout << " ] BEST BY FITNESS:" << endl;
        cout << " ] Eval. no.:\t" << stats.fEval << endl;
        cout << " ] Test score:\t" << stats.fTestScore << " / "
            << stats.maxTestScore << "\t"
            << static_cast<Float>(stats.fTestScore) * 100.0
            / static_cast<Float>(stats.maxTestScore) << "%\n";
        cout << " ] Val. score:\t" << stats.fValScore << " / "
            << stats.maxValScore << "\t\t"
            << static_cast<Float>(stats.fValScore) * 100.0
            / static_cast<Float>(stats.maxValScore) << "%\n";
        cout << " ] Fitness:\t" << stats.fFitness << "\n\n";
        //8

        //stats b - 6
        cout << " ] BEST BY TEST SCORE:" << endl;
        cout << " ] Eval. no.:\t" << stats.sEval << endl;
        cout << " ] Test score:\t" << stats.sTestScore << " / "
            << stats.maxTestScore << "\t"
            << static_cast<Float>(stats.sTestScore) * 100.0
            / static_cast<Float>(stats.maxTestScore) << "%\n";
        cout << " ] Val. score:\t" << stats.sValScore << " / "
            << stats.maxValScore << "\t\t"
            << static_cast<Float>(stats.sValScore) * 100.0
            / static_cast<Float>(stats.maxValScore) << "%\n";
        cout << " ] Fitness:\t" << stats.sFitness << "\n\n";
        //14

        //stats c - 6
        cout << " ] BEST BY VALIDATION:" << endl;
        cout << " ] Eval. no.:\t" << stats.vEval << endl;
        cout << " ] Test score:\t" << stats.vTestScore << " / "
            << stats.maxTestScore << "\t"
            << static_cast<Float>(stats.vTestScore) * 100.0
            / static_cast<Float>(stats.maxTestScore) << "%\n";
        cout << " ] Val. score:\t" << stats.vValScore << " / "
            << stats.maxValScore << "\t\t"
            << static_cast<Float>(stats.vValScore) * 100.0
            / static_cast<Float>(stats.maxValScore) << "%\n";
        cout << " ] Fitness:\t" << stats.vFitness << "\n\n";
        //20

        //progress bar - 3
        cout << " ] PROGRESS:\t" << stats.eval << " / "
            << stats.maxEval << endl << endl;
        auto dots = stats.eval / (stats.maxEval / totalDots);
        cout << "[";
        for (int i=0; i<dots; ++i)
            cout << "X";
        for (int i=0; i<totalDots-dots; ++i)
            cout << "-";
        cout << "]\n";
        //23

        //bottom line - 1
        for (int i=0; i<80; ++i)
            cout << "=";
        //24
    };

    //run test
    for (stats.eval = 1;
        stats.eval <= stats.maxEval;
        ++stats.eval)
    {
        //create network
        net.create(
            move(algo.getNext()),
            s.numInputs,
            s.numIONodes - s.numInputs,
            depth);
            
        //evaluate
        vector<Float> netOut = move(
            net.snapshot(in.data(), numRows));
        auto error = static_cast<Float>(0.0);
        auto err2 = static_cast<Float>(0.0);
        auto good = 0;
        for (int j=0; j<numRows; ++j) {
            //pick an answer
            auto picked = -1;
            auto pickedValue = static_cast<Float>(0.0);
            for (int k=0; k<numOutputs; ++k) {
                if (netOut[j*numOutputs + k] > pickedValue) {
                    pickedValue = netOut[j*numOutputs + k];
                    picked = j*numOutputs + k;
                }
                
                error = labels[j*numOutputs + k]
                    - netOut[j*numOutputs + k];
                err2 += error * error;
            }

            //verify
            if (picked != -1)
                if (labels[picked] == 1.0)
                    ++good;
        }

        //map error to (0, 1], higher is better
        auto fitness = static_cast<Float>(1.0)
            + abs(err2)
            / -(abs(err2) + static_cast<Float>(1.0));

        //include accuracy measure - might help
        fitness = fitness * static_cast<Float>(0.25)
            + static_cast<Float>(0.75)
            * (static_cast<Float>(good)
            / static_cast<Float>(numRows));

        //pass rating
        algo.evaluate(algo.getId(), fitness);

        //print findings
        if (good > stats.sTestScore
        || fitness > stats.fFitness)
        {
            //pick set to update
            

            //compute score on validation set and print
            vector<Float> netOut = move(
                net.snapshot(inv.data(), numRowsv));
            auto goodv = 0;
            for (int j=0; j<numRowsv; ++j) {
                //pick an answer
                auto picked = -1;
                auto pickedValue = static_cast<Float>(0.0);
                for (int k=0; k<numOutputs; ++k) {
                    if (netOut[j*numOutputs + k]
                        > pickedValue)
                    {
                        pickedValue =
                            netOut[j*numOutputs + k];
                        picked = j*numOutputs + k;
                    }
                }

                //verify
                if (picked != -1)
                    if (labelsv[picked] == 1.0)
                        ++goodv;
            }

            //update test score set
            if (good > stats.sTestScore) {
                stats.sEval = stats.eval;
                stats.sFitness = fitness;
                stats.sTestScore = good;
                stats.sValScore = goodv;
            }

            //update fitness set
            if (fitness > stats.fFitness) {
                stats.fEval = stats.eval;
                stats.fFitness = fitness;
                stats.fTestScore = good;
                stats.fValScore = goodv;
            }

            //update validation set
            if (goodv >= stats.vValScore) {
                stats.vEval = stats.eval;
                stats.vFitness = fitness;
                stats.vTestScore = good;
                stats.vValScore = goodv;
            }

            //print stats
            draw();

            //check goal
            if (good == numRows
            && goodv == numRowsv)
                break;
        } else if (
            stats.eval % (stats.maxEval / totalDots) == 0)
        {
            draw();
        }
    }//for evaluation

    //final screen
    stats.eval = stats.maxEval; // cause 101 / 100 !
    draw();
}//testIris

int main() {
    //testXor();
    testIris();
}//main
