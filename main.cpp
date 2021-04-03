#include <iostream>
#include "Functions.hpp"
using namespace std;


int main(int argc, char *argv[])
{
    const unsigned int L = atoi(argv[1]);
    const double p = atof(argv[2]);
    vector<vector<unsigned int>> Lattice;
    Lattice.resize(L, vector<unsigned int>(L));
    
    auto initialLattice = initLattice(Lattice, p);
    printLattice(initialLattice);

    auto burnedLattice = initialLattice;
    burningMethod(burnedLattice);
    printLattice(burnedLattice);

    // auto clusteredLettice = initialLattice;
    // clusterDistribution(clusteredLettice);
    // printLattice(clusteredLettice);
}