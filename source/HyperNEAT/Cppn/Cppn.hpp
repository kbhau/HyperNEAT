/* Copyright (C) 2017 Wlodzimierz Samborski */

inline
void Cppn::createNeuronIfAbsent(
    int neuronId,
    const Genotype& genes)
{
    if (nodes[neuronId].y == -1.0) {
        auto& node = genes.getNeuron(neuronId);
        nodes[neuronId] = std::move(CppnNeuron(
            node.y,
            node.functionNumber));
    }
}

inline
int Cppn::getLinksAmount()
{
    return links.size();
}
