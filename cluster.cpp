#include <iostream>
#include "Functions.hpp"
#include "ClusterDistribution.hpp"
#include <chrono>
using namespace std;

int main(int argc, char *argv[])
{
    map<string, string> input = parseInput();
    string sL = input["L"];
    string sT = input["T"];
    const unsigned int L = atoi(sL.c_str());
    const unsigned int T = atoi(sT.c_str());
    double p0 = atof(input["p0"].c_str());
    const double dp = atof(input["dp"].c_str());
    const double pk = atof(input["pk"].c_str());
    vector<vector<unsigned int>> Lattice;
    Lattice.resize(L, vector<unsigned int>(L));

    map<unsigned int, int> clusterDistMap;
    map<int, int> cummulativeDistMap;
    map<int, double> normalizedDistMap;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(0, 1);

    const double EPSILON = 0.0000001;
    auto start = std::chrono::high_resolution_clock::now();
    while((pk - p0) >= EPSILON) {
        cleanDistFile(toStringWithPrecision(p0), sL, sT);
        // cout << "p = " << p0 <<endl;
        for (int i = T; i > 0; --i) {
            auto initialLattice = initLattice(Lattice, p0, gen, dis);
            //printLattice(initialLattice);
            auto clusteredLettice = initialLattice;
            auto max_el = clusterDistribution(clusteredLettice, clusterDistMap);
            if (i == T) cummulativeClusterDistribution(clusterDistMap, true);
            if (i == 1) cummulativeDistMap = cummulativeClusterDistribution(clusterDistMap, false);
            else cummulativeClusterDistribution(clusterDistMap, false);
            // printLattice(clusteredLettice);
        }
        normalizedDistMap = normalizedClusterDistributionData(cummulativeDistMap);
        saveToFileDist(sL, sT, p0, normalizedDistMap);
        p0 += dp;
        clusterDistMap.clear();
        cummulativeDistMap.clear();
        normalizedDistMap.clear();
        // cout << endl;
    }
    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;
    cout << "Elapsed time: " << elapsed.count() << " s\n";
}