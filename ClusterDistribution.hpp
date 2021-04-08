#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>
using namespace std;

enum class NeighborsState {
    unoccupied = 0,
    aboveOccupied,
    leftOccupied,
    bothOccupied,
    other
};

NeighborsState checkIfNewCluster(const vector<vector<unsigned int>>& lattice
    , const unsigned i, const unsigned j
    , const unsigned max, unsigned& val, unsigned& val2)
{
    // cout << "i=" << i << ", j=" << j << endl;
    if (i != 0 && j != 0) {
        bool isAboveOccupied = false;
        bool isLeftOccupied = false;
        
        if ((lattice[i-1][j] == 0) && (lattice[i][j-1] == 0)) return NeighborsState::unoccupied;
        if (lattice[i-1][j] != 0 && lattice[i-1][j] != 1) {
            val = lattice[i-1][j];
            isAboveOccupied = true;
        }
        if (lattice[i][j-1] != 0 && lattice[i][j-1] != 1) {
            val2 = lattice[i][j-1];
            isLeftOccupied = true;
        }

        if (val == val2) return NeighborsState::aboveOccupied;
        if (isAboveOccupied && isLeftOccupied) {
            // cout << "bothOccupied if1" << endl;
            return NeighborsState::bothOccupied;
        }
        else if (isAboveOccupied && !isLeftOccupied) {
            // cout << "above occupied only if1" << endl;
            return NeighborsState::aboveOccupied;
        }
        else if (!isAboveOccupied && isLeftOccupied) {
            // cout << "left occupied only if1" << endl;
            return NeighborsState::leftOccupied;
        }
    }
    else {
        if (i == 0 && j == 0) return NeighborsState::unoccupied;
        else if (i == 0) {
            // cout << "i == 0 " << endl;
            if (lattice[i][j-1] == 0) return NeighborsState::unoccupied;
            else if (lattice[i][j-1] != 0 && lattice[i][j-1] != 1) {
                val2 = lattice[i][j-1];
                // cout << "left occupied only if2" << endl;
                return NeighborsState::leftOccupied;
            }
        }
        else if (j == 0) {
            // cout << "j == 0 " << endl;
            if (lattice[i-1][j] == 0) return NeighborsState::unoccupied;
            else if (lattice[i-1][j] != 0 && lattice[i-1][j] != 1) {
                val = lattice[i-1][j];
                // cout << "above occupied only if2" << endl;
                return NeighborsState::aboveOccupied;
            }
        }
    }
    
    return NeighborsState::other;
}

int clusterDistribution(vector<vector<unsigned int>>& lattice, map<unsigned int, int>& distribution)
{
    map<unsigned int, int> M;
    unsigned int k = 2;

    // find fist occupied element
    for (auto i = 0; i < lattice.size(); ++i) {
        for (auto j = 0; j < lattice.size(); ++j) {
            if (lattice[i][j] == 1) {
                lattice[i][j] = k;
                ++M[k];
                ++k;
                break;
            }
        }
        break; 
    }

    for (auto i = 0; i < lattice.size(); ++i) {
        for (auto j = 0; j < lattice.size(); ++j) {
            if (lattice[i][j] == 1) {
                unsigned value, value2 = 0;
                if (NeighborsState::unoccupied == checkIfNewCluster(lattice, i, j, lattice.size()-1, value, value2)) {
                    // cout << "unoccupied value: " << value << "  value2: " << value2 << endl;
                    lattice[i][j] = k;
                    ++M[k];
                    ++k;
                }
                else if (NeighborsState::bothOccupied == checkIfNewCluster(lattice, i, j, lattice.size()-1, value, value2)) {
                    // cout << "bothOccupied value: " << value << "  value2: " << value2 << endl;
                    lattice[i][j] = value;
                    M[value] += M[value2] + 1;
                    M[value2]  = -M[value];
                    for (auto i = 0; i < lattice.size(); ++i) {
                        for (auto j = 0; j < lattice.size(); ++j) {
                            if (lattice[i][j] == value2)
                                lattice[i][j] = value;
                        }
                    }
                }
                else if (NeighborsState::aboveOccupied == checkIfNewCluster(lattice, i, j, lattice.size()-1, value, value2)) {
                    // cout << "aboveOccupied value: " << value << "  value2: " << value2 << endl;
                    lattice[i][j] = value;
                    ++M[value];
                    // for (auto i = 0; i < lattice.size(); ++i) {
                    //     for (auto j = 0; j < lattice.size(); ++j) {

                    //     }
                    // }
                }
                else if (NeighborsState::leftOccupied == checkIfNewCluster(lattice, i, j, lattice.size()-1, value, value2)) {
                    // cout << "lefOccupied value: " << value << "  value2: " << value2 << endl;
                    lattice[i][j] = value2;
                    ++M[value2];
                }
                //else // cout << "ERR" << endl;
            }
        }
    }
    
    // for (auto k = 2; k < M.size()+2; ++k) {
    //     if (M[k] > 0)
    //         cout << "M[" << k << "] = " << M[k] << endl;
    // }

    map<unsigned int,int>::iterator max_cluster
        = max_element(M.begin(),M.end(),
        [] (const std::pair<unsigned int,int>& a, const std::pair<unsigned int,int>& b)->bool{ return a.second < b.second; } );

    // cout << "Returned max cluster = " << max_cluster->second << endl;
    distribution = M;
    return max_cluster->second;
}

map<int, int> cummulativeClusterDistribution(const map<unsigned int, int>& M, bool clear)
{
    map<int, int> distr;
    for (const auto& cluster : M)
        if (cluster.second > 0)
            ++distr[cluster.second];

    static map<int, int> averageDistribution;

    if (clear)
        averageDistribution.clear();

    for (const auto& cluster : distr)
        averageDistribution[cluster.first] += cluster.second;

    // for (const auto& cluster : averageDistribution) {
    //     cout << "s:" << cluster.first << " ns:" << cluster.second << endl;
    // }

    return averageDistribution;
}

map<int, double> normalizedClusterDistributionData(const map<int, int>& distr)
{
    map<int, double> normalizedDistribution;
    int sumOfAllClusters = 0;
    for (const auto& cluster : distr)
        sumOfAllClusters += cluster.second;

    const double factor = (double)1/sumOfAllClusters;
    // cout << "sum=" << sumOfAllClusters << endl;
    // cout << "factor=" << factor << endl;

    for (const auto& cluster : distr) {
        normalizedDistribution[cluster.first] = cluster.second*factor;
    }

    // for (const auto& cluster : normalizedDistribution) {
    //     cout << "s:" << cluster.first << " ns:" << cluster.second << endl;
    // }

    return normalizedDistribution;
}

