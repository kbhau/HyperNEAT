/* Copyright (C) 2017 Wlodzimierz Samborski */

template <class T>
std::vector<std::pair<T, T>> Genotype::lineUp(
    const std::vector<T>& a,
    const std::vector<T>& b) const
{
    //get required size
    int count = a[0].innovationNumber;
    for (int i=1; i<a.size(); ++i)
        if (a[i].innovationNumber > count)
            count = a[i].innovationNumber;
    for (int i=0; i<b.size(); ++i)
        if (b[i].innovationNumber > count)
            count = b[i].innovationNumber;
    
    //allocate space
    std::vector<std::pair<T, T>> lineup(count+1);

    //assign values
    for (auto& gene : a)
        lineup[gene.innovationNumber].first = gene;
    for (auto& gene : b)
        lineup[gene.innovationNumber].second = gene;
    
    return lineup;
}
