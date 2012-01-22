#include "Eigen/Dense"
#include "pcasolver.h"
#include "sortvector.h"
#include <iostream>
#include <boost/test/minimal.hpp>

using namespace Eigen;

/** Class for comparing
  * matrices and vertices.
  */
class Comparator {
public:
    static bool matricesCompare(MatrixXf m1, MatrixXf m2) {
        for (int i = 0; i < m1.rows(); ++i) {
            for (int j = 0; j < m1.cols(); ++j) {
                if (fabs(m1(i, j) - m2(i, j)) > 0.0001)	return false;
            }
        }
        return true;
    }
	
    static bool verticesCompare(VectorXf v1, VectorXf v2) {
        if (v1.size() != v2.size()) return false;
        for (int i = 0; i < v1.size(); ++i) {
            if (fabs(v1(i) - v2(i)) > 0.0001) return false;
        }
        return true;
    }
	
};

/**
 * Unit tests for PcaSolver class.
 */
class PcaSolverTest : public PcaSolver {
public:
    static void normalizeRowsTest()
    {
        MatrixXf mInput(3, 3);
        mInput << 1, 2, 3,
            8, 4, 5,
            8, 10, 2;
		     
        MatrixXf mExpected(3, 3);
        mExpected << -0.707107, 0, 0.707107,
            0.792594, -0.566138, -0.226455,
            0.226455, 0.566139, -0.792594;
			 
        MatrixXf mResult = PcaSolver::normalizeRows(mInput);
			 
        BOOST_CHECK(Comparator::matricesCompare(mResult, mExpected));
    }
	
    static void absMatrixTest()
    {
        MatrixXf mInput(3, 4);
        mInput << -1, 2, 3, 4,
            5, 6, -7, 8,
            -9, -10, -11, -12;
		     
        MatrixXf mExpected(3, 4);
        mExpected << 1, 2, 3, 4,
            5, 6, 7, 8,
            9, 10, 11, 12;		
		     
        MatrixXf mResult = PcaSolver::absMatrix(mInput);		     
		     
        BOOST_CHECK(Comparator::matricesCompare(mResult, mExpected));		     
    }
	
    static void calculateCovarianceMatrixTest()
    {
        MatrixXf mInput(3, 3);
        mInput << 1, 2, 3,
            0, 3, 1,
            2, 0, 0;
		     
        MatrixXf mExpected(3, 3);
        mExpected << 14, 9, 2,
            9, 10, 0,
            2, 0, 4;	
		     
        MatrixXf mResult = PcaSolver::calculateCovarianceMatrix(mInput);		      
		     
        BOOST_CHECK(Comparator::matricesCompare(mResult, mExpected));				
    }
	
    static void sortEigenvectorsAccordingToEigenvaluesTest()
    {
        // 1, 2, 3
        // 1, 1, 1
        // 2, 1, 2
		
        MatrixXf eigenvectors(3, 3);
        eigenvectors << 0, 1, 0,
            1, 0, 0,
            0, 0, 1;
			
        VectorXf eigenvalues(3);
        eigenvalues << 0, 1, 1;
		
        double minAbsCorrelation = 0;
		
        MatrixXf mExpected(3, 3);
        mExpected << 1, 0, 0,
            0, 0, 1,
            0, 1, 0;		
		
        MatrixXf mResult = PcaSolver::sortEigenvectorsAccordingToEigenvalues(eigenvectors, eigenvalues, minAbsCorrelation);
		
        BOOST_CHECK(Comparator::matricesCompare(mResult, mExpected));		
	}
	
    static void totalTest()
    {
        normalizeRowsTest();
        absMatrixTest();
        calculateCovarianceMatrixTest();
        sortEigenvectorsAccordingToEigenvaluesTest();
    }
};

class SortVectorTest {
public:

    static void sortTest()
    {
        VectorXf vInput(10);
        vInput << 1, -2, 10, 15, 11, 12, 14, 13, 16, 9;
		
        VectorXf vExpectedDesc(10);
        vExpectedDesc << 16, 15, 14, 13, 12, 11, 10, 9, 1, -2;
        VectorXf vExpectedAsc(10);
        vExpectedAsc << -2, 1, 9, 10, 11, 12, 13, 14, 15, 16;		
		
        SortVector resultDesc(vInput, SortVector::descend);
        SortVector resultAsc(vInput, SortVector::ascend);		
		
        BOOST_CHECK(Comparator::verticesCompare(resultDesc.getSortedVector(), vExpectedDesc));
        BOOST_CHECK(Comparator::verticesCompare(resultAsc.getSortedVector(), vExpectedAsc));		
    }

    void totalTest()
    {
        sortTest();
    }
};

int test_main(int argc, char* argv[])
{
    PcaSolverTest pcaTest;
    pcaTest.totalTest();
	
    SortVectorTest sortVectorTest;
    sortVectorTest.totalTest();
	
    return 0;
}
