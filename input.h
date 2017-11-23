// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"
#include "helper.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <algorithm>
#include <errno.h>
#include <pthread.h>
#include <iostream>
#include <memory>
#ifndef _HELPER_H_
#define _HELPER_H_    1


enum {
   LOCK,
   UNLOCK,
};

extern void errp(char *s, int code);
extern void thr_sleep(time_t sec, long nsec);
extern void mulock(int ul, pthread_mutex_t *m);
#endif

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
