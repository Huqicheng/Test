
#include "input.h"
#include "helper.h"

pthread_mutex_t mc = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t m2 = PTHREAD_MUTEX_INITIALIZER;
int main(){
    
    char V;
    MGraph G;
    pthread_t thread_cnf,thread_approx1,thread_approx2,thread_print;
    while (std::cin >> V) {
        int flag = 10;
        if (V == 'V') {
            
            flag = CreateMGraph(&G);
            if (flag == 0) {
                goto end;
            }
        }
        if (flag == 2) {
//            printf("CNF-SAT-VC: \n");
//            printf("APPROX-VC-1: \n");
//            printf("APPROX-VC-2: \n");
//            fflush(stdout);
            std::cout <<"CNF-SAT-VC: "<<std::endl;
            std::cout << "APPROX-VC-1: " << std::endl;
            std::cout << "APPROX-VC-2: " << std::endl;
        }
        else{
                void *result_cnf, *result_approx1, *result_approx2, *result_print;
                
                mulock(LOCK, &mc);
                // Create thread for CNF_SAT_VC
                if (pthread_create(&thread_print, NULL, &Output, (void *)&G) == -1) {
                    puts("fail to create pthread thread_approx2");
                    exit(1);
                }
                if (pthread_create(&thread_cnf, NULL, &CNF_SAT_VC, (void *)&G) == -1) {
                    puts("fail to create pthread thread_cnf");
                    exit(1);
                }
                if (pthread_join(thread_cnf, &result_cnf) == -1) {
                    puts("fail to recollect thread_cnf");
                    exit(1);
                } 

                if (pthread_create(&thread_approx1, NULL, &ApproxVc1, (void *)&G) == -1) {
                    puts("fail to create pthread thread_approx1");
                    exit(1);
                }
                if (pthread_join(thread_approx1, &result_approx1) == -1) {
                    puts("fail to recollect thread_approx1");
                    exit(1);
                }
                if (pthread_create(&thread_approx2, NULL, &ApproxVc2, (void *)&G) == -1) {
                    puts("fail to create pthread thread_approx2");
                    exit(1);
                }

                // Wait thread terminate
                if (pthread_join(thread_approx2, &result_approx2) == -1) {
                    puts("fail to recollect thread_approx2");
                    exit(1);
                }

                // Wait thread terminate
                if (pthread_join(thread_print, &result_print) == -1) {
                    puts("fail to recollect thread_approx2");
                    exit(1);
               // }
                }
            
        }
        
    }
    
end:    return 0;
}


int CreateMGraph(MGraph *G)
{
loop:
    int i, j, k;
    std::cin >> v;
    G->numVertexes = v;    //input the number of vertices
    char E;
    int num = (v + 1)*(v + 1) * 7;
    char *p = new char[num];
    int count = 0;
    int countFigure = 0;
    int DecNum = 0;
    int countP = 0;
    int tmp;
    
    std::cin >> E;
    for (int i = 0; i < num; ++i) {
        std::cin >> p[i];
        countP++;
        if (p[i] == '}') {
            break;
        }
    }//input E to p
    
    if (p[1] != '}') {
        k = 0;
        while (k < countP) {
            
            if (p[k] != '{'&&p[k] != '}'&&p[k] != '<'&&p[k] != '>'&&p[k] != ',') {
                if (k > 0) {
                    if ((p[k - 1] <= '9') && (p[k - 1] >= '0')) {
                        tmp = 10 * (p[k - 1] - 48) + p[k] - 48;
                        k++;
                    }
                    else {
                        tmp = p[k] - 48;
                        k++;
                    }
                }
                else {
                    tmp = p[k] - 48;
                    k++;
                }
            }
            else {
                tmp = 0;
                k++;
            }
            while (tmp >= v) {
                std::cerr << "Error:invalid edge." << std::endl;
                std::cin.clear();
                countP = 0;
                if (std::cin >> E) {
                    if (E == 'V') { goto loop; }
                    for (int i = 0; i < num; ++i) {
                        std::cin >> p[i];
                        countP++;
                        if (p[i] == '}') {
                            countP++;
                            break;
                        }
                    }//input E to p
                    k = 0;
                    tmp = 0;
                }
                else {
                    return 0;
                }
                
            }
        }
        
        
        if (v > 0) {
            i = 0;
            while (p[i] != '}') {
                count++;
                if (p[i] != '{'&&p[i] != '}'&&p[i] != '<'&&p[i] != '>'&&p[i] != ',') {
                    countFigure++;
                }
                i++;
            }//caculate size of p
            i = 0;
            j = 0;
            
            
            int *stringPointer = new int[countFigure];
            while (p[i] != '}') {
                if (p[i] != '{'&&p[i] != '}'&&p[i] != '<'&&p[i] != '>'&&p[i] != ',') {
                    if (i > 0) {
                        if ((p[i - 1] <= '9') && (p[i - 1] >= '0')) {
                            stringPointer[j - 1] = 10 * (p[i - 1] - 48) + p[i] - 48;
                            DecNum++;
                        }
                        else {
                            stringPointer[j] = p[i] - 48;
                            j++;
                        }
                    }
                    else {
                        stringPointer[j] = p[i] - 48;
                        j++;
                    }
                }
                i++;
            }
            delete[]p;
            
            int *stringFinal = new int[countFigure - DecNum];
            for (int i = 0; i < countFigure - DecNum; ++i) {
                stringFinal[i] = stringPointer[i];
            }
            delete[]stringPointer;
            G->numEdges = countFigure - DecNum;
            //std::cout << G->numEdges << std::endl;
            
            std::cin.clear();
            
            
            
            for (int i = 0; i < G->numVertexes; ++i) {
                for (int j = 0; j < G->numVertexes; ++j)
                {
                    if (i == j) {
                        G->arc[i][j] = 0;
                    }
                    else {
                        G->arc[i][j] = 9999;
                    }
                    
                }
            }
            
            for (int k = 0; k < G->numEdges; k=k+2){
                i = stringFinal[k];
                j = stringFinal[k + 1];
                if (i != j) {
                    G->arc[i][j] = 1;
                    G->arc[j][i] = G->arc[i][j];
                }
            }
            
            delete[]stringFinal;
            return 1;
        }
    }
    //else {
    return 2;
    //}
}

