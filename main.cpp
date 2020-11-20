#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>             //assert, malloc and random are some how packaged in it

class Graph{
private:
    int number_of_v, number_of_e = 0;   // number_of_v = |V|, number_of_e = |S|
    int     *p,                         //[number_of_v] The array of group id of each vertex for PERTITIONS => {0,0,1,1,1} means {{1,2},{3,4,5}}
            *yOfPi;                     //[number_of_e] S=>{0,1}, 1 if both v belong to the same U.
    float   *c,                         //[number_of_e] set of COST of all edges
            *theta,                     //[number_of_v] attributes on each vertex;
            **x;                        //[number_of_e][number_of_v] attribute vector of every pair(a, a')

public:
    //***************************************
    //    Initialization and I/O
    //***************************************
    Graph(int number_of_v);
    int* pi0();
    void input_x(float** input_x);
    void input_theta(float* input_theta);

    //***************************************
    //    Computations
    //***************************************
    float phi();
    float phi(int* pi);
    int* greedy_move(int* pi);


    //***************************************
    //    Tools
    //***************************************
    int* move(int a,int U, int* pi);
    int get_edge_id(int start, int end);
    int get_y_of_pi(int a, int a_prime);
    int num_of_gruops(int* pi);
    int* get_y_from_pi(int *pi);

    //***************************************
    //    To-Do list
    //***************************************
    float* compute_p_of_Y(float* x);
    float* normal_distribution(float* theta, float rou);
};

int greedy_move();
int kl(Graph graph, int* pi);

int* Graph::greedy_move(int* pi){
    int best_a=0, best_U=0, min_diff = 0;
    int *min_pi, *new_pi;
    float phi_y_pi = phi(), new_phi;
    for(int i = 0; i < number_of_v; i++){
        int nog = num_of_gruops(pi);
        for(int j = 0; j < nog; j++){
            new_pi = move(i, j, pi);
            new_phi = phi(new_pi);
            if(min_diff < phi_y_pi - new_phi){
                best_a = i;
                best_U = j;
            }
            free(new_pi);
        }
    }
    min_pi = move(best_a, best_U, pi);
    if(min_diff < 0){
        return greedy_move(min_pi);
    }else{
        return pi;
    }

}

//int kl(Graph graph, int* pi){

//}
//***************************************
//    Constructor: Allocation only
//***************************************
Graph::Graph(int number_of_vertex){
    number_of_v = number_of_vertex;
    number_of_e = number_of_vertex * (number_of_vertex + 1) / 2;
    x = (float**)malloc (number_of_e * sizeof (float*));
    for(int i = 0; i < number_of_e; i++){
        x[i] = (float*)malloc (number_of_v * sizeof (float));
    }
    c = (float*)malloc (number_of_e * sizeof (float));
    theta = (float*)malloc (number_of_v * sizeof (float));
    p = (int*)malloc (number_of_v * sizeof (int));
    yOfPi = (int*)malloc (number_of_e * sizeof (int));
}
int* Graph::pi0(){
    for(int i = 0; i < number_of_v; i++){
        p[i] = 0;
    }
    return p;
}
void Graph::input_theta(float* input_theta){
    if(input_theta!=0){
        theta = input_theta;
        return;
    }
    for(int i = 0; i < number_of_v; i++){
        theta[i] = random()%100;
    }
}

void Graph::input_x(float** input_x){
    if(x!=0){
        x = input_x;
        return;
    }
    for(int i = 0; i < number_of_e; i++){
        for(int j = 0; j < number_of_v; j++){
            x[i][j] = random()%100;
        }
    }
}

//***************************************
//    (6, 19)
//    phi of y = <c, y_of_pi>
//    c  (a, a') = <phi, x[get_edge_id(a,a')]>
//***************************************
float Graph::phi(){
    for(int i = 0; i < number_of_e; i++){
        c[i] = 0;
        for(int j = 0; j < number_of_v; j++){
            c[i] += x[i][j] * theta[j];
        }
    }
    float phi = 0;
    for(int i = 0; i < number_of_e; i++){
        phi += c[i] * yOfPi[i];
    }
    return phi;
}

float Graph::phi(int* pi){
    for(int i = 0; i < number_of_e; i++){
        c[i] = 0;
        for(int j = 0; j < number_of_v; j++){
            c[i] += x[i][j] * theta[j];
        }
    }
    float phi = 0;
    for(int i = 0; i < number_of_e; i++){
        phi += c[i] * yOfPi[i];
    }
    return phi;
}

int* Graph::get_y_from_pi(int *pi){
    int *new_yOfPi = (int*)malloc (number_of_e * sizeof (int));
    for(int i = 0; i < number_of_v; i++){
        for(int j = i; j < number_of_v; j++){
            if(pi[i] == pi[j]){
                new_yOfPi[get_edge_id(i,j)] = 1;
            }else{
                new_yOfPi[get_edge_id(i,j)] = 0;
            }
        }
    }
    return new_yOfPi;
}

//************************************************************
//    1.Copy the origin partition. Remember to FREE it.
//    2.Change the gruop-id to the new one.
//    3.Reassign group-id if the last element deleted.
//************************************************************

int* Graph::move(int a,int U, int* pi){
    int *after;
    int max = pi[0], u0 = pi[a];
    bool empty_group = true;
    after =  (int*)malloc(sizeof(int) * number_of_v);
    for(int i = 0; i < number_of_v; i++){
        after[i] = pi [i];
        if (max < pi[i]){
            max = pi[i];
        }
        if (u0 == pi[i] && i != a){
            empty_group = false;
        }
    }
    after[a] = U;
    if(empty_group == true){
        for(int i = 0; i < number_of_v; i++){
            if(after[i] == max){
                after[i] = u0;
            }
        }
    }
    return after;
}
//************************************************************
//    |U|
//************************************************************
int Graph::num_of_gruops(int* pi){
    int max = pi[0];
    for(int i = 0; i < number_of_v; i++){
        if (max < pi[i]){
            max = pi[i];
        }
    }
    return max+1;
}

int Graph::get_edge_id(int start, int end){
    assert(0 <= start < number_of_v || 0 <= end < number_of_v);
    //If first vertex(start) has a greater id, swap them.
    if(start > end){
        start = start + end;
        end = start - end;
        start = start - end;
    }
    return (start - 1) * (number_of_v * 2 - start + 2) + end - start;
}




int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();


    Graph graph = Graph(5);
    graph.input_x(0);
    graph.input_theta(0);
    graph.greedy_move(graph.pi0());


}
