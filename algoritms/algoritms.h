#ifndef ALGORITMS_H
#define ALGORITMS_H

#include "data_structures/trapezoid.h"
#include "data_structures/dag.h"
#include <cg3/geometry/segment2.h>
#include "data_structures/dagnode.h"
namespace algoritms
{
    void buildTrapMapDag(DAG& dag, TrapMap& trapMap, const cg3::Segment2d segment);


};

#endif // ALGORITMS_H
