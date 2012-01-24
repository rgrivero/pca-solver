#ifndef PCA_SORT_VECTOR
#define PCA_SORT_VECTOR

#include <vector>
#include <utility>
#include <algorithm>
#include "Eigen/Dense"

using namespace Eigen;

/** Provides sorting for VectorXf type. Sorting is done in constructor.
  * Sorted vector and sorted original indices are available */
class SortVector {
public:
    enum SortType {ascend, descend};
public:
    /** Constructor sorts given vector, according to sorting rule.
      * @param vector vector to be sorted
      * @param type type of sort (ascend, descend)
      */
    SortVector(const VectorXf& vector, const SortType& type = descend);

    VectorXf const getSortedVector()
    {
        return mVector;
    }

    VectorXi const getSortedIndices()
    {
        return mIndices;
    }

protected:
    typedef std::pair<double, int> ElementWithIndex;

    static bool descendComparator(const ElementWithIndex& a, 
        const ElementWithIndex& b);
    
    static bool ascendComparator(const ElementWithIndex& a, 
        const ElementWithIndex& b);

protected:
    VectorXf mVector;
    VectorXi mIndices;

    int mSize;
    std::vector<ElementWithIndex> mStdVector;
};

#endif
