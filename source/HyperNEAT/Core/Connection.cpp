/* Copyright (C) 2017 Wlodzimierz Samborski */

#include "Connection.h"



namespace vt {

bool operator<(
    const vt::Connection a,
    const vt::Connection b)
{
    return (a.from < b.from) ?
        true :
        (a.from == b.from) ?
            (a.to < b.to) :
            false;
}

}//namespace
