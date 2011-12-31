#include <iostream>
#include <stdio.h>
#include "Eigen/Dense"
#include "pcasolver.h"

using namespace std;
using namespace Eigen;

int main(int argc, char* argv[])
{
    MatrixXf data(4, 4);
    data << 1, 2, 3, 4,
            17, 120, 32, -7,
            2, 4, 6, 8,
            3, 6, 9, 12;
    PcaSolver pcaSolver;
    MatrixXf reducedData = pcaSolver.performPcaOnData(data);
    cout << "reduced data: \n" << reducedData << endl;

    return 0;
}

