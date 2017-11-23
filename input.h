//
//  input.h
//  Threads
//
//  Created by Elaine on 2017/11/18.
//  Copyright © 2017年 Elaine. All rights reserved.
//

// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <iostream>

#define MAXVEX 100
#define QueueSize 100
typedef char VertexType;
typedef int EdgeType;
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
