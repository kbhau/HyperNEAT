/* Copyright (C) 2017 Wlodzimierz Samborski */

#include "pch.h"
#include "TestIris.h"

using namespace std;



void TestIris::draw()
{
    //top line - 2
    for (int i=0; i<80; ++i)
        cout << "=";
    cout << endl << endl;
    //2

    //stats a - 6
    cout << " ] BEST BY FITNESS:" << endl;
    cout << " ] Eval. no.:\t" << fEval << endl;
    cout << " ] Test score:\t" << fTestScore << " / "
        << maxTestScore << "\t"
        << static_cast<Float>(fTestScore) * 100.0
        / static_cast<Float>(maxTestScore) << "%\n";
    cout << " ] Val. score:\t" << fValScore << " / "
        << maxValScore << "\t\t"
        << static_cast<Float>(fValScore) * 100.0
        / static_cast<Float>(maxValScore) << "%\n";
    cout << " ] Fitness:\t" << fFitness << "\n\n";
    //8

    //stats b - 6
    cout << " ] BEST BY TEST SCORE:" << endl;
    cout << " ] Eval. no.:\t" << sEval << endl;
    cout << " ] Test score:\t" << sTestScore << " / "
        << maxTestScore << "\t"
        << static_cast<Float>(sTestScore) * 100.0
        / static_cast<Float>(maxTestScore) << "%\n";
    cout << " ] Val. score:\t" << sValScore << " / "
        << maxValScore << "\t\t"
        << static_cast<Float>(sValScore) * 100.0
        / static_cast<Float>(maxValScore) << "%\n";
    cout << " ] Fitness:\t" << sFitness << "\n\n";
    //14

    //stats c - 6
    cout << " ] BEST BY VALIDATION:" << endl;
    cout << " ] Eval. no.:\t" << vEval << endl;
    cout << " ] Test score:\t" << vTestScore << " / "
        << maxTestScore << "\t"
        << static_cast<Float>(vTestScore) * 100.0
        / static_cast<Float>(maxTestScore) << "%\n";
    cout << " ] Val. score:\t" << vValScore << " / "
        << maxValScore << "\t\t"
        << static_cast<Float>(vValScore) * 100.0
        / static_cast<Float>(maxValScore) << "%\n";
    cout << " ] Fitness:\t" << vFitness << "\n\n";
    //20

    //progress bar - 3
    cout << " ] PROGRESS:\t" << eval << " / "
        << maxEval << endl << endl;
    auto dots = eval / (maxEval / totalDots);
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
}

void TestIris::importDataset()
{
    string line;
    LabeledRow<4, 3> row;

    //open file
    ifstream iris("../testData/iris.txt");
    if (!iris.is_open())
        throw runtime_error("Error while opening Iris data.");

    //process contents
    auto pos = 0;
    auto rowNum = 0;
    while (getline(iris, line)) {
        for (int i=0; i<4; ++i) {
            pos = line.find(',');
            row.data[i] = stod(line.substr(0, pos));
            line = line.substr(pos+1);
        }
        row.label.fill(0.0);
        if (line == "Iris-setosa") {
            row.label[0] = 1.0;
        } else if (line == "Iris-versicolor") {
            row.label[1] = 1.0;
        } else if (line == "Iris-virginica") {
            row.label[2] = 1.0;
        } else {
            throw logic_error("Error reading Iris data.");
        }
        if (rowNum < 45
        || (rowNum >= 50 && rowNum < 95)
        || (rowNum >= 100 && rowNum < 145))
            trainingSet.push_back(row);
        else
            testSet.push_back(row);
        ++rowNum;
    }

    if (iris.bad())
        throw runtime_error("Error while reading Iris dataset.");
}

void TestIris::init()
{
    //get data
    importDataset();

    //set up the substrate
    s.numInputs = 4;
    s.numIONodes = 3 + s.numInputs;
    numOutputs = s.numIONodes - s.numInputs;
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
    depth = 5;

    //init the core
    algo.readSettings("../settings/iris.txt");
    algo.initialise(s);
    Globals::setSeed(42);

    //init stats
    maxTestScore = trainingSet.size();
    maxValScore = testSet.size();
    fFitness = static_cast<Float>(0.0);
    fTestScore = 0;
    fValScore = 0;
    sFitness = static_cast<Float>(0.0);
    sTestScore = 0;
    sValScore = 0;
    vFitness = static_cast<Float>(0.0);
    vTestScore = 0;
    vValScore = 0;
}

void TestIris::perform(
    int numEvaluations)
{
    maxEval = numEvaluations;
    init();
    run();

    if (eval > maxEval)
        --eval;
    draw();
}

void TestIris::run()
{
    for (eval = 1; eval <= maxEval; ++eval) {
        auto solution = move(algo.getNext());

        //create network
        net.create(
            solution,
            s.numInputs,
            s.numIONodes - s.numInputs,
            depth);
            
        //evaluate
        auto error = static_cast<Float>(0.0);
        auto err2 = static_cast<Float>(0.0);
        auto score = 0;
        array<Float, 3> out;
        for (int row=0; row<trainingSet.size(); ++row) {
            net.snapshot<4, 3>(trainingSet[row].data, out);

            //pick an answer
            auto picked = -1;
            auto pickedValue = static_cast<Float>(0.0);
            for (int field=0; field<numOutputs; ++field) {
                if (out[field] > pickedValue) {
                    pickedValue = out[field];
                    picked = field;
                }

                error =
                    out[field] - trainingSet[row].label[field];
                err2 += error * error;
            }

            //verify
            if (picked != -1)
                if (trainingSet[row].label[picked] == 1.0)
                    ++score;
        }

        //map error to (0, 1], higher is better
        auto fitness = static_cast<Float>(1.0)
            + fabs(err2)
            / -(fabs(err2) + static_cast<Float>(1.0));

        //include accuracy measure - might help
        fitness = fitness * static_cast<Float>(0.25)
            + static_cast<Float>(0.75)
            * (static_cast<Float>(score)
            / static_cast<Float>(trainingSet.size()));

        //pass rating
        algo.evaluate(algo.getId(), fitness);

        //print findings
        if (score > sTestScore
        || fitness > fFitness)
        {
            //compute score on validation set
            auto testScore = 0;
            for (int row=0; row<testSet.size(); ++row) {
                net.snapshot<4, 3>(testSet[row].data, out);
                
                //pick an answer
                auto picked = -1;
                auto pickedValue = static_cast<Float>(0.0);
                for (int field=0; field<numOutputs; ++field) {
                    if (out[field] > pickedValue) {
                        pickedValue = out[field];
                        picked = field;
                    }
                }

                //verify
                if (picked != -1)
                    if (testSet[row].label[picked] == 1.0)
                        ++testScore;
            }

            //update test score set
            if (score > sTestScore) {
                sEval = eval;
                sFitness = fitness;
                sTestScore = score;
                sValScore = testScore;
            }

            //update fitness set
            if (fitness > fFitness) {
                fEval = eval;
                fFitness = fitness;
                fTestScore = score;
                fValScore = testScore;
            }

            //update validation set
            if (testScore > vValScore) {
                vEval = eval;
                vFitness = fitness;
                vTestScore = score;
                vValScore = testScore;
            }

            //print stats
            draw();

            //check goal
            if (score == static_cast<int>(trainingSet.size())
            && testScore == static_cast<int>(testSet.size()))
                break;
        } else if ((eval % (maxEval / totalDots)) == 0) {
            draw();
        }
    }//for eval
}
