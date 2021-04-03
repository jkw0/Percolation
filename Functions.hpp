#include <iostream>
#include <array>
#include <map>
#include <random>
using namespace std;

/* GLOBALS*/
const unsigned int L = 5;
double p = 0.7;
array<array<unsigned int,L>,L> Lattice = {0};
//**********//

void printLattice(const array<array<unsigned int,L>,L>& lattice)
{
    for (auto i = 0; i < lattice.size(); ++i) {
        for (auto j = 0; j < lattice.size(); ++j) {
            cout << lattice[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

double getRandom()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(0, 1);
    return dis(gen);
}

array<array<unsigned int,L>,L> initLattice()
{
    for (auto i = 0; i < Lattice.size(); ++i) {
        for (auto j = 0; j < Lattice.size(); ++j) {
            auto R = getRandom();
            if (R < p) Lattice[i][j] = 1;
            else Lattice[i][j] = 0;
        }
    }
    return Lattice;
}

void burnCellsWithChecks(array<array<unsigned int,L>,L>& lattice, unsigned i, unsigned j, unsigned max, unsigned t)
{
    if (i != 0 && j != 0 && i != max && j != max) {
        cout << "i=" << i << " j=" << j << endl;
        cout << "i != 0 && j != 0 && i != max && j != max" << endl;
        if (lattice[i-1][j] == 1) lattice[i-1][j] = t;
        if (lattice[i+1][j] == 1) lattice[i+1][j] = t;
        if (lattice[i][j-1] == 1) lattice[i][j-1] = t;
        if (lattice[i][j+1] == 1) lattice[i][j+1] = t;
    }
    else {
        if (i == 0 && j == 0) {
            cout << "i=" << i << " j=" << j << endl;
            cout << "i == 0 && j == 0" << endl;
            if (lattice[i+1][j] == 1) lattice[i+1][j] = t;
            if (lattice[i][j+1] == 1) lattice[i][j+1] = t; 
        }
        else if (i == max && j == max) {
            cout << "i=" << i << " j=" << j << endl;
            cout << "i == max && j == max" << endl;
            if (lattice[i-1][j] == 1) lattice[i-1][j] = t;
            if (lattice[i][j-1] == 1) lattice[i][j-1] = t;
        }
        else if (i == 0 && j == max) {
            cout << "i=" << i << " j=" << j << endl;
            cout << "i == 0 && j == max" << endl;
            if (lattice[i+1][j] == 1) lattice[i+1][j] = t;
            if (lattice[i][j-1] == 1) lattice[i][j-1] = t;
        }
        else if (i == max && j == 0) {
            cout << "i=" << i << " j=" << j << endl;
            cout << "i == max && j == 0" << endl;
            if (lattice[i-1][j] == 1) lattice[i-1][j] = t;
            if (lattice[i][j+1] == 1) lattice[i][j+1] = t; 
        }
        else {
            if (i == 0) {
                cout << "i=" << i << " j=" << j << endl;
                cout << "i == 0" << endl;
                if (lattice[i+1][j] == 1) lattice[i+1][j] = t;
                if (lattice[i][j-1] == 1) lattice[i][j-1] = t;
                if (lattice[i][j+1] == 1) lattice[i][j+1] = t; 
            }
            else if (j == 0) {
                cout << "i=" << i << " j=" << j << endl;
                cout << "j == 0" << endl;
                if (lattice[i-1][j] == 1) lattice[i-1][j] = t;
                if (lattice[i+1][j] == 1) lattice[i+1][j] = t;
                if (lattice[i][j+1] == 1) lattice[i][j+1] = t; 
            }
            else if (i == max) {
                cout << "i=" << i << " j=" << j << endl;
                cout << "i == max" << endl;
                if (lattice[i-1][j] == 1) lattice[i-1][j] = t;
                if (lattice[i][j-1] == 1) lattice[i][j-1] = t;
                if (lattice[i][j+1] == 1) lattice[i][j+1] = t; 
            }
            else if (j == max) {
                cout << "i=" << i << " j=" << j << endl;
                cout << "j == max" << endl;
                if (lattice[i-1][j] == 1) lattice[i-1][j] = t;
                if (lattice[i+1][j] == 1) lattice[i+1][j] = t;
                if (lattice[i][j-1] == 1) lattice[i][j-1] = t;
            }
        }
    }
}

void burningMethod(array<array<unsigned int,L>,L>& lattice)
{
    for (auto i = 0; i < lattice.size(); ++i)
        if (lattice[0][i] == 1)
            lattice[0][i] = 2;

    unsigned int rows = 0;
    unsigned int t = 2;
    do {
        std::vector<std::pair<int, int>> cells;
        cells.resize(0);
        for (auto i = 0; i < lattice.size(); ++i)
            for (auto j = 0; j < lattice.size(); ++j)
                if (lattice[i][j] == t)
                    cells.push_back(std::pair<int,int>(i,j));

        for (std::pair<int, int> idx : cells)
            burnCellsWithChecks(lattice, idx.first, idx.second, lattice.size()-1, t+1);
            
        ++rows;
        ++t;
    } while (rows != lattice.size());
}

enum class NeighborsState {
    unoccupied = 0,
    aboveOccupied,
    leftOccupied,
    bothOccupied,
    other
};

NeighborsState checkIfNewCluster(const array<array<unsigned int,L>,L>& lattice
    , const unsigned i, const unsigned j
    , const unsigned max, unsigned& val, unsigned& val2)
{

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
            return NeighborsState::bothOccupied;
        }
        else if (isAboveOccupied && !isLeftOccupied) {
            return NeighborsState::aboveOccupied;
        }
        else if (!isAboveOccupied && isLeftOccupied) {
             return NeighborsState::leftOccupied;
        }
    }
    else {
        if (i == 0 && j == 0) return NeighborsState::unoccupied;
        else if (i == 0) {
            if (lattice[i][j-1] == 0) return NeighborsState::unoccupied;
            else if (lattice[i][j-1] != 0 && lattice[i][j-1] != 1) {
                val = lattice[i][j-1];
                return NeighborsState::leftOccupied;
            }
        }
        else if (j == 0) {
            if (lattice[i-1][j] == 0) return NeighborsState::unoccupied;
            else if (lattice[i-1][j] != 0 && lattice[i-1][j] != 1) {
                val = lattice[i-1][j];
                return NeighborsState::aboveOccupied;
            }
        }
    }
    
    return NeighborsState::other;
}

void clusterDistribution(array<array<unsigned int,L>,L>& lattice)
{
    map<unsigned int, int> M;
    unsigned int k = 2;

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
                unsigned value, value2;
                if (NeighborsState::unoccupied == checkIfNewCluster(lattice, i, j, lattice.size()-1, value, value2)) {
                    lattice[i][j] = k;
                    ++M[k];
                    ++k;
                }
                else if (NeighborsState::bothOccupied == checkIfNewCluster(lattice, i, j, lattice.size()-1, value, value2)) {
                    lattice[i][j] = value;
                    cout << "i=" << i << " j=" << j << endl;
                    cout << "value=" << value << endl;
                    cout << "value2=" << value2 << endl;
                    cout << "before M[value]=" << M[value] << endl;
                    cout << "before M[value2]=" << M[value2] << endl;
                    M[value] += M[value2] + 1;
                    M[value2]  = -M[value];
                    cout << "after M[value]=" << M[value] << endl;
                    cout << "after M[value2]=" << M[value2] << endl;
                    cout << endl;
                    for (auto i = 0; i < lattice.size(); ++i) {
                        for (auto j = 0; j < lattice.size(); ++j) {
                            if (lattice[i][j] == value2)
                                lattice[i][j] = value;
                        }
                    }
                }
                else if (NeighborsState::aboveOccupied == checkIfNewCluster(lattice, i, j, lattice.size()-1, value, value2)
                    || NeighborsState::leftOccupied == checkIfNewCluster(lattice, i, j, lattice.size()-1, value, value2)) {
                    lattice[i][j] = value;
                    ++M[value];
                }
                else cout << "ERR" << endl;
            }
        }
    }
    
    for (auto k = 2; k < M.size()+2; ++k) {
        if (M[k] > 0)
            cout << "M[" << k << "] = " << M[k] << endl;
    }
}



