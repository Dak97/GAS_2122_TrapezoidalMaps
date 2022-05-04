#ifndef DAGNODE_H
#define DAGNODE_H


class DagNode
{

public:

    enum TypeNode{ Left, Right, Segment, Trapezoid};

    typedef struct {
        TypeNode type; // tipo di oggetto che contiene il nodo
        void *objj; // puntatore all'oggetto
    }DagData;

    DagNode(DagNode::TypeNode type, void *objj);
    ~DagNode();
    const DagData &getData() const;
    void setData(const DagData &newData);
    void setObject(void* obj);

    DagNode *left; // punta al figlio sinistro
    DagNode *right; // punta al figlio destro
    DagNode *parent; // punta al nodo padre
private:
    DagData data; // contiene i dato del nodo della DAG

};

#endif // DAGNODE_H
