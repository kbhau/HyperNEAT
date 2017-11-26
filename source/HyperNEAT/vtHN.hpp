/* Copyright (C) 2017 Wlodzimierz Samborski */

inline
Float vtHN::getDiscountedFitness(
    Float fitness,
    int links,
    int thresholdMin,
    int thresholdMax,
    Float power)
{
    if (links <= thresholdMin)
        return fitness;
    
    if (links >= thresholdMax)
        return Globals::fitnessBase;
    
    auto interpolationValue = pow(
        static_cast<Float>(links - thresholdMin)
        / static_cast<Float>(thresholdMax),
        power);
    
    return fitness
        * (static_cast<Float>(1.0) - interpolationValue)
        + interpolationValue * Globals::fitnessBase;
}
