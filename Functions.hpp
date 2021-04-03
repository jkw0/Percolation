#include <iostream>
#include <array>
#include <map>
#include <random>
using namespace std;

/* GLOBALS*/
const unsigned int L = 5;
double p = 0.8;
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
        if (lattice[i-1][j] == 1) lattice[i-1][j] = t;
        if (lattice[i+1][j] == 1) lattice[i+1][j] = t;
        if (lattice[i][j-1] == 1) lattice[i][j-1] = t;
        if (lattice[i][j+1] == 1) lattice[i][j+1] = t;
    }
    else {
        if (i == 0 && j == 0) {
            if (lattice[i+1][j] == 1) lattice[i+1][j] = t;
            if (lattice[i][j+1] == 1) lattice[i][j+1] = t; 
        }
        else if (i == max && j == max) {
            if (lattice[i-1][j] == 1) lattice[i-1][j] = t;
            if (lattice[i][j-1] == 1) lattice[i][j-1] = t;
        }
        else if (i == 0 && j == max) {
            if (lattice[i+1][j] == 1) lattice[i+1][j] = t;
            if (lattice[i][j-1] == 1) lattice[i][j-1] = t;
        }
        else if (i == max && j == 0) {
            if (lattice[i-1][j] == 1) lattice[i-1][j] = t;
            if (lattice[i][j+1] == 1) lattice[i][j+1] = t; 
        }
        else {
            if (i == 0) {
                if (lattice[i+1][j] == 1) lattice[i+1][j] = t;
                if (lattice[i][j-1] == 1) lattice[i][j-1] = t;
                if (lattice[i][j+1] == 1) lattice[i][j+1] = t; 
            }
            else if (j == 0) {
                if (lattice[i-1][j] == 1) lattice[i-1][j] = t;
                if (lattice[i+1][j] == 1) lattice[i+1][j] = t;
                if (lattice[i][j+1] == 1) lattice[i][j+1] = t; 
            }
            else if (i == max) {
                if (lattice[i-1][j] == 1) lattice[i-1][j] = t;
                if (lattice[i][j-1] == 1) lattice[i][j-1] = t;
                if (lattice[i][j+1] == 1) lattice[i][j+1] = t; 
            }
            else if (j == max) {
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

    for (auto i = 0, t = 3; i < lattice.size(); ++i, ++t) {
        for (auto j = 0; j < lattice.size(); ++j) {
            if (lattice[i][j] == (t-1))
                burnCellsWithChecks(lattice, i, j, lattice.size()-1, t);
        }
    }
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
    , const unsigned max, unsigned& val)
{

    if (i != 0 && j != 0) {
        bool isAboveOccupied = false;
        bool isLeftOccupied = false;
        
        if ((lattice[i-1][j] == 0) && (lattice[i][j-1] == 0)) return NeighborsState::unoccupied;
        else if (lattice[i-1][j] != 0 && lattice[i-1][j] != 1) {
            val = lattice[i-1][j];
            isAboveOccupied = true;
        }
        else if (lattice[i][j-1] != 0 && lattice[i][j-1] != 1) {
            val = lattice[i][j-1];
            isLeftOccupied = true;
        }
        if (isAboveOccupied && isLeftOccupied) return NeighborsState::bothOccupied;
        else if (isAboveOccupied && !isLeftOccupied) return NeighborsState::aboveOccupied;
        else if (!isAboveOccupied && isLeftOccupied) return NeighborsState::leftOccupied;
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
    map<unsigned int, unsigned int> M;
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
                unsigned value;
                if (checkIfNewCluster(lattice, i, j, lattice.size()-1, value) == NeighborsState::unoccupied) {
                    lattice[i][j] = k;
                    ++M[k];
                    ++k;
                }
                else if ()
                else if (checkIfNewCluster(lattice, i, j, lattice.size()-1, value) == NeighborsState::aboveOccupied
                    || checkIfNewCluster(lattice, i, j, lattice.size()-1, value) == NeighborsState::leftOccupied) {
                    lattice[i][j] = value;
                    ++M[value];
                }
                else cout << "ERR" << endl;
            }
        }
    }
    
    for (auto k = 2; k < M.size()+2; ++k) {
        cout << "M[" << k << "] = " << M[k] << endl;
    }
}



