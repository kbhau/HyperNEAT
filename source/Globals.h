/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once

/*
Class holding parameters and common functions.
*/
class Globals {
public:
    /*
    Clamps a value to a range.
    */
    static Float clamp(
        Float value,
        Float minVal, 
        Float maxVal);

    /*
    Returns a uniform real value in range [0, 1).
    */
    static Float randf();

    /*
    Returns a normally distributed real value with mean = 0 and
    standard deviation = 1.
    */
    static Float randNorm();
    
    /*
    Sets the seed for the random number generator.
    */
    static void setSeed(int seed);

    //parameters

    static Float costCoefCppnLinks;
    static Float costCoefOutputBiases;
    static Float costCoefOutputLinks;
    static Float costOutputBiasAbsence;
    static Float costPowerCppnLinks;
    static Float costPowerOutputBiases;
    static Float costPowerOutputLinks;
    static Float fitnessBase;
    static Float fitnessNan;
    static int costThresholdCppnLinksMax;
    static int costThresholdCppnLinksMin;
    static int costThresholdOutputBiasesMax;
    static int costThresholdOutputBiasesMin;
    static int costThresholdOutputLinksMax;
    static int costThresholdOutputLinksMin;

    static Float crossoverChance;
    static Float crossoverOnlyChance;
    static Float linkCreateChance;
    static Float linkDisableChance;
    static Float linkInitialMutationStep;
    static Float linkMinimalMutationStep;
    static Float linkRecurrentChance;
    static Float linkReenableChance;
    static Float neuronCreateChance;
    static Float neuronMutateChance;
    static Float similarityDisjointCoefficient;
    static Float similarityThreshold;
    static Float similarityThresholdModifier;
    static Float similarityWeightCoefficient;
    static Float survivalThreshold;
    static int numCPPNFunctions;
    static int populationSize;
    static int speciesDropoffAfter;
    static int speciesMassExtinctionAfter;
    static int speciesTargetNumber;
    
private:
    static RNG rng;
    static FRNG realGenerator;
    static NRNG normalGenerator;
};



inline
Float Globals::clamp(
    Float value,
    Float minVal,
    Float maxVal)
{
    return std::min(std::max(value, minVal), maxVal);
}

inline
void Globals::setSeed(int seed)
{
    rng.seed(seed);
}
