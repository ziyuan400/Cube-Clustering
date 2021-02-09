#ifndef GRAPH_H
#define GRAPH_H

#include <cassert>
#include <vector>
#include <random>
#include <algorithm>

#include <plotter.h>
#include <Eigen/Dense>

class Graph{
private:
    int number_of_v, number_of_e = 0,   // number_of_v = |V|, number_of_e = |S|
        number_of_t = 0;                // number_of_t : number of all trangle relations;
    int     *p,                         //[number_of_v] The array of group id of each vertex for PERTITIONS => {0,0,1,1,1} means {{1,2},{3,4,5}}
            *yOfPi;                     //[number_of_e] S=>{0,1}, 1 if both v belong to the same U.
    float   *c,                         //[number_of_e] set of COST of all edges
            *theta,                     //[number_of_v] attributes on each vertex;
            **x,                        //[number_of_e][number_of_v] attribute vector of every pair(a, a')
            *tri_cost, *tri_cost_p,     //[number_of_t] T
            *point_location;            //[number_of_v * 3]
    Plotter plotter;

public:
    //***************************************
    //    Initialization and I/O
    //***************************************
    Graph(int number_of_v);
    ~Graph();
    int* pi0();
    void input_x(float** input_x);
    void input_c_cp(float* c, float* cp);
    void input_theta(float* input_theta);
    void input_points(float* points);

    //***************************************
    //    Computations
    //***************************************

    float phi(int* pi);
    float cube_phi(int* new_pi);
    float cube_phi_d(int *pi);
    float cube_phi_d(int *pi, int i, float old_phi, int new_group);
    float cost_generator(float *p1, float *p2, float *p3);
    int get_edge_id(int start, int end);
    int get_n_er_relation_id(int* start, int n_er_relation);

    //***************************************
    //    Algorithms
    //***************************************

    int* greedy_move(int* pi);
    int* kl(int* pi);
    int* cube_clustering();
    int *cube_clustering_d();

    //***************************************
    //    Tools
    //***************************************

    int* move(int a,int U, int* pi);
    int get_y_of_pi(int a, int a_prime);
    int num_of_gruops(int* pi);
    int* get_y_from_pi(int *pi);
    void print(char c);
    void addPlotInfo(std::string name, int *data);
    float cube_phi_test();
    float evaluation(int *gt);

    //***************************************
    //    To-Do list
    //***************************************
    float* compute_p_of_Y(float* x);
    float* normal_distribution(float* theta, float rou);
};


#endif // GRAPH_H
