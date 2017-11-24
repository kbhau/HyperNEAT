/* Copyright (C) 2017 Wlodzimierz Samborski */

#include "pch.h"
#include "Population.h"

#include "Globals.h"

using namespace std;



const Genotype& Population::getGenotype(
    int id)
{
    return population[id];
}

void Population::initialise()
{
    //set variables
    best = -1.0;
    similarityThreshold = Globals::similarityThreshold;
    speciesId = 0;
    stagnantFor = 0;
    species.clear();
    population.clear();

    //fill the population with species hint 0
    population.resize(Globals::populationSize);
    for (auto& unit : population) {
        unit.createFromScratch(5, 4);
        unit.speciesHint = 0;
    }

    //create species 0 from genotype 0
    species.insert(make_pair(0, move(Species(population[0]))));
}

void Population::rateGenotype(
    Float fitness,
    int id)
{
    population[id].fitness = fitness;
}

void Population::stepGeneration()
{
    //erase species
    for (auto& kv : species) {
        kv.second.members.clear();
        kv.second.best = -1.0;
    }

    //sort into species
    for (auto& unit : population) {
        auto distance =
            unit.getSimilarityMeasure(
                species[unit.speciesHint].first);
        if (distance < similarityThreshold) {
            species[unit.speciesHint].members.push_back(&unit);
        } else {
            auto assigned = false;
            for (auto& kv : species) {
                if (kv.first != unit.speciesHint) {
                    distance = unit.getSimilarityMeasure(
                        kv.second.first);
                    if (distance < similarityThreshold) {
                        species[unit.speciesHint].members
                            .push_back(&unit);
                        assigned = true;
                        break;
                    }
                }
            }
            if (!assigned) {
                species.insert(make_pair(
                    ++speciesId,
                    move(Species(unit))));
                species[speciesId].members.push_back(&unit);
            }
        }
    }

    //remove empty species - should not need
    vector<int> blacklist;
    for (auto& kv : species)
        if (kv.second.members.size() == 0)
            blacklist.push_back(kv.first);
    for (auto id : blacklist)
        species.erase(id);

    //adjust compatibility threshold
    if (species.size() < Globals::speciesTargetNumber)
        similarityThreshold -=
            Globals::similarityThresholdModifier;
    else if (species.size() > Globals::speciesTargetNumber)
        similarityThreshold +=
            Globals::similarityThresholdModifier;
    if (similarityThreshold
        < Globals::similarityThresholdModifier)
    {
        similarityThreshold =
            Globals::similarityThresholdModifier;
    }

    //remove stagnant species
    blacklist.clear();
    for (auto& kv : species)
        if (kv.second.isStagnant())
            blacklist.push_back(kv.first);
    for (auto id : blacklist)
        species.erase(id);

    //if population is stagnant, leave only 2 top species
    auto stagnant = true;
    for (auto& kv : species) {
        if (kv.second.best > best) {
            stagnant = false;
            best = kv.second.best;
        }
    }
    if (stagnant) {
        if (++stagnantFor
            >= Globals::speciesMassExtinctionAfter)
        {
            //sort
            deque<pair<Float, int>> top;
            for (auto& kv : species) {
                auto placed = false;
                for (int i=0; i<top.size(); ++i) {
                    if (kv.second.best > top[i].first) {
                        top.insert(
                            top.begin() + i,
                            make_pair(
                                kv.second.best,
                                kv.first));
                        placed = true;
                        break;
                    }
                }
                if (!placed) {
                    top.push_back(make_pair(
                        kv.second.best,
                        kv.first));
                }
            }

            //leave top 2
            for (int i=2; i<top.size(); ++i)
                species.erase(top[i].second);
            
            //count again
            stagnantFor = 0;
        }
    } else {
        stagnantFor = 0;
    }

    //sum adjusted fitness
    Float adjustedFitnessSum = 0.0;
    for (auto& kv : species) {
        kv.second.computeAdjustedFitness();
        adjustedFitnessSum += kv.second.adjustedFitness;
    }

    //compute number of offspring from each species
    auto numTotalOffspring = 0;
    for (auto& kv : species) {
        kv.second.numOffspring = ceil(
            (kv.second.adjustedFitness / adjustedFitnessSum)
            * static_cast<Float>(Globals::populationSize));
        numTotalOffspring += kv.second.numOffspring;
    }

    //find error distributing offspring
    auto error = Globals::populationSize - numTotalOffspring;

    //fix error
    if (error < 0) {
        //keep reducing until error spent
        while (error < 0) {
            //find biggest
            auto most = 0;
            auto mostId = -1;
            for (auto& kv : species) {
                if (kv.second.numOffspring > most) {
                    most = kv.second.numOffspring;
                    mostId = kv.first;
                }
            }

            //reduce offspring by 1
            if (species[mostId].numOffspring > 1) {
                species[mostId].numOffspring -= 1;
                error += 1;
            } else {
                string str = "Population size error too big: ";
                str += to_string(error);
                throw logic_error(str);
            }
        }
    } else if (error > 0) {
        //keep adding until error spent
        while (error > 0) {
            //find smallest
            auto least = 0;
            auto leastId = -1;
            for (auto& kv : species) {
                if (kv.second.numOffspring < least) {
                    least = kv.second.numOffspring;
                    leastId = kv.first;
                }
            }

            //add 1 offspring
            species[leastId].numOffspring += 1;
            error -= 1;
        }
    }

    //select parents
    for (auto& kv : species) {
        kv.second.selectParents();
    }

    //produce offspring
    auto genotypeId = -1;
    auto parentA = -1;
    auto parentB = -1;
    auto pick = static_cast<Float>(0.0);
    auto sumFitness = static_cast<Float>(0.0);
    auto sumSpent = static_cast<Float>(0.0);
    for (auto& kv : species) {
        if (kv.second.parents.size() == 1) {
            //for only 1 parent produce mutated clones
            for (int i=0; i<kv.second.numOffspring; ++i) {
                population[++genotypeId].createByMutationOnly(
                    kv.second.parents[0]);
                population[genotypeId].speciesHint = kv.first;
            }
        } else if (kv.second.parents.size() == 2) {
            //2 parents, sum the fitness
            sumFitness = kv.second.parents[0].fitness
                + kv.second.parents[1].fitness;

            //produce offspring
            for (int i=0; i<kv.second.numOffspring; ++i) {
                if (Globals::randf() < Globals::crossoverChance)
                    //if crossover then pick both
                    population[++genotypeId].createByCrossover(
                        kv.second.parents[0],
                        kv.second.parents[1]);
                else {
                    //if no crossover pick one - odds by fitness
                    parentA = 
                        (Globals::randf() * sumFitness
                            < kv.second.parents[0].fitness) ?
                                0 : 1;
                    
                    //create child
                    population[++genotypeId]
                        .createByMutationOnly(
                            kv.second.parents[parentA]);
                }
                population[genotypeId].speciesHint =
                    kv.first;
            }
        } else {
            //many parents, sum fitness
            sumFitness = static_cast<Float>(0.0);
            for (int i=0; i<kv.second.parents.size(); ++i)
                sumFitness += kv.second.parents[i].fitness;

            //produce pffspring
            for (int i=0; i<kv.second.numOffspring; ++i) {
                //select first parent
                pick = Globals::randf() * sumFitness;
                sumSpent = static_cast<Float>(0.0);
                for (int j=0; j<kv.second.parents.size(); ++j) {
                    sumSpent += kv.second.parents[j].fitness;
                    if (pick < sumSpent) {
                        parentA = j;
                        break;
                    }
                }

                //produce offspring
                if (Globals::randf()
                    < Globals::crossoverChance)
                {
                    //if crossover then find the other parent
                    do {
                        pick = Globals::randf() * sumFitness;
                        sumSpent = static_cast<Float>(0.0);
                        for (int j=0;
                            j<kv.second.parents.size();
                            ++j)
                        {
                            sumSpent +=
                                kv.second.parents[j].fitness;
                            if (pick < sumSpent) {
                                parentB = j;
                                break;
                            }
                        }
                    } while (parentB == parentA);
                    
                    //create child
                    population[++genotypeId].createByCrossover(
                        kv.second.parents[parentA],
                        kv.second.parents[parentB]);
                } else {
                    //if no crossover then 1 parent does the job
                    population[++genotypeId]
                        .createByMutationOnly(
                            kv.second.parents[parentA]);
                }
                population[genotypeId].speciesHint = kv.first;
            }
        }
    }

    //clear parents
    for (auto& kv : species) {
        kv.second.parents.clear();
    }
}
