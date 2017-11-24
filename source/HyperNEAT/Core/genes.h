/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once

struct LinkPair {
    int from;
    int to;
};

bool operator<(
    const LinkPair a,
    const LinkPair b);

struct LinkGene {
    LinkPair link;
    Float weight;
    Float mutationStep;
    int innovationNumber = -1;
    bool enabled;
};

struct NeuronGene {
    LinkPair link;
    Float x;
    Float y;
    int functionNumber;
    int innovationNumber = -1;
};
