#include "Eigen/Dense"
#include "pcasolver.h"
#include "sortvector.h"
#include <iostream>
#include <boost/test/minimal.hpp>

using namespace Eigen;

class PcaSolver_Test : public PcaSolver {
public:
	static void subtractMeanFromRows_test()
	{
		MatrixXf m(3, 4);
		m << 1, 2, 3, 4,
			 5, 6, 7, 8,
		     -9, -10, -11, -12;
		     
		MatrixXf m_exp(3, 4);
		m << -1.5, -0.5, 0.5, 1.5,
			 -1.5, -0.5, 0.5, 1.5,
			 1.5, 0.5, -0.5, -1.5;
			 
		MatrixXf m_res = PcaSolver::subtractMeanFromRows(m);
		std::cout << m_res;
			 
		BOOST_CHECK(m_res == m_exp);	
		// TODO (no compare operator...?)
	}
	
	static void calculateCovarianceMatrix_test()
	{
		// TODO	
	}
	
	static void sortEigenvectorsAccordingToEigenvalues_test()
	{
		// TODO
	}
	
	static void total_test()
	{
		subtractMeanFromRows_test();
		calculateCovarianceMatrix_test();
		sortEigenvectorsAccordingToEigenvalues_test();
	}
};

class SortVector_Test : public SortVector {
public:
	SortVector_Test(const VectorXf& vector, const SortType& type = descend) : SortVector(vector)
	{}
	
	void total_test()
	{
		// TODO	
	}
};

int test_main(int argc, char* argv[])
{
	PcaSolver_Test pcaTest;
	pcaTest.total_test();
	
	SortVector_Test sortVectorTest(VectorXf::Random(20), SortVector::descend);
	sortVectorTest.total_test();
	
	return 0;
}