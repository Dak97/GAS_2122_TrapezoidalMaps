#include "algoritms.h"

namespace algoritms
{
    void buildTrapMapDag(DAG& dag, TrapMap& trapMap, const cg3::Segment2d segment){
        cg3::Segment2d s;
        std::pair<DagNode*, DagNode*> trapsNodeQuery;
        std::vector<Trapezoid*> trapforDag;
        bool leftToRight = true;

        if (segment.p2().x() < segment.p1().x()) {
            leftToRight = false; // il segmento non é orientato da sx -> dx
        }

        // ho i nodi dei trapezoidi da modificare
         trapsNodeQuery = dag.query(segment, leftToRight);

         // il primo ed il secondo punto giaciono sullo stesso trapezoide
         if ((*(Trapezoid*)trapsNodeQuery.first->getData().objj).getId() ==
                 (*(Trapezoid*)trapsNodeQuery.second->getData().objj).getId()){

             // ho aggiunto 4 nuovi trapezoidi alla trapezoidal map
             trapforDag = trapMap.addFourTrapezoids(segment,trapsNodeQuery.first, leftToRight);
         }else{
             // eseguo il follow segment per trovare tutti i trapezoidi da modificare
             trapforDag = dag.followSegment(segment, trapsNodeQuery.first);

         }

        int bbId = (*(Trapezoid*)trapsNodeQuery.first->getData().objj).getId();

         //aggiorno la dag con i nuovi trapezoidi
        if (leftToRight){
            dag.updateDag(trapforDag, trapsNodeQuery.first, segment);
        }else{
            s = cg3::Segment2d(segment.p2(), segment.p1());
            dag.updateDag(trapforDag, trapsNodeQuery.first, s);
        }

        // elimino il trapezoide dalla trapezoidal map
        trapMap.deleteTrapezoidWithId(bbId);

    }


}