/************************************************************************/
/* CNF - SAT - VC                                                       */
/************************************************************************/
void* CNF_SAT_VC(void *graph){
    //mulock(UNLOCK, &l);
    MGraph * G = (MGraph *)graph;
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    Minisat::Lit x[MAXVEX][MAXVEX];
    //int vertexcover[v];

    int k = 1;
    for (k = 1; k < v; ++k) {
        for (int i = 0; i < v; ++i) {
            for (int j = 0; j < k; ++j) {
                x[i][j] = Minisat::mkLit(solver->newVar());
            }
        }
        //std::cout << k << std::endl;
        Minisat::vec<Minisat::Lit> constr1[k];
        for (int i = 0; i < k; ++i) {
            for (int j = 0; j < v; ++j) {
                constr1[i].push(x[j][i]);
            }
            solver->addClause(constr1[i]);
        }
        //std::cout << "1"<<" ";
        Minisat::vec<Minisat::Lit> constr2[v*k*(k - 1) / 2];
        int i = 0;
        for (int m = 0; m < v; ++m) {
            for (int p = 0; p < k; ++p) {
                for (int q = 0; q < k; ++q) {
                    if (p < q) {
                        constr2[i].push(~x[m][p]);
                        constr2[i].push(~x[m][q]);
                        solver->addClause(constr2[i]);
                        i++;
                    }
                }
            }
        }
        //std::cout << "2" << " ";
        Minisat::vec<Minisat::Lit> constr3[k*v*(v - 1) / 2];
        int j = 0;
        for (int m = 0; m < k; ++m) {
            for (int p = 0; p < v; ++p) {
                for (int q = 0; q < v; ++q) {
                    if (p < q) {
                        constr3[j].push(~x[p][m]);
                        constr3[j].push(~x[q][m]);
                        solver->addClause(constr3[j]);
                        j++;
                    }
                }
            }
        }
        //std::cout << "3" << " ";
        int count = 0;
        Minisat::vec<Minisat::Lit> constr4[(G->numEdges) * 2];
        for (int i = 0; i < v; ++i) {
            for (int j = 0; j < v; ++j)
            {
                if ((G->arc[i][j]) == 1) {
                    for (int m = 0; m < k; ++m) {
                        constr4[count].push(x[i][m]);
                        constr4[count].push(x[j][m]);
                    }
                    solver->addClause(constr4[count]);
                    count++;
                }
            }
        }
        //std::cout << "4" << std::endl;
        bool res = solver->solve();
        if (res == 1) {
            //std::cout << "the size of k:" << k << std::endl;
            int m = 0;
            for (int i = 0; i < v; ++i) {
                for (int j = 0; j < k; ++j) {
                    if (Minisat::toInt(solver->modelValue(x[i][j])) == 0) {
                        G -> cnf_vc[m] = i;
                        m++;
                    }
                }
            }
            solver.reset(new Minisat::Solver());
            break;
        }
        else {
            solver.reset(new Minisat::Solver());
            continue;
        }
        
    }
    G-> cnf_size = k;
    //G -> *cnf_vc = vertexcover;
//    std::cout << "CNF-SAT-VC: ";
//    if (k == v) {
//        for (int i = 0; i < (v - 1); ++i) {
//            std::cout << i << " ";
//        }
//        std::cout << v - 1 << std::endl;
//    }
//    else {
//        for (int i = 0; i < (k - 1); ++i) {
//            std::cout << vertexcover[i] << ",";
//        }
//        std::cout << vertexcover[k - 1] << std::endl;
//    }
//     mulock(UNLOCK, &mc);
    return NULL;
    
}
/************************************************************************/
/* APPROX - VC - 1                                                                 */
/************************************************************************/
void* ApproxVc1(void *graph) {
    /*check if the matrix is 0*/
    MGraph * G = (MGraph *)graph;
    int degree[v];
    
    int max = -1;
    int index = 0;
    //int S[v];
    int count = 0;
    int arc[v][v];
    
    for (int i = 0; i < v; ++i) {
        for (int j = 0; j < v; ++j) {
            arc[i][j] = G->arc[i][j];
            /*cout << arc[i][j] << " ";*/
        }
        /*cout << endl;*/
    }
    
    
    while (max != 0) {
        /*cout <<"max:" <<max << " ";*/
        
        for (int i = 0; i < v; ++i) {
            degree[i] = 0;
        }
        for (int i = 0; i < v; ++i) {
            for (int j = 0; j < v; ++j) {
                if (arc[i][j] == 1) {
                    degree[i]++;
                }
            }
        }
        
        /*for (int i = 0; i < v; ++i) {
         cout << "degree "<<i<<":"<<degree[i] << " ";
         }*/
        max = degree[0];
        index = 0;
        for (int i = 1; i < v; ++i) {
            if (degree[i] > max) {
                max = degree[i];
                index = i;
            }
        }
        G -> approx1_vc[count] = index;
        count++;
        for (int i = 0; i < v; ++i) {
            arc[index][i] = 0;
            arc[i][index] = 0;
        }
        
    }

    sort(G -> approx1_vc, G -> approx1_vc + count - 1);
    G-> approx1_size = count;
//    std::cout << "APPROX-VC-1: ";
//    for (int i = 0; i < count - 2; ++i) {
//        std::cout << S[i] << ",";
//    }
//    std::cout << S[count - 2] << std::endl;
    //mulock(UNLOCK, &m1);
    return NULL;
}


