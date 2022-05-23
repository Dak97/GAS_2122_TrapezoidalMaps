#include "algorithm.h"

namespace Algorithm
{
    void buildTrapMapDag(Dag& dag, TrapMap& trapMap, const cg3::Segment2d segment){
        cg3::Segment2d s = segment;
        std::pair<DagNode*, DagNode*> trapsNodeQuery;
        std::vector<Trapezoid*> trapforDag, newTrapsForDag;
        bool leftToRight = true;

        if (s.p2().x() < s.p1().x()) {
            leftToRight = false; // il segmento non é orientato da sx -> dx
            s = cg3::Segment2d(s.p2(), s.p1());
        }

        // ho i nodi dei trapezoidi da modificare
         trapsNodeQuery = dag.query(s, leftToRight);

         // il primo ed il secondo punto giaciono sullo stesso trapezoide
         if ((*(Trapezoid*)trapsNodeQuery.first->getData().objj).getId() ==
                 (*(Trapezoid*)trapsNodeQuery.second->getData().objj).getId()){

             // ho aggiunto 4 nuovi trapezoidi alla trapezoidal map
             trapforDag = trapMap.addFourTrapezoids(s,trapsNodeQuery.first, leftToRight);

             int bbId = (*(Trapezoid*)trapsNodeQuery.first->getData().objj).getId();

              //aggiorno la dag con i nuovi trapezoidi
             if (leftToRight){
                 dag.updateDag(trapforDag, trapsNodeQuery.first, s);
             }else{
                 dag.updateDag(trapforDag, trapsNodeQuery.first, s);
             }

             // elimino il trapezoide dalla trapezoidal map
             trapMap.deleteTrapezoidWithId(bbId);
         }else{
             // eseguo il follow segment per trovare tutti i trapezoidi da modificare
             trapforDag = dag.followSegment(s, trapsNodeQuery.first);

             newTrapsForDag = trapMap.newTrapezoids(s, trapforDag, leftToRight);

             //aggiornamento della dag con i nuovi trapezoidi
             dag.updateDag2(newTrapsForDag, trapforDag, s);

             for (Trapezoid* t : trapforDag){
                     trapMap.deleteTrapezoidWithId(t->getId());
             }

         }
    }

    bool pointIsAboveSegment(cg3::Segment2d segment, cg3::Point2d point){

        cg3::Point2d v1 = cg3::Point2d(segment.p2().x()-segment.p1().x(),segment.p2().y()-segment.p1().y());
        cg3::Point2d v2 = cg3::Point2d(point.x()-segment.p1().x(),point.y()-segment.p1().y());
        double cross_product = v1.x() * v2.y() - v1.y() * v2.x();

        if (cross_product > 0){
            //il punto si trova sopra il segmento
            return true;
        }else{
            // il punto si trova sotto il segmento o sul
            return false;
        }
    }

}
