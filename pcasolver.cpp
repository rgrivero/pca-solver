#include "pcasolver.h"
#include <iostream>

using namespace std;

MatrixXf PcaSolver::performPcaOnData(const MatrixXf& data)
{
    calculateEigen(data);
    performPcaUsingEigenvectors();
    return mReducedData;
}

MatrixXf PcaSolver::absMatrix(const MatrixXf& data)
{
    MatrixXf absData(data.rows(), data.cols());
    for (int i = 0; i < data.rows(); i++) {
        for (int j = 0; j < data.cols(); j++) {
            absData(i, j) = abs(data(i, j));
        }
    }
    return absData;
}

void PcaSolver::calculateEigen(const MatrixXf& data)
{
    mOriginalData = data;
    if (mDebug) cout << "original data:\n" << mOriginalData << endl;
    
    MatrixXf normalizedData = normalizeRows(data);
    if (mDebug) cout << "normalized data:\n" << normalizedData << endl;
    
    MatrixXf covarianceMatrix = calculateCovarianceMatrix(normalizedData);
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

void PcaSolver::setMinAbsCovariance(double value)
{
    mMinAbsCovariance = abs(value);
}

MatrixXf PcaSolver::performPcaUsingEigenvectors()
{
    if (!mEigenCalculated) throw EigenNotCalculated();
    
    VectorXf absEigenvalues = absMatrix(mEigenvalues);
    if (mDebug) cout << "abs eigenvalues:\n" << absEigenvalues << endl;

    mPrincipalComponentsMatrix = 
        sortEigenvectorsAccordingToEigenvalues(mEigenvectors, absEigenvalues, mMinAbsCovariance);
    if (mDebug) cout << "pc:\n" << mPrincipalComponentsMatrix << endl;
    if (mDebug) cout << "pc':\n" << mPrincipalComponentsMatrix.transpose() << endl;

    mReducedData = mPrincipalComponentsMatrix.transpose() * mOriginalData;
    mPcaPerformed = true;
    return mReducedData;
}

MatrixXf PcaSolver::normalizeRows(const MatrixXf &m)
{
    MatrixXf result(m.rows(), m.cols());
    VectorXf zeroMeanRow(m.cols());

    for (int i = 0; i < m.rows(); i++) {
        double rowMean = m.row(i).mean();
        for (int j = 0; j < m.cols(); j++) {
            zeroMeanRow(j) = m(i, j) - rowMean;
        }
        double rowNorm = zeroMeanRow.norm();
        result.row(i) = zeroMeanRow / rowNorm;
    }
    return result;
}

MatrixXf PcaSolver::calculateCovarianceMatrix(const MatrixXf &m)
{
    return m * m.transpose();
}

MatrixXf PcaSolver::sortEigenvectorsAccordingToEigenvalues(
    const MatrixXf &eigenvectors, const VectorXf &eigenvalues,
    double minAbsCorrelation)
{
    SortVector sortVector(eigenvalues, SortVector::descend);
    VectorXi sortedIndices = sortVector.getSortedIndices();
    VectorXf sortedValues = sortVector.getSortedVector();
    
    std::vector<int> validIndices;
    for (int i = 0; i < sortedIndices.size(); i++) {
        if (sortedValues(i) < minAbsCorrelation) {
            break;
        } else {
            validIndices.push_back(sortedIndices(i));
        }
    }

    MatrixXf sortedValidVectors(eigenvectors.rows(), validIndices.size());
    for (unsigned int i = 0; i < validIndices.size(); i++) {
        int index = validIndices[i];
        sortedValidVectors.col(i) = eigenvectors.col(index);
    }
    return sortedValidVectors;
}

