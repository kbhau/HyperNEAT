/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once

#include "genes.h"

class Archive;

/*
A genotype for underlying NEAT implementation.
*/
class Genotype {
public:
    /*
    Creates a child with mutation only.
    */
    void createByMutationOnly(
        const Genotype& a);
    
    /*
    Creates a child by performing crossover and mutation.
    */
    void createByCrossover(
        const Genotype& a,
        const Genotype& b);
    
    /*
    Creates a basic genotype with one link.
    */
    void createFromScratch(
        int numInputs,
        int numOutputs);
    
    /*
    Returns a reference to a neuron of given id. Throws
    out_of_range if not there.
    */
    const NeuronGene& getNeuron(
        int innovationNumber) const;
    
    /*
    Returns similarity measure between this genotype and passed
    one.
    */
    Float getSimilarityMeasure(
        const Genotype& other) const;

    /*
    Sets the archive to use by all Genotypes. Probably use only
    on initialisation and changing the algorithm run.
    */
    static void setArchive(Archive* newArchive);

    Float fitness = -1.0;
    int speciesHint = -1;

private:
    /*
    Adds a link if it finds an unconnected pair. Intent is to
    evolve CPPNs so there will be no recurrent links.
    */
    void addLink();

    /*
    Picks a link and splits it in two. Old link is disabled.
    */
    void addNeuron();
    
    /*
    Checks if link between nodes at given innovation numbers is
    present. Returns -1 on absence, -2 on presence and link index
    if link is disabled - can reenable it.
    */
    int isLinkPresent(
        int from,
        int to);
    
    /*
    Returns a vector of pairs of genes. Index corresponds to
    innovation number. Innovation number == -1 is absent gene.
    */
    template <class T>
    std::vector<std::pair<T, T>> lineUp(
        const std::vector<T>& a,
        const std::vector<T>& b) const;

    /*
    Performs mutation on genes.
    */
    void mutate();

    static Archive* archive;
    
    std::vector<LinkGene> linkGenes;
    std::vector<NeuronGene> neuronGenes;
    int numInputs;
    int numOutputs;
    int numIONodes;

    //For linkGenes...
    friend class Cppn;
};

#include "Genotype.hpp"
