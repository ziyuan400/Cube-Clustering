#include "graph.h"


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

    plotter = Plotter();
}

int* Graph::pi0(){
    for(int i = 0; i < number_of_v; i++){
        p[i] = 0;
    }
    int *pi = (int*)malloc (number_of_v * sizeof (int));
    for(int i = 0; i < number_of_v; i++){
        pi[i] = 0;
    }
    return pi;
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
    const float MIN_RAND = 2.0, MAX_RAND = 6.0;
    const float range = MAX_RAND - MIN_RAND;
    if(input_x!=0){
        x = input_x;
        return;
    }
    for(int i = 0; i < number_of_e; i++){
        for(int j = 0; j < number_of_v; j++){
            x[i][j] = range * ((((float) rand()) / (float) RAND_MAX)) + MIN_RAND ;
        }
    }
}

void Graph::print(char c){
    switch(c){
    case 'x':
        plotter.print_to_cout("float2D", x, number_of_e, "X", number_of_v);
    case 't':
        plotter.print_to_cout("float", theta, number_of_v, "Parameter THETA", 0);
        break;
    case 'p':
        plotter.print_to_cout("int", p, number_of_v, "Partation PI", 0);
    }
}
//***************************************
//    (6, 19)
//    phi of y = <c, y_of_pi>
//    c  (a, a') = <phi, x[get_edge_id(a,a')]>
//***************************************
float Graph::phi(int* pi){
    int *y = get_y_from_pi(pi);
    for(int i = 0; i < number_of_e; i++){
        c[i] = 0;
        for(int j = 0; j < number_of_v; j++){
            c[i] += x[i][j] * theta[j];
        }
    }
    float phi = 0;
    for(int i = 0; i < number_of_e; i++){
        phi += c[i] * y[i];
    }
    free(y);
    return phi;
}

int* Graph::get_y_from_pi(int *pi){
    int *new_yOfPi = (int*)malloc (number_of_e * sizeof (int));
    for(int i = 0; i < number_of_v; i++){
        for(int j = i; j < number_of_v; j++){
            if(pi[i] == pi[j]){
                new_yOfPi[get_edge_id(i+1,j+1)] = 1;
            }else{
                new_yOfPi[get_edge_id(i+1,j+1)] = 0;
            }
        }
    }
    //plotter.print_to_cout("int", new_yOfPi, number_of_e, "NEW Y OF PI", 0);
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


int* Graph::greedy_move(int* pi){
    int best_a=-1, best_U=-1, gruop = 0;
    int *min_pi, *new_pi;
    float phi_y_pi = phi(pi), new_phi, min_diff = 0;
    gruop = num_of_gruops(pi);                                       //num_of_gruops = 1 for {0,0,0,0,0}

    for(int i = 0; i < number_of_v; i++){
        for(int j = 0; j < gruop+1; j++){
            new_pi = move(i, j, pi);
            new_phi = phi(new_pi);
            if(min_diff < phi_y_pi - new_phi){
                best_a = i;
                best_U = j;
                min_diff = phi_y_pi - new_phi;
            }

            plotter.print_to_cout("diff", new_pi, number_of_v, "", phi_y_pi - new_phi);
            free(new_pi);
        }
    }

    if(min_diff > 0 && best_U >=0){
        min_pi = move(best_a, best_U, pi);
        plotter.print_to_cout("diff", min_pi, number_of_v, "MIN_PI TO FIND NEXT MOVE", min_diff);
        free(pi);
        return greedy_move(min_pi);
    }else{

        plotter.print_to_cout("diff", pi, number_of_v, "FINAL PARTITION", min_diff);
        return pi;
    }
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
    assert(0 < start <= number_of_v || 0 < end <= number_of_v);
    //If first vertex(start) has a greater id, swap them.
    if(start > end){
        start = start + end;
        end = start - end;
        start = start - end;
    }
    return (start - 1) * (number_of_v * 2 - start + 2) / 2 + end - start;
}


