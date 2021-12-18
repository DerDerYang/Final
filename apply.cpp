#include "apply.h"
#include "vector"

using namespace std;

val operation(val v1, val v2, char op) {
    if (v1 == X || v2 == X) {
        return X;
    }
    if (op == '+') {
        bool temp = v1 | v2;
        if (temp) {
            return True;
        } else {
            return False;
        }
    } else if (op == '*') {
        bool temp = v1 & v2;
        if (temp) {
            return True;
        } else {
            return False;
        }
    }
}

vertex *apply_step(vertex *v1, vertex *v2, vector<vector<vertex *>> table, char op,int n,int &flag) {
    flag++;
    if(flag>=3){
        return nullptr;
    }
    vertex *u = table[v1->getId()][v2->getId()];
    if (u != nullptr)
        return u;
    u = new vertex();
    table[v1->getId()][v2->getId()] = u;
    u->setValue(operation(v1->getValue(), v2->getValue(), op));
    if(u->getValue()!=X){

        u->setIndex(n+1);
        u->setLow(nullptr);
        u->setHigh(nullptr);

    }else{
        u->setIndex(min(v1->getIndex(),v2->getIndex()));
        vertex *vLow1,*vHigh1,*vLow2,*vHigh2;
        if(v1->getIndex()==u->getIndex()){
            vLow1 = v1->getLow();
            vHigh1 = v1->getHigh();
        } else{
            vLow1 = v1;
            vHigh1 = v1;
        }
        if(v2->getIndex()==u->getIndex()){
            vLow2 = v2->getLow();
            vHigh2 = v2->getHigh();
        } else{
            vLow2 = v2;
            vHigh2 = v2;
        }
        u->setLow(apply_step(vLow1,vLow2,table,op,n,flag));
        u->setHigh(apply_step(vHigh1,vHigh2,table,op,n,flag));
    }
    return u;
}

vertex *apply(vertex *v1, vertex *v2, char op,int n,vector<vector<vertex *>> &table) {
    int flag = 0;
    return apply_step(v1, v2, table, op,n,flag);
}
