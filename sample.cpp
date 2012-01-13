#include <iostream>
#include <assert.h>
#include "Eigen/Dense"
#include "pcasolver.h"

using namespace std;
using namespace Eigen;

int main(int argc, char* argv[])
{
	int n, m;
	cin >> n >> m;
	assert(n >= 1 && m >= 1);
	
    MatrixXf data(n, m);
    for (int i = 0; i < n; ++i) {
    	for (int j = 0; j < m; ++j) {
    		cin >> data(i, j);
    	}
    }
    
    PcaSolver pcaSolver(true);
    MatrixXf reducedData = pcaSolver.performPcaOnData(data);
    cout << "reduced data: \n" << reducedData << endl;

    return 0;
}
