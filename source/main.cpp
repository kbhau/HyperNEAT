/* Copyright (C) 2017 Wlodzimierz Samborski */

#include "pch.h"
#include "TestIris.h"
#include "TestXor.h"



int main() {
    auto runXor = false;
    auto runIris = true;

    if (runXor) {
        TestXor testXor;
        testXor.perform(10);
    }

    if (runIris) {
        TestIris testIris;
        testIris.perform(50000);
    }
}
