// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <algorithm>
#include <errno.h>
#include <pthread.h>
#include <iostream>
#include <memory>


#define MAXVEX 100
#define QueueSize 100
typedef char VertexType;
typedef int EdgeType;
using namespace std;
int v = 0;

class MGraph {
public:
	VertexType vexs[MAXVEX];
	EdgeType arc[MAXVEX][MAXVEX];
	int numVertexes, numEdges;
};

int CreateMGraph(MGraph *);
void* CNF_SAT_VC(void *);
void* ApproxVc1(void *);
void* ApproxVc2(void *);
