/* Copyright (C) 2017 Wlodzimierz Samborski */

#include "pch.h"
#include "Genotype.h"

#include "Archive.h"
#include "Globals.h"

Archive* Genotype::archive = nullptr;

using namespace std;



void Genotype::addLink()
{
    //pick first neuron
    auto from = numInputs;
    while (from >= numInputs && from < numIONodes)
        from = rand() % neuronGenes.size();

    //store possible outputs
    NeuronGene* fromNeuron = &(neuronGenes[from]);
    deque<int> outputIndexes;
    if (Globals::randf() < Globals::linkRecurrentChance) {
        //recurrent only
        for (int i=0; i<neuronGenes.size(); ++i)
            if ((i < numInputs || i >= numIONodes)
            && (neuronGenes[i].y <= fromNeuron->y))
                outputIndexes.push_back(i);
    } else {
        //forwards only
        for (int i=numInputs; i<neuronGenes.size(); ++i)
            if (neuronGenes[i].y > fromNeuron->y)
                outputIndexes.push_back(i);
    }

    //pick second neuron
    auto to = -1;
    NeuronGene* toNeuron = nullptr;
    while (outputIndexes.size() > 0U ) {
        to = rand() % outputIndexes.size();
        toNeuron = &(neuronGenes[outputIndexes[to]]);

        //check if link already present
        auto presence = isLinkPresent(
            fromNeuron->innovationNumber,
            toNeuron->innovationNumber);
    
        //erase from options if already there and look again
        if (presence == -2) {
            outputIndexes.erase(outputIndexes.begin() + to);
            continue;
        }

        //enable if disabled, work done
        if (presence >= 0) {
            linkGenes[presence].enabled = true;
            return;
        }

        //create and fill the link, work done
        auto pair = LinkPair{
            fromNeuron->innovationNumber,
            toNeuron->innovationNumber};
        linkGenes.push_back(LinkGene{
            pair,
            Globals::randNorm(),
            Globals::linkInitialMutationStep,
            archive->getLinkInnovation(pair),
            true
        });
        return;
    }
}

void Genotype::addNeuron()
{
    //find available links
    vector<int> ids;
    ids.reserve(linkGenes.size());
    for (int i=0; i<linkGenes.size(); ++i)
        if (linkGenes[i].enabled)
            ids.push_back(i);

    //if no links, can't do
    if (ids.size() == 0)
        return;

    //pick random enabled link and disable it
    LinkGene* link = &(linkGenes[ids[rand() % ids.size()]]);
    link->enabled = false;

    //find linked neurons
    auto goals = 0;
    NeuronGene* fromNeuron = nullptr;
    NeuronGene* toNeuron = nullptr;
    for (auto& neuron : neuronGenes) {
        if (neuron.innovationNumber == link->link.from) {
            ++goals;
            fromNeuron = &neuron;
        }
        if (neuron.innovationNumber == link->link.to) {
            ++goals;
            toNeuron = &neuron;
        }
    }

    //test
    if (goals < 2)
        throw runtime_error("Couldn't find nodes in addNeuron.");

    //create node
    auto node = NeuronGene{
        link->link,
        (fromNeuron->x + toNeuron->x) * 0.5,
        (fromNeuron->y + toNeuron->y) * 0.5,
        rand() % Globals::numCPPNFunctions,
        archive->getNeuronInnovation(link->link)
    };

    //create new link from node and copy old weight
    LinkPair pair{
        node.innovationNumber,
        toNeuron->innovationNumber
    };
    linkGenes.push_back(LinkGene{
        pair,
        link->weight,
        link->mutationStep,
        archive->getLinkInnovation(pair),
        true
    });

    //create new link to node with weight 1
    pair.from = fromNeuron->innovationNumber;
    pair.to = node.innovationNumber;
    linkGenes.push_back(LinkGene{
        pair,
        1.0,
        Globals::linkInitialMutationStep,
        archive->getLinkInnovation(pair),
        true
    });

    //add node
    neuronGenes.push_back(move(node));

//test2
    if (pair.from > 30000) {
        archive->printNeuronInnovations();
        throw runtime_error("JESUS!!!");
    }    
}

