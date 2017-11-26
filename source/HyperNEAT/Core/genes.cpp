/* Copyright (C) 2017 Wlodzimierz Samborski */

#include "pch.h"
#include "genes.h"

bool operator<(
    const Connection a,
    const Connection b)
{
    return (a.from < b.from) ?
        true :
        (a.from == b.from) ?
            (a.to < b.to) :
            false;
}
