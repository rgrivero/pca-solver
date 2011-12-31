#include "pcasolver.h"
#include <iostream>

using namespace std;

MatrixXf PcaSolver::performPcaOnData(const MatrixXf& data)
{
    cout << "original data:\n" << data << endl;
    MatrixXf zeroMeanData = subtractMeanFromRows(data);
    cout << "zero mean data:\n" << zeroMeanData << endl;
    MatrixXf covarianceMatrix =
        calculateCovarianceMatrix(zeroMeanData);
    SelfAdjointEigenSolver<MatrixXf> eigensolver(covarianceMatrix);
    if (eigensolver.info() != Success) {
        abort();
    }
    VectorXf eigenvalues = eigensolver.eigenvalues();
    MatrixXf eigenvectors = eigensolver.eigenvectors();
    cout << "eigenvalues:\n" << eigenvalues << endl;
    cout << "eigenvectors:\n" << eigenvectors << endl;
    mPrincipalComponentsMatrix = 
        sortEigenvectorsAccordingToEigenvalues(eigenvectors, eigenvalues);
    //mPrincipalComponentsMatrix = eigenvectors;
    cout << "pc:\n" << mPrincipalComponentsMatrix << endl;
    cout << "pc':\n" << mPrincipalComponentsMatrix.transpose() <<endl;
    mReducedData = mPrincipalComponentsMatrix.transpose() * data;        
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
    for (int i = 0; i < result.rows(); i++)
        for (int j = 0; j < result.cols(); j++)
            result(i, j) = result(i, j) / (samplesNumber - 1);
    return result;
}

MatrixXf PcaSolver::sortEigenvectorsAccordingToEigenvalues(
    const MatrixXf &eigenvectors, const VectorXf &eigenvalues)
{
    SortVector sortVector(eigenvalues, SortVector::descend);
    VectorXi sortedIndices = sortVector.getSortedIndices();
    MatrixXf sortedVectors(eigenvectors.rows(), eigenvectors.cols());
    for (int i = 0; i < sortedIndices.size(); i++) {
        int index = sortedIndices(i);
        sortedVectors.col(i) = eigenvectors.col(index);
    }
    return sortedVectors;
}

