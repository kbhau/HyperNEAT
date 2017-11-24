/* Copyright (C) 2017 Wlodzimierz Samborski */

inline
void CppnSynapse::impulse()
{
    to->sum += from->out * weight;
}
