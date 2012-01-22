#include <iostream>
#include <assert.h>
#include "Eigen/Dense"
#include "pcasolver.h"
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

using namespace std;
using namespace boost;
using namespace Eigen;

int main(int argc, char* argv[])
{

    vector<vector<string> > inputData;
    string line;
    vector<string> lineSplit;
    while(!getline(cin, line).eof()) {
        split(lineSplit, line, is_any_of(","));
        inputData.push_back(lineSplit);
    }
    
    int n = inputData.size();
    int m = inputData.at(0).size();
    MatrixXf data(n, m);
    
    for (unsigned int i = 0; i < inputData.size(); ++i) {
        lineSplit = inputData.at(i);
        for (unsigned int j = 0; j < lineSplit.size(); ++j) {
            data(i, j) = atof(lineSplit.at(j).c_str());
        }
    }
    
    PcaSolver pcaSolver(true);
    MatrixXf reducedData = pcaSolver.performPcaOnData(data);
    cout << "reduced data: \n" << reducedData << endl;
    
    return 0;
}
