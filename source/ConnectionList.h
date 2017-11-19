/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once

class ConnectionList {
public:
    //Format: id, value.
    std::vector<std::pair<int, Float>> biases;

    //Format: fromId, toId, weight.
    std::vector<std::tuple<int, int, Float>> links;
};
