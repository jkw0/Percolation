#include <iostream>
#include "Functions.hpp"
#include "BurningMethod.hpp"
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
    
    cleanAveFile(sL, sT);

    vector<int> connectionData;
    vector<int> maxClusterData;
    map<unsigned int, int> clusterDistMap;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(0, 1);

    const double EPSILON = 0.0000001;
    auto start = std::chrono::high_resolution_clock::now();
    while((pk - p0) >= EPSILON) {
        // cout << "p = " << p0 <<endl;
        for (int i = T; i > 0; --i) {
            auto initialLattice = initLattice(Lattice, p0, gen, dis);
            //printLattice(initialLattice);
            auto burnedLattice = initialLattice;
            int connected = burningMethod(burnedLattice) ? 1 : 0;
            connectionData.push_back(connected);
            //printLattice(burnedLattice);
            auto clusteredLettice = initialLattice;
            auto max_el = clusterDistribution(clusteredLettice, clusterDistMap);
            maxClusterData.push_back(max_el);
        }
        double Pflow = averageValue(connectionData);
        double maxClusterAvg = averageValue(maxClusterData);
        saveToFileAve(sL, sT, p0, Pflow, maxClusterAvg);
        p0 += dp;
        connectionData.clear();
        maxClusterData.clear();
        clusterDistMap.clear();
    }

    auto finish = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = finish - start;
    cout << "Elapsed time: " << elapsed.count() << " s\n";
}