void Genotype::createByMutationOnly(
    const Genotype& a)
{
    *this = a;
    mutate();
}

void Genotype::createByCrossover(
    const Genotype& a,
    const Genotype& b)
{
    linkGenes.clear();
    neuronGenes.clear();

    numInputs = a.numInputs;
    numOutputs = a.numOutputs;
    numIONodes = a.numIONodes;

    //should perform mutation
    auto crossoverOnly =
        (Globals::randf() < Globals::crossoverOnlyChance) ?
        true :
        false;

    //make fitter first and line up
    auto random = (a.fitness == b.fitness) ? true : false;
    vector<pair<LinkGene, LinkGene>> links;
    vector<pair<NeuronGene, NeuronGene>> neurons;
    if (a.fitness > b.fitness) {
        links = lineUp(a.linkGenes, b.linkGenes);
        neurons = lineUp(a.neuronGenes, b.neuronGenes);
    } else {
        links = lineUp(b.linkGenes, a.linkGenes);
        neurons = lineUp(b.neuronGenes, a.neuronGenes);
    }
    
    //neurons - matching: random, disjoint: all
    for (auto& pair : neurons) {
        if (pair.first.innovationNumber != -1
            && pair.second.innovationNumber != -1)
        {
            if (rand() % 2 == 0)
                neuronGenes.push_back(pair.first);
            else
                neuronGenes.push_back(pair.second);
        } else if (pair.first.innovationNumber != -1) {
            neuronGenes.push_back(pair.first);
        } else if (pair.second.innovationNumber != -1) {
            neuronGenes.push_back(pair.second);
        }
    }

    //links
    if (random) {
        //matching: random, disjoint: random
        for (auto& pair : links) {
            if (pair.first.innovationNumber != -1
                && pair.second.innovationNumber != -1)
            {
                if (rand() % 2 == 0)
                    linkGenes.push_back(pair.first);
                else
                    linkGenes.push_back(pair.second);
            } else if (pair.first.innovationNumber != -1) {
                if (rand() % 2 == 0)
                    linkGenes.push_back(pair.first);
            } else if (pair.second.innovationNumber != -1) {
                if (rand() % 2 == 0)
                    linkGenes.push_back(pair.second);
            }
        }
    } else {
        //matching: random, disjoint: fitter
        for (auto& pair : links) {
            if (pair.first.innovationNumber != -1
                && pair.second.innovationNumber != -1)
            {
                if (rand() % 2 == 0)
                    linkGenes.push_back(pair.first);
                else {
                    linkGenes.push_back(pair.second);
                    if (!crossoverOnly
                    && Globals::randf()
                    < Globals::linkDisableChance)
                        linkGenes.back().enabled = false;
                }
            } else if (pair.first.innovationNumber != -1) {
                linkGenes.push_back(pair.first);
            }
        }
    }

    //mutate child
    if (!crossoverOnly)
        mutate();
}

void Genotype::createFromScratch(
    int numInputs,
    int numOutputs)
{
    linkGenes.clear();
    neuronGenes.clear();

    //set numbers
    this->numInputs = numInputs;
    this->numOutputs = numOutputs;
    numIONodes = numInputs + numOutputs;

    //create io neurons
    neuronGenes.resize(numIONodes);

    //fill the properties
    for (int i=0; i<numIONodes; ++i) {
        if (i < numInputs) {    //for input neurons
            //2D position
            neuronGenes[i].x =
                {static_cast<Float>(i)
                / static_cast<Float>(numInputs - 1)};
            neuronGenes[i].y = {0.0};
        } else {    //for output neurons
            //2D position
            neuronGenes[i].x =
                {static_cast<Float>(i - numInputs)
                / static_cast<Float>(numInputs - 1)};
            neuronGenes[i].y = {1.0};
        }
        //random activation function
        neuronGenes[i].functionNumber =
            rand() % Globals::numCPPNFunctions;

        //pair, -1 is impossible so will not be duplicated
        neuronGenes[i].link = {-1, i};

        //assign innovation number - must the same across initial
        //population
        neuronGenes[i].innovationNumber =
            archive->getNeuronInnovation(
                neuronGenes[i].link);
    }
    
    for (int i=0; i<2; ++i)
        addLink();
}

