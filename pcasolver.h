#ifndef PCA_PCA_SOLVER
#define PCA_PCA_SOLVER

#include "Eigen/Dense"
#include "sortvector.h"
#include <stdexcept>
#include <string>

class PcaException : public std::logic_error {
public:
    PcaException(const std::string& msg) : std::logic_error(msg)
    {}
};

class PcaNotPerformedYet : public PcaException { 
public:
	PcaNotPerformedYet() : PcaException("PCA was not performed yet. Invoke performPcaOnData() first.")
	{}
};

class EigenNotCalculated : public PcaException {
public:
    EigenNotCalculated() : PcaException("Eigenvectors calculation was not performed yet. Invoke calculateEigen() first.")
    {}
};

class PcaSolver {
public:
    PcaSolver(bool printOutput = false) : mDebug(printOutput),
        mEigenCalculated(false), mPcaPerformed(false), mMinAbsCorrelation(0)
    {}

    MatrixXf performPcaOnData(const MatrixXf& data);

    void calculateEigen(const MatrixXf& data);
    
    MatrixXf performPcaUsingEigenvectors();
    
    void setMinAbsCorrelation(double value);

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

    VectorXf const getEigenvalues()
    {
        if (!mEigenCalculated) throw EigenNotCalculated();
        return mEigenvalues;
    }

protected:
    static MatrixXf subtractMeanFromRows(const MatrixXf &m);

    static MatrixXf calculateCovarianceMatrix(const MatrixXf &m);

    static MatrixXf sortEigenvectorsAccordingToEigenvalues(
        const MatrixXf &eigenvectors, const VectorXf &eigenvalues,
        double minAbsCorrelation);

protected:
        bool mDebug;
        bool mEigenCalculated;
        bool mPcaPerformed;
        double mMinAbsCorrelation;
        MatrixXf mOriginalData;
        MatrixXf mEigenvectors;
        VectorXf mEigenvalues;
        MatrixXf mPrincipalComponentsMatrix;
        MatrixXf mReducedData;
};

#endif
