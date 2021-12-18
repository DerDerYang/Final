#include "BDD.h"
#include "vector"

#ifndef FINAL_APPLY_H
#define FINAL_APPLY_H

using namespace std;
vertex *apply(vertex *v1, vertex *v2, char op, int n, vector<vector<vertex *>> &table);

//vertex *apply_step(vertex *v1, vertex *v2);

#endif //FINAL_APPLY_H
