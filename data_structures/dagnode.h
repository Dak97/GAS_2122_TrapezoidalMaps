#ifndef DAGNODE_H
#define DAGNODE_H


class DagNode
{

public:

    enum TypeNode{ Left, Right, Segment, Trapezoid}; // type of object

    typedef struct {
        TypeNode type; // type of the object
        void *obj; // reference to the object
    }DagData;

    DagNode(DagNode::TypeNode type, void *objj);
    ~DagNode();
    const DagData &getData() const;
    void setData(const DagData &newData);
    void setObject(void* obj);

    DagNode *left; // pointer to the left node
    DagNode *right; // pointer to the right node
//    DagNode *parent; // punta al nodo padre
private:
    DagData data; // conteiner of the data

};

#endif // DAGNODE_H
