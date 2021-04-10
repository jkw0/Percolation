#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <numeric>
using namespace std;

void burnCellsWithChecks(vector<vector<unsigned int>>& lattice, unsigned i, unsigned j, unsigned max, unsigned t)
{
    //NOT ON EDGE
    if (i != 0 && j != 0 && i != max && j != max) {
        // // cout << "i=" << i << " j=" << j << endl;
        // // cout << "i != 0 && j != 0 && i != max && j != max" << endl;
        if (lattice[i-1][j] == 1) lattice[i-1][j] = t;
        if (lattice[i+1][j] == 1) lattice[i+1][j] = t;
        if (lattice[i][j-1] == 1) lattice[i][j-1] = t;
        if (lattice[i][j+1] == 1) lattice[i][j+1] = t;
    }
    else {
        if (i == 0 && j == 0) {
            // // cout << "i=" << i << " j=" << j << endl;
            // // cout << "i == 0 && j == 0" << endl;
            if (lattice[i+1][j] == 1) lattice[i+1][j] = t;
            if (lattice[i][j+1] == 1) lattice[i][j+1] = t; 
        }
        else if (i == max && j == max) {
            // // cout << "i=" << i << " j=" << j << endl;
            // // cout << "i == max && j == max" << endl;
            if (lattice[i-1][j] == 1) lattice[i-1][j] = t;
            if (lattice[i][j-1] == 1) lattice[i][j-1] = t;
        }
        else if (i == 0 && j == max) {
            // // cout << "i=" << i << " j=" << j << endl;
            // // cout << "i == 0 && j == max" << endl;
            if (lattice[i+1][j] == 1) lattice[i+1][j] = t;
            if (lattice[i][j-1] == 1) lattice[i][j-1] = t;
        }
        else if (i == max && j == 0) {
            // // cout << "i=" << i << " j=" << j << endl;
            // // cout << "i == max && j == 0" << endl;
            if (lattice[i-1][j] == 1) lattice[i-1][j] = t;
            if (lattice[i][j+1] == 1) lattice[i][j+1] = t; 
        }
        else {
            if (i == 0) {
                // // cout << "i=" << i << " j=" << j << endl;
                // // cout << "i == 0" << endl;
                if (lattice[i+1][j] == 1) lattice[i+1][j] = t;
                if (lattice[i][j-1] == 1) lattice[i][j-1] = t;
                if (lattice[i][j+1] == 1) lattice[i][j+1] = t; 
            }
            else if (j == 0) {
                // // cout << "i=" << i << " j=" << j << endl;
                // // cout << "j == 0" << endl;
                if (lattice[i-1][j] == 1) lattice[i-1][j] = t;
                if (lattice[i+1][j] == 1) lattice[i+1][j] = t;
                if (lattice[i][j+1] == 1) lattice[i][j+1] = t; 
            }
            else if (i == max) {
                // // cout << "i=" << i << " j=" << j << endl;
                // // cout << "i == max" << endl;
                if (lattice[i-1][j] == 1) lattice[i-1][j] = t;
                if (lattice[i][j-1] == 1) lattice[i][j-1] = t;
                if (lattice[i][j+1] == 1) lattice[i][j+1] = t; 
            }
            else if (j == max) {
                // // cout << "i=" << i << " j=" << j << endl;
                // // cout << "j == max" << endl;
                if (lattice[i-1][j] == 1) lattice[i-1][j] = t;
                if (lattice[i+1][j] == 1) lattice[i+1][j] = t;
                if (lattice[i][j-1] == 1) lattice[i][j-1] = t;
            }
        }
    }
}

bool burningMethod(vector<vector<unsigned int>>& lattice)
{
    // find the last occupied element
    std::pair<int,int> endIdx;
    bool foundEndPath = false;
    for (int i = (lattice.size()-1); i >= 0; --i) {
        //for (int j = (lattice.size()-1); j >= 0; --j) {
            if (lattice[lattice.size()-1][i] == 1) {
                endIdx.first = i;
                //endIdx.second = j;
                foundEndPath = true;
                break;
            }
        //}
        //if (leave) break;
    }
    if (foundEndPath == false)
        return false;

    // // cout << "endIdxfirst=" << endIdx.first << ", endIdxsecond=" << endIdx.second << endl;
    // // cout << endl;
    for (auto i = 0; i < lattice.size(); ++i)
        if (lattice[0][i] == 1)
            lattice[0][i] = 2;

    //unsigned int rows = 0;
    unsigned int t = 2;
    bool stop = false;
    while(stop == false) {
        // // cout << endl;
        // // cout << "t=" << t << endl;
        // // cout << "rows=" << rows << endl;
        std::vector<std::pair<int, int>> cells;
        cells.resize(0);
        for (auto i = 0; i < lattice.size(); ++i)
            for (auto j = 0; j < lattice.size(); ++j)
                if (lattice[i][j] == t)
                {
                    // // cout << "OK : i=" << i << ", j=" << j << " lattice=" << lattice[i][j] << " t=" << t << endl;
                    cells.push_back(std::pair<int,int>(i,j));
                }

        if (cells.empty())
        {
            //cout << "no connection" << endl;
            return false;
        }

        for (std::pair<int, int> idx : cells)
        {
            // // cout << "Burning : i=" << idx.first << ", j=" << idx.second << endl;
            burnCellsWithChecks(lattice, idx.first, idx.second, lattice.size()-1, t+1);
            for (int i = 0; i < lattice.size(); ++i) {
                //if (1 != lattice[endIdx.first][i] && 0 != lattice[endIdx.first][i])
                if ((t+1) == lattice[lattice.size()-1][i])
                {
                    // // cout << "endIdx.first=" << endIdx.first << ", i=" << i << " lattice=" << lattice[endIdx.first][i] << endl;
                    //cout << "END END END" << endl;
                    stop = true;
                }
            }
        }
        // // cout << "stop=" << (stop ? "true\n" : "false\n");
        //++rows;
        ++t;
    }
    if (stop == true) return true;
    // cout << "sometning went wrong" << endl;
    return false;
}