#include <iostream>
#include "Functions.hpp"
#include "BurningMethod.hpp"
#include "ClusterDistribution.hpp"
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
    map<int, int> cummulativeDistMap;
    map<int, double> normalizedDistMap;

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(0, 1);

    const double EPSILON = 0.0000001;
//TODO FIX in order to print 1.00
    while((pk - p0) >= EPSILON) {
        //cleanDistFile(toStringWithPrecision(p0), sL, sT);
        cout << "p = " << p0 <<endl;
        for (int i = T; i > 0; --i) {
            // cout << endl << "TRIAL = " << i << endl;
            auto initialLattice = initLattice(Lattice, p0, gen, dis);
            //printLattice(initialLattice);
            
            auto burnedLattice = initialLattice;
            int connected = burningMethod(burnedLattice) ? 1 : 0;
            connectionData.push_back(connected);
            // if (connected)
            //     printLattice(burnedLattice);
            auto clusteredLettice = initialLattice;
            auto max_el = clusterDistribution(clusteredLettice, clusterDistMap);
            maxClusterData.push_back(max_el);
            if (i == T) cummulativeClusterDistribution(clusterDistMap, true);
            if (i == 1) cummulativeDistMap = cummulativeClusterDistribution(clusterDistMap, false);
            else cummulativeClusterDistribution(clusterDistMap, false);
            // printLattice(clusteredLettice);
        }
        normalizedDistMap = normalizedClusterDistributionData(cummulativeDistMap);
        double Pflow = averageValue(connectionData);
        double maxClusterAvg = averageValue(maxClusterData);
        // cout << "Pflow = " << Pflow << endl;
        // cout << "Max Cluster = " << maxClusterAvg << endl;
        saveToFileAve(sL, sT, p0, Pflow, maxClusterAvg);
        //saveToFileDist(sL, sT, p0, normalizedDistMap);
        // cout << "p0=" << p0 << ", dp=" << dp << ", pk=" << pk << endl;
        p0 += dp;
        connectionData.clear();
        maxClusterData.clear();
        clusterDistMap.clear();
        cummulativeDistMap.clear();
        normalizedDistMap.clear();
        cout << endl;
    }
}