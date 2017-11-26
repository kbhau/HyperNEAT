/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once



/*
Connection from one node to another.
*/
struct Connection {
    int from;
    int to;
};

/*
Operator needed by map.
*/
bool operator<(
    const Connection a,
    const Connection b);

/*
Encodes a link between two neurons.
*/
struct LinkGene {
    Connection link;
    Float weight;
    Float mutationStep;
    int innovationNumber = -1;
    bool enabled;
};

/*
Encodes a neuron.
*/
struct NeuronGene {
    Connection link;
    Float x;
    Float y;
    int functionNumber;
    int innovationNumber = -1;
};
