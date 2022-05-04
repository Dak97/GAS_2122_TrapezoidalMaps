#include "algoritms.h"

namespace algoritms
{
    void buildTrapMapDag(DAG& dag, TrapMap& trapMap, const cg3::Segment2d segment){
        // ho i nodi dei trapezoidi da modificare
         DagNode* trapezoidNode = dag.query(segment);

         // ho aggiunto 4 nuovi trapezoidi alla trapezoidal map
        std::vector<Trapezoid*> trapforDag = trapMap.addFourTrapezoids(segment,trapezoidNode);

        Trapezoid bb = *(Trapezoid*)trapezoidNode->getData().objj;
        int id = bb.getId();
         //aggiorno la dag con i nuovi trapezoidi
        dag.updateDag(trapforDag, trapezoidNode, segment);

        trapMap.deleteTrapezoidWithId(id);

    }

}
