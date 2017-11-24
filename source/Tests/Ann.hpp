/* Copyright (C) 2017 Wlodzimierz Samborski */

inline
Float Ann::activationFunction(
    Float x)
{
    //all day wasted looking for a bug caused by bad abs... :(
    return (x / (static_cast<Float>(0.25) + std::fabs(x)))
        * static_cast<Float>(0.5)
        + static_cast<Float>(0.5);
}

template<
    int tNumInputs,
    int tNumOutputs>
void Ann::snapshot(
    std::array<Float, tNumInputs>& in,
    std::array<Float, tNumOutputs>& out)
{
    //set clean
    for (auto& kv : nodes) {
        if (kv.first < numInputs)
            kv.second.out = in[kv.first];
        else
            kv.second.out = 0.0;
        kv.second.sum = 0.0;
    }
    
    //process
    for (int i=1; i<depth; ++i) {
        //propagate
        for (auto& l : links) {
            l.to->sum += l.from->out * l.weight;
        }

        //fire
        for (auto& kv : nodes) {
            if (kv.first < numInputs
            || kv.first >= numIONodes) {
                kv.second.out = activationFunction(
                    kv.second.sum + kv.second.bias);
                kv.second.sum = 0.0;
            }
        }
    }

    //save output
    out.fill(-1.0);
    for (int field=0; field<out.size(); ++field) {
        out[field] = activationFunction(
            nodes[numInputs + field].sum 
            + nodes[numInputs + field].bias);
    }
}
