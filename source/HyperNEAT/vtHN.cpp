/* Copyright (C) 2017 Wlodzimierz Samborski */

#include "pch.h"
#include "vtHN.h"

#include "Genotype.h"

using namespace std;
using namespace vt;



void vtHN::evaluate(
    int id,
    Float fitness)
{
    //if nan, disqualify
    if (isnan(fitness)) {
        fitness = Globals::fitnessNan;
    } else {
        //compute costs
        auto cppnLinksCost = getDiscountedFitness(
            fitness,
            solutionStats[id].cppnLinks,
            Globals::costThresholdCppnLinksMin,
            Globals::costThresholdCppnLinksMax,
            Globals::costPowerCppnLinks);
        auto outputBiasesCost = getDiscountedFitness(
            fitness,
            solutionStats[id].outputBiases,
            Globals::costThresholdOutputBiasesMin,
            Globals::costThresholdOutputBiasesMax,
            Globals::costPowerOutputBiases);
        auto outputLinksCost = getDiscountedFitness(
            fitness,
            solutionStats[id].outputLinks,
            Globals::costThresholdOutputLinksMin,
            Globals::costThresholdOutputLinksMax,
            Globals::costPowerOutputLinks);
        
        //apply costs
        fitness = fitness * rawFitnessCoef
            + cppnLinksCost * Globals::costCoefCppnLinks
            + outputBiasesCost * Globals::costCoefOutputBiases
            + outputLinksCost * Globals::costCoefOutputLinks;
        
        //make sure it's rated higher than rejects
        if (solutionStats[id].outputLinks > 0
        && fitness < 2.0 * Globals::fitnessBase)
            fitness = 2.0 * Globals::fitnessBase;
    }

    //no biases is bad
    if (solutionStats[id].outputBiases == 0)
        fitness *= Globals::costOutputBiasAbsence;
    
    //pass the enhanced rating
    population.rateGenotype(fitness, id);
}

int vtHN::getId()
{
    return nextId;
}

Solution vtHN::getNext()
{
    while (true) {
        //epoch if no more solutions left in population
        if (++nextId >= Globals::populationSize) {
            population.stepGeneration();
            nextId = 0;
        }

        //get model
        auto solution =
            cppn.query(population.getGenotype(nextId));
        
        //fill stats
        solutionStats[nextId].cppnLinks = cppn.getLinksAmount();
        solutionStats[nextId].outputBiases =
            solution.biases.size();
        solutionStats[nextId].outputLinks =
            solution.links.size();
        
        //has links?
        if (solution.links.size() > 0U)
            return solution;
        else
            evaluate(nextId, Globals::fitnessBase);
    }
}

void vtHN::initialise(
    const Substrate& substrate)
{
    nextId = -1;
    archive.clear();
    Genotype::setArchive(&archive);
    cppn.setSubstrate(substrate);
    population.initialise();
    solutionStats.resize(Globals::populationSize);
    rawFitnessCoef = static_cast<Float>(1.0)
        - (Globals::costCoefCppnLinks
        + Globals::costCoefOutputBiases
        + Globals::costCoefOutputLinks);
}

void vtHN::readSettings(string path)
{
    string line;
    ifstream f(path);
    if (!f.is_open())
        throw runtime_error("Error while opening settings.");
    while (getline(f, line)) {
        //remove white space
        line.erase(
            remove_if(
                line.begin(),
                line.end(),
                bind(
                    isspace<char>,
                    placeholders::_1,
                    locale::classic())),
            line.end());

        //if empty, read next line
        if (line.empty())
            continue;
        
        //read the value
        auto value = stod(line.substr(
            line.find('=') + 1));
        
        //store value in the right variable
        line = line.substr(0, line.find('='));
        if (line == "costCoefCppnLinks")
            Globals::costCoefCppnLinks = value;
        else if (line == "costCoefOutputBiases")
            Globals::costCoefOutputBiases = value;
        else if (line == "costCoefOutputLinks")
            Globals::costCoefOutputLinks = value;
        else if (line == "costOutputBiasAbsence")
            Globals::costOutputBiasAbsence = value;
        else if (line == "costPowerCppnLinks")
            Globals::costPowerCppnLinks = value;
        else if (line == "costPowerOutputBiases")
            Globals::costPowerOutputBiases = value;
        else if (line == "costPowerOutputLinks")
            Globals::costPowerOutputLinks = value;
        else if (line == "costThresholdCppnLinksMax")
            Globals::costThresholdCppnLinksMax = value;
        else if (line == "costThresholdCppnLinksMin")
            Globals::costThresholdCppnLinksMin = value;
        else if (line == "costThresholdOutputBiasesMax")
            Globals::costThresholdOutputBiasesMax = value;
        else if (line == "costThresholdOutputBiasesMin")
            Globals::costThresholdOutputBiasesMin = value;
        else if (line == "costThresholdOutputLinksMax")
            Globals::costThresholdOutputLinksMax = value;
        else if (line == "costThresholdOutputLinksMin")
            Globals::costThresholdOutputLinksMin = value;
        else if (line == "crossoverChance")
            Globals::crossoverChance = value;
        else if (line == "crossoverOnlyChance")
            Globals::crossoverOnlyChance = value;
        else if (line == "fitnessBase")
            Globals::fitnessBase = value;
        else if (line == "fitnessNan")
            Globals::fitnessNan	= value;
        else if (line == "linkCreateChance")
            Globals::linkCreateChance = value;
        else if (line == "linkDisableChance")
            Globals::linkDisableChance = value;
        else if (line == "linkInitialMutationStep")
            Globals::linkInitialMutationStep = value;
        else if (line == "linkMinimalMutationStep")
            Globals::linkMinimalMutationStep = value;
        else if (line == "linkRecurrentChance")
            Globals::linkRecurrentChance = value;
        else if (line == "linkReenableChance")
            Globals::linkReenableChance = value;
        else if (line == "neuronCreateChance")
            Globals::neuronCreateChance = value;
        else if (line == "neuronMutateChance")
            Globals::neuronMutateChance = value;
        else if (line == "populationSize")
            Globals::populationSize = value;
        else if (line == "similarityDisjointCoefficient")
            Globals::similarityDisjointCoefficient = value;
        else if (line == "similarityThreshold")
            Globals::similarityThreshold = value;
        else if (line == "similarityThresholdModifier")
            Globals::similarityThresholdModifier = value;
        else if (line == "similarityWeightCoefficient")
            Globals::similarityWeightCoefficient = value;
        else if (line == "speciesDropoffAfter")
            Globals::speciesDropoffAfter = value;
        else if (line == "speciesMassExtinctionAfter")
            Globals::speciesMassExtinctionAfter = value;
        else if (line == "speciesTargetNumber")
            Globals::speciesTargetNumber = value;
        else if (line == "survivalThreshold")
            Globals::survivalThreshold = value;
        else {
            string str = "Invalid variable in settings: ";
            str += line;
            throw runtime_error(str);
        }
    }
    if (f.bad())
        throw runtime_error("Error while reading settings.");
}
