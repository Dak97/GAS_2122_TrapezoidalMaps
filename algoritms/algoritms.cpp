#include "algoritms.h"

namespace algoritms
{
    void buildTrapMapDag(DAG& dag, TrapMap& trapMap, const cg3::Segment2d segment){
        // ho i nodi dei trapezoidi da modificare
         std::vector<DagNode*> trapezoidsNode = dag.query(segment);

        std::cout << "fin qui ci sono!" << std::endl;
         // ho aggiunto 4 nuovi trapezoidi alla trapezoidal map
         Trapezoid bb = *(Trapezoid*)trapezoidsNode.back()->getData().objj;
         std::vector<Trapezoid> trapforDag = trapMap.addFourTrapezoids(segment,bb);

         //aggiorno la dag con i nuovi trapezoidi
        dag.updateDag(trapforDag, trapezoidsNode, segment);

    }


}
