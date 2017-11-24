/* Copyright (C) 2017 Wlodzimierz Samborski */

#include "pch.h"
#include "Species.h"

#include "Globals.h"

using namespace std;



Species::Species()
{}

Species::Species(
    const Genotype& first_)
:
    champion(first_),
    first(first_)
{}

void Species::computeAdjustedFitness()
{
    adjustedFitness = members[0]->fitness;
    for (int i=1; i<members.size(); ++i) {
        adjustedFitness += members[i]->fitness;
    }
    adjustedFitness /= members.size();
}

bool Species::isStagnant()
{
    bool improved = false;
    Genotype* bestMember = nullptr;
    for (auto member : members) {
        if (member->fitness > best) {
            bestMember = member;
            best = member->fitness;
            stagnantFor = 0;
            improved = true;
        }
    }
    
    if (improved) {
        champion = *bestMember;
        return false;
    }
        
    if (++stagnantFor >= Globals::speciesDropoffAfter)
        return true;
    
    return false;
}

void Species::selectParents()
{
    sort(
        members.begin(),
        members.end(),
        [](auto a, auto b) -> bool
        {
            return (a->fitness > b->fitness);
        });
    
    int numParents =
        ceil(members.size() * Globals::survivalThreshold);
    if (stagnantFor > 0) {
        parents.reserve(numParents + 1);
        parents.push_back(champion);
    } else {
        parents.reserve(numParents);
    }
    
    for (int i=0; i<numParents; ++i)
        parents.push_back(*(members[i]));
    members.clear();
}
