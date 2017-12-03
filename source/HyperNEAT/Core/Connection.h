/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once



namespace vt {

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

}//namespace
