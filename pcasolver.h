#ifndef PCA_PCA_SOLVER
#define PCA_PCA_SOLVER

#include "Eigen/Dense"
#include "sortvector.h"
#include <stdexcept>

class PcaNotPerformedYet : public std::logic_error { 
public:
	PcaNotPerformedYet() : std::logic_error("PCA was not performed yet. Invoke performPcaOnData() first.")
	{}
};

class PcaSolver {
public:
    PcaSolver() : mPcaPerformed(false)
    {}

    MatrixXf performPcaOnData(const MatrixXf& data);

    MatrixXf const getReducedData()
    {
        if (!mPcaPerformed) throw PcaNotPerformedYet();
        return mReducedData;
    }

    MatrixXf const getPrincipalComponentsMatrix()
    {
        if (!mPcaPerformed) throw PcaNotPerformedYet();
        return mPrincipalComponentsMatrix;
    }

protected:
    static MatrixXf subtractMeanFromRows(const MatrixXf &m);

    static MatrixXf calculateCovarianceMatrix(const MatrixXf &m);

    static MatrixXf sortEigenvectorsAccordingToEigenvalues(
        const MatrixXf &eigenvectors, const VectorXf &eigenvalues);

protected:
        bool mPcaPerformed;
        MatrixXf mPrincipalComponentsMatrix;
        MatrixXf mReducedData;
};

#endif
