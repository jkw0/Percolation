#include <iostream>
#include "Functions.hpp"
#include "BurningMethod.hpp"
#include "ClusterDistribution.hpp"
using namespace std;

void cleanAveFile(const string& L, const string& T)
{
    ofstream aveFile;
    string name = "Ave_L" + L + "T" + T + ".txt"; 
    aveFile.open(name, ios::out);
    aveFile << "p  PFlow  MaxCluster\n";
    aveFile.close();
}

void cleanDistFile(const string& p, const string& L, const string& T)
{
    ofstream distFile;
    string name = "Dist_p" + p + "L" + L + "T" + T + ".txt";
    distFile.open(name, ios::out);
    distFile << "s  n(s,p,L)\n";
    distFile.close();
}

int main(int argc, char *argv[])
{
    string sL = argv[1];
    string sT = argv[2];
    const unsigned int L = atoi(sL.c_str());
    const unsigned int T = atoi(sT.c_str());
    double p0 = atof(argv[3]);
    const double dp = atof(argv[4]);
    const double pk = atof(argv[5]);
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

    while(p0 <= pk) {
        cleanDistFile(toStringWithPrecision(p0), sL, sT);
        cout << "p = " << p0 <<endl;
        for (int i = T; i > 0; --i) {
            cout << endl << "TRIAL = " << i << endl;
            auto initialLattice = initLattice(Lattice, p0, gen, dis);
            // cout << "Initial" << endl;
            // printLattice(initialLattice);
            auto burnedLattice = initialLattice;
            int connected = burningMethod(burnedLattice) ? 1 : 0;
            connectionData.push_back(connected);
            // printLattice(burnedLattice);
            auto clusteredLettice = initialLattice;
            auto max_el = clusterDistribution(clusteredLettice, clusterDistMap);
            maxClusterData.push_back(max_el);
            if (i == T) cummulativeClusterDistribution(clusterDistMap, true);
            if (i == 1) cummulativeDistMap = cummulativeClusterDistribution(clusterDistMap, false);
            else cummulativeClusterDistribution(clusterDistMap, false);
            // cout << "Clustered" << endl;
            // printLattice(clusteredLettice);
        }
        normalizedDistMap = normalizedClusterDistributionData(cummulativeDistMap);
        double Pflow = averageValue(connectionData);
        double maxClusterAvg = averageValue(maxClusterData);
        // cout << "Pflow = " << Pflow << endl;
        cout << "Max Cluster = " << maxClusterAvg << endl;
        saveToFileAve(sL, sT, p0, Pflow, maxClusterAvg);
        saveToFileDist(sL, sT, p0, normalizedDistMap);
        p0 += dp;
        cout << "p0=" << p0 << ", dp=" << dp << ", pk=" << pk << endl;
        connectionData.clear();
        maxClusterData.clear();
        clusterDistMap.clear();
        cummulativeDistMap.clear();
        normalizedDistMap.clear();
        cout << endl;
    }
}