const NeuronGene& Genotype::getNeuron(
        int innovationNumber) const
{
    for (auto& neuronGene : neuronGenes)
        if (neuronGene.innovationNumber == innovationNumber)
            return neuronGene;
    
    string str = "Neuron does not exist: ";
    str += to_string(innovationNumber);
    throw out_of_range(str);
}

Float Genotype::getSimilarityMeasure(
    const Genotype& other) const
{
    auto numDisjoint = 0;
    auto numMatching = 0;
    Float weightDifferenceSum = 0.0;
    auto line = lineUp(linkGenes, other.linkGenes);
    auto numGenes = (linkGenes.size() > other.linkGenes.size()) ?
        linkGenes.size() :
        other.linkGenes.size();
    Float disjointDivisor = (numGenes < 20) ?
        sqrt(numGenes) :
        static_cast<Float>(numGenes);

    for (auto& pair : line) {
        if (pair.first.innovationNumber != -1
            && pair.second.innovationNumber != -1)
        {
            ++numMatching;
            weightDifferenceSum += abs(
                pair.first.weight - pair.second.weight);
        } else if (pair.first.innovationNumber != -1
            || pair.second.innovationNumber != -1)
        {
            ++numDisjoint;
        }
    }

    return
        (weightDifferenceSum / static_cast<Float>(numMatching))
        * Globals::similarityWeightCoefficient
        +
        (static_cast<Float>(numDisjoint) / disjointDivisor)
        * Globals::similarityDisjointCoefficient;
}

int Genotype::isLinkPresent(
        int from,
        int to)
{
    for (int i=0; i<linkGenes.size(); ++i)
        if (linkGenes[i].link.from == from
            && linkGenes[i].link.to == to)
        {
            if (linkGenes[i].enabled)
                return -2;
            else
                return i;
        }
    
    return -1;
}

template <class T>
vector<pair<T, T>> Genotype::lineUp(
    const vector<T>& a,
    const vector<T>& b) const
{
    //get required size
    auto count = a[0].innovationNumber;
    for (int i=1; i<a.size(); ++i)
        if (a[i].innovationNumber > count)
            count = a[i].innovationNumber;
    for (int i=0; i<b.size(); ++i)
        if (b[i].innovationNumber > count)
            count = b[i].innovationNumber;
    
    //allocate space
    vector<pair<T, T>> lineup(count+1);

    //assign values
    for (auto& gene : a)
        lineup[gene.innovationNumber].first = gene;
    for (auto& gene : b)
        lineup[gene.innovationNumber].second = gene;
    
    return lineup;
}

void Genotype::mutate()
{
    //mutate links
    for (auto& link : linkGenes) {
        if (link.enabled) {
            //adjust mutation step
            link.mutationStep += Globals::randNorm();
            if (link.mutationStep
                < Globals::linkMinimalMutationStep)
            {
                link.mutationStep =
                    Globals::linkMinimalMutationStep;
            }
            
            //modify the weight
            link.weight +=
                Globals::randNorm() * link.mutationStep;
        } else {
            if (Globals::randf() < Globals::linkReenableChance)
                link.enabled = true;
        }
    }

    //mutate neurons
    for (auto& neuron : neuronGenes) {
        if (neuron.y > 0.0
        && Globals::randf() < Globals::neuronMutateChance)
            neuron.functionNumber =
                rand() % Globals::numCPPNFunctions;
    }

    //add new neuron
    if (Globals::randf() < Globals::neuronCreateChance)
        addNeuron();

    //add new link
    if (Globals::randf() < Globals::linkCreateChance)
        addLink();
}

void Genotype::setArchive(Archive* newArchive)
{
    archive = newArchive;
}
