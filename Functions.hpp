#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <numeric>
using namespace std;

void printLattice(const vector<vector<unsigned int>>& lattice)
{
    for (auto i = 0; i < lattice.size(); ++i) {
        for (auto j = 0; j < lattice.size(); ++j) {
            cout << lattice[i][j] << "\t";
        }
        cout << endl;
    }
    cout << endl;
}

vector<vector<unsigned int>> initLattice(vector<vector<unsigned int>>& lattice, double p
    , mt19937& gen, uniform_real_distribution<double>& dis)
{
    for (auto i = 0; i < lattice.size(); ++i) {
        for (auto j = 0; j < lattice.size(); ++j) {
            auto R = dis(gen);
            if (R < p) lattice[i][j] = 1;
            else lattice[i][j] = 0;
        }
    }
    return lattice;
}

double averageValue(const vector<int>& data)
{
    return accumulate(data.begin(), data.end(), 0.0)/data.size();
}

void saveToFileAve(const string& L, const string& T, const double p, const double Pflow, const unsigned int smax)
{
    string name = "Ave_L" + L + "T" + T + ".txt"; 
    ofstream file;
    file.open(name, ios::out | ios::app);

    file << fixed << setprecision(1) << p << "  " << Pflow << "  " << smax << "\n";

    file.close();
}

string toStringWithPrecision(const double value)
{
    std::ostringstream out;
    out.precision(1);
    out << fixed << value;
    return out.str();
}

void saveToFileDist(const string& L, const string& T, const double p, const map<int, double>& distr)
{
    string name = "Dist_p" + toStringWithPrecision(p) + "L" + L + "T" + T + ".txt"; 
    ofstream file;
    file.open(name, ios::out | ios::app);

    for (const auto& cluster : distr) {
        file << fixed << setprecision(2) << cluster.first << "  " << cluster.second << "\n";
    }

    file.close();
}

map<string, string> parseInput()
{
    map<string, string> inputData;

    ifstream file;
    file.open("perc_ini.txt", ifstream::in);
    string L, T, p0, pk, dp;
    file >> L >> T >> p0 >> pk >> dp;
    inputData["L"] = L;
    inputData["T"] = T;
    inputData["p0"] = p0;
    inputData["pk"] = pk;
    inputData["dp"] = dp;

    return inputData;
}

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