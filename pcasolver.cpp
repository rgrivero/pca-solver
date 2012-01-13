#include "pcasolver.h"
#include <iostream>

using namespace std;

MatrixXf PcaSolver::performPcaOnData(const MatrixXf& data)
{
    calculateEigen(data);
    performPcaUsingEigenvectors();
    return mReducedData;
}

void PcaSolver::calculateEigen(const MatrixXf& data)
{
    if (mDebug) cout << "original data:\n" << data << endl;
    MatrixXf zeroMeanData = subtractMeanFromRows(data);
    mOriginalData = data;
    if (mDebug) cout << "zero mean data:\n" << zeroMeanData << endl;
    MatrixXf covarianceMatrix =
        calculateCovarianceMatrix(zeroMeanData);
    if (mDebug) cout << "covariance:\n" << covarianceMatrix << endl;
    SelfAdjointEigenSolver<MatrixXf> eigensolver(covarianceMatrix);
    if (eigensolver.info() != Success) {
        abort();
    }
    mEigenvalues = eigensolver.eigenvalues();
    mEigenvectors = eigensolver.eigenvectors();
    if (mDebug) cout << "eigenvalues:\n" << mEigenvalues << endl;
    if (mDebug) cout << "eigenvectors:\n" << mEigenvectors << endl;
    mEigenCalculated = true;
}

void PcaSolver::setMinAbsCorrelation(double value)
{
    mMinAbsCorrelation = abs(value);
}

MatrixXf PcaSolver::performPcaUsingEigenvectors()
{
    if (!mEigenCalculated) throw EigenNotCalculated();
    mPrincipalComponentsMatrix = 
        sortEigenvectorsAccordingToEigenvalues(mEigenvectors, mEigenvalues, mMinAbsCorrelation);
    if (mDebug) cout << "pc:\n" << mPrincipalComponentsMatrix << endl;
    if (mDebug) cout << "pc':\n" << mPrincipalComponentsMatrix.transpose() <<endl;
    mReducedData = mPrincipalComponentsMatrix.transpose() * mOriginalData;
    mPcaPerformed = true;
    return mReducedData;
}

MatrixXf PcaSolver::subtractMeanFromRows(const MatrixXf &m)
{
    MatrixXf result(m.rows(), m.cols());
    for (int i = 0; i < m.rows(); i++) {
        double rowMean = m.row(i).mean();
        for (int j = 0; j < m.cols(); j++) {
            result(i, j) = m(i, j) - rowMean;
        }
    }
    return result;
}

MatrixXf PcaSolver::calculateCovarianceMatrix(const MatrixXf &m)
{
    MatrixXf result = (m * m.transpose());
    int samplesNumber = m.cols();
    for (int i = 0; i < result.rows(); i++) {
        for (int j = 0; j < result.cols(); j++) {
            result(i, j) = result(i, j) / (samplesNumber - 1);
        }
    }
    return result;
}

MatrixXf PcaSolver::sortEigenvectorsAccordingToEigenvalues(
    const MatrixXf &eigenvectors, const VectorXf &eigenvalues,
    double minAbsCorrelation)
{
    SortVector sortVector(eigenvalues, SortVector::descend);
    VectorXi sortedIndices = sortVector.getSortedIndices();
    VectorXf sortedValues = sortVector.getSortedVector();
    MatrixXf sortedVectors(eigenvectors.rows(), eigenvectors.cols());
    MatrixXf zerocol(eigenvectors.rows(), 1);
    for (int i = 0; i < eigenvectors.rows(); i++) {
        zerocol(i, 0) = 0;
    }
    for (int i = 0; i < sortedIndices.size(); i++) {
        int index = sortedIndices(i);
        if (sortedValues(i) >= minAbsCorrelation) {
            sortedVectors.col(i) = eigenvectors.col(index);
        } else {
            sortedVectors.col(i) = zerocol;
        }
    }
    return sortedVectors;
}

