#include <iostream>
#include "Functions.hpp"
using namespace std;

int main()
{
    auto initialLattice = initLattice();
    printLattice(initialLattice);

    auto burnedLattice = initialLattice;
    burningMethod(burnedLattice);
    printLattice(burnedLattice);

    // auto clusteredLettice = initialLattice;
    // clusterDistribution(clusteredLettice);
    // printLattice(clusteredLettice);
}