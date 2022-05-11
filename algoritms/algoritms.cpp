#include "algoritms.h"

namespace algoritms
{
    void buildTrapMapDag(DAG& dag, TrapMap& trapMap, const cg3::Segment2d segment){
        cg3::Segment2d s;
        bool leftToRight = true;

        if (segment.p2().x() < segment.p1().x()) {
            leftToRight = false; // il segmento non é orientato da sx -> dx
        }

        // ho i nodi dei trapezoidi da modificare
         DagNode* trapezoidNode = dag.query(segment);


         // ho aggiunto 4 nuovi trapezoidi alla trapezoidal map
        std::vector<Trapezoid*> trapforDag = trapMap.addFourTrapezoids(segment,trapezoidNode, leftToRight);

        Trapezoid bb = *(Trapezoid*)trapezoidNode->getData().objj;
        int id = bb.getId();

         //aggiorno la dag con i nuovi trapezoidi
        if (leftToRight){
            dag.updateDag(trapforDag, trapezoidNode, segment);
        }else{
            s = cg3::Segment2d(segment.p2(), segment.p1());
            dag.updateDag(trapforDag, trapezoidNode, s);
        }

        // elimino il trapezoide dalla trapezoidal map
        trapMap.deleteTrapezoidWithId(id);

    }


}
