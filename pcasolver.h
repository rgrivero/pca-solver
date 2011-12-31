#ifndef PCA_PCA_SOLVER
#define PCA_PCA_SOLVER

#include "Eigen/Dense"
#include "sortvector.h"

class PcaSolver {
public:
    PcaSolver() : mPcaPerformed(false)
    {}

    MatrixXf performPcaOnData(const MatrixXf& data);

    MatrixXf const getReducedData()
    {
        // TODO exception if not mPcaPerformed
        return mReducedData;
    }

    MatrixXf const getPrincipalComponentsMatrix()
    {
        // TODO exception if not mPcaPerformed
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
