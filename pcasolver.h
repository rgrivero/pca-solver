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
        mEigenCalculated(false), mPcaPerformed(false), mMinAbsCovariance(0)
    {}

    /** Performs PCA on provided data.
     * @param data matrix of input data, rows = features
     * @return reduced dimensions data
     */
    MatrixXf performPcaOnData(const MatrixXf& data);

    /** Calulates eigenvectors and eigenvalues,
     * stores given data in class. 
     * Should be followed by performPcaUsingEigenvectors().
     * @param data matrix of input data, rows = features
     */ 
    void calculateEigen(const MatrixXf& data);
  
    /** Sets min absolute value of covariance for
     * a feature to be selected in PCA.
     * Features with smaller abs eigenvalue will be
     * ommited by performPcaUsingEigenVectors().
     * Should be proceeded by calculateEigen().
     * @param value min absolute value of covariance
     */
    void setMinAbsCovariance(double value);

    /** Second step of PCA analysis, takes into account
     * min absolute value of covariance, if set.
     * Must be proceeded by calculateEigen().
     * @return reduced dimensions data
     */
    MatrixXf performPcaUsingEigenvectors();
    
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

    VectorXf const getAbsEigenvalues()
    {
        if (!mEigenCalculated) throw EigenNotCalculated();
        return absMatrix(mEigenvalues);
    }

protected:
    static MatrixXf normalizeRows(const MatrixXf &m);
    
    static MatrixXf absMatrix(const MatrixXf& data);

    static MatrixXf calculateCovarianceMatrix(const MatrixXf &m);

    static MatrixXf sortEigenvectorsAccordingToEigenvalues(
        const MatrixXf &eigenvectors, const VectorXf &eigenvalues,
        double minAbsCorrelation);

protected:
        bool mDebug;
        bool mEigenCalculated;
        bool mPcaPerformed;
        double mMinAbsCovariance;
        MatrixXf mOriginalData;
        MatrixXf mEigenvectors;
        VectorXf mEigenvalues;
        MatrixXf mPrincipalComponentsMatrix;
        MatrixXf mReducedData;
};

#endif
