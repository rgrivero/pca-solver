#include "sortvector.h"

SortVector::SortVector(const VectorXf& vector, const SortType& type)
{
    mSize = vector.size();
    for (int i = 0; i < mSize; i++) {
        mStdVector.push_back(ElementWithIndex(vector(i), i));
    }
    if (type == ascend) {
        sort(mStdVector.begin(), mStdVector.end(), ascendComparator);
    } else {
        sort(mStdVector.begin(), mStdVector.end(), descendComparator);
    }
    mIndices = VectorXi(mSize);
    mVector = VectorXf(mSize);
    for (int i = 0; i < mSize; i++) {
        mVector(i) = mStdVector[i].first;
        mIndices(i) = mStdVector[i].second;
    }
}

bool SortVector::descendComparator(const ElementWithIndex& a,
    const ElementWithIndex& b)
{
    if (a.first > b.first) {
        return true;
    } else {
        return false;
    }
}

bool SortVector::ascendComparator(const ElementWithIndex& a,
    const ElementWithIndex& b)
{
    if (a.first < b.first) {
        return true;
    } else {
        return false;
    }
}