/************************************************************************/
/* APPROX - VC - 2                                                                 */
/************************************************************************/
void* ApproxVc2(void *graph) {
    MGraph * G = (MGraph *)graph;
    //int vertexCover[v];
    int arc[v][v];
    for (int i = 0; i < v; ++i) {
        for (int j = 0; j < v; ++j) {
            arc[i][j] = G->arc[i][j];
        }
    }
    
    for (int i = 0; i < v; ++i) {
        G -> approx2_vc[i] = -1;
    }
    int count1 = 0;
    int flagIn = 0;
    for (int i = 0; i < v; ++i) {
        for (int j = 0; j < v; ++j) {
            if (arc[i][j] == 1) {
                for (int k = 0; k < v; ++k) {
                    if (( G -> approx2_vc[k] == i) || ( G -> approx2_vc[k] == j)) {
                        flagIn = 1;
                        break;
                    }
                    else {
                        flagIn = 2;
                    }
                }
                if (flagIn == 2) {
                     G -> approx2_vc[count1] = i;
                    count1++;
                     G -> approx2_vc[count1] = j;
                    count1++;
                    for (int k = 0; k < v; ++k) {
                        arc[i][k] = 0;
                        arc[k][i] = 0;
                        arc[j][k] = 0;
                        arc[k][j] = 0;
                        
                    }
                }
            }
        }
    }
    sort( G -> approx2_vc,  G -> approx2_vc + count1);
    G-> approx2_size = count1;
//    std::cout << "APPROX-VC-2: ";
//    for (int i = 0; i < count1 - 1; ++i) {
//        std::cout << vertexCover[i] << ",";
//    }
//    std::cout << vertexCover[count1 - 1] << std::endl;
    mulock(UNLOCK, &mc);
    return NULL;
}

void* Output(void *graph){
    
    std::cout << "BALABALA\n ";
    MGraph * G = (MGraph *)graph;
    
    mulock(LOCK, &mc);
    //Output CNF-SAT result
    std::cout << "CNF-SAT-VC: ";
    
    for (int i = 0; i < (G ->cnf_size - 1); ++i) {
        std::cout << G -> cnf_vc[i] << ",";
    }
    std::cout << G -> cnf_vc[G ->cnf_size - 1] << std::endl;
    
    //Output APPROX1 result
    std::cout << "APPROX-VC-1: ";
    for (int i = 0; i < G ->approx1_size - 2; ++i) {
        std::cout << G ->approx1_vc[i] << ",";
    }
    std::cout << G ->approx1_vc[G ->approx1_size - 2] << std::endl;
    
    //Output APPROX2 result
    std::cout << "APPROX-VC-2: ";
    for (int i = 0; i < G ->approx2_size - 1; ++i) {
        std::cout << G ->approx2_vc[i] << ",";
    }
    std::cout << G ->approx2_vc[G ->approx2_size - 1] << std::endl;
    return NULL;
}

