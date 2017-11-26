/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once

#include "pch.h"



/*
Holds information about possible node placement.
*/
struct Substrate {
    std::vector<std::pair<Float, Float>> coords;
    int numInputs;
    int numIONodes;
};
