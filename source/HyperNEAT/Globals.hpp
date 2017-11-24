inline
Float Globals::clamp(
    Float value,
    Float minVal,
    Float maxVal)
{
    return std::min(std::max(value, minVal), maxVal);
}

inline
void Globals::setSeed(int seed)
{
    rng.seed(seed);
}

/* Copyright (C) 2017 Wlodzimierz Samborski */
