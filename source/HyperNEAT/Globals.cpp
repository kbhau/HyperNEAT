/* Copyright (C) 2017 Wlodzimierz Samborski */

#include "pch.h"
#include "Globals.h"

using namespace vt;



//functions

Float Globals::randf()
{
    return realGenerator(rng);
}

Float Globals::randNorm()
{
    return normalGenerator(rng);
}



//parameters

Float Globals::costCoefCppnLinks                = 0.2;
Float Globals::costCoefOutputBiases             = 0.3;
Float Globals::costCoefOutputLinks              = 0.3;
Float Globals::costOutputBiasAbsence            = 0.85;
Float Globals::costPowerCppnLinks               = 1.0;
Float Globals::costPowerOutputBiases            = 1.0;
Float Globals::costPowerOutputLinks             = 1.0;
Float Globals::fitnessBase                      = 1.0e-6;
Float Globals::fitnessNan                       = 1.0e-8;
int Globals::costThresholdCppnLinksMax          = 100;
int Globals::costThresholdCppnLinksMin          = 50;
int Globals::costThresholdOutputBiasesMax       = 20;
int Globals::costThresholdOutputBiasesMin       = 5;
int Globals::costThresholdOutputLinksMax        = 50;
int Globals::costThresholdOutputLinksMin        = 10;

Float Globals::crossoverChance                  = 0.75;
Float Globals::crossoverOnlyChance              = 0.25;
Float Globals::linkCreateChance                 = 0.25;
Float Globals::linkDisableChance                = 0.025;
Float Globals::linkInitialMutationStep          = 2.5;
Float Globals::linkMinimalMutationStep          = 0.1;
Float Globals::linkRecurrentChance              = 0.15;
Float Globals::linkReenableChance               = 0.05;
Float Globals::neuronCreateChance               = 0.075;
Float Globals::neuronMutateChance               = 0.2;
Float Globals::similarityDisjointCoefficient    = 2.0;
Float Globals::similarityThreshold              = 6.0;
Float Globals::similarityThresholdModifier      = 0.3;
Float Globals::similarityWeightCoefficient      = 0.9;
Float Globals::survivalThreshold                = 0.2;
int Globals::numCPPNFunctions                   = 10;
int Globals::populationSize                     = 150;
int Globals::speciesDropoffAfter                = 15;
int Globals::speciesMassExtinctionAfter         = 20;
int Globals::speciesTargetNumber                = 10;



//generators

RNG Globals::rng = RNG{};
FRNG Globals::realGenerator = FRNG(0.0, 1.0);
NRNG Globals::normalGenerator = NRNG(0.0, 1.0);
