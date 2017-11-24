/* Copyright (C) 2017 Wlodzimierz Samborski */

#pragma once

/*
Represents a single data row.
*/
template <
    int numDataFields,
    int numLabelFields>
struct LabeledRow {
    std::array<Float, numDataFields> data;
    std::array<Float, numLabelFields> label;
};
