#include "graph.h"

//***************************************
//    Constructor: Allocation only
//***************************************
Graph::Graph(int number_of_vertex){
    number_of_v = number_of_vertex;
    number_of_e = number_of_vertex * (number_of_vertex + 1) / 2;
    number_of_t = number_of_vertex * (number_of_vertex - 1) * (number_of_vertex - 2) / 6;
    x = (float**)malloc (number_of_e * sizeof (float*));
    for(int i = 0; i < number_of_e; i++){
        x[i] = (float*)malloc (number_of_v * sizeof (float));
    }
    c = (float*)malloc (number_of_e * sizeof (float));
    tri_cost = (float*)malloc (number_of_v * number_of_v * number_of_v * sizeof (float));
    tri_cost_p = (float*)malloc (number_of_v * number_of_v * number_of_v * sizeof (float));
    theta = (float*)malloc (number_of_v * sizeof (float));
    p = (int*)malloc (number_of_v * sizeof (int));
}
Graph::~Graph(){

    //******************************************************************************
    //    TODO: free memory.
    //******************************************************************************
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
void Graph::input_points(float* points){
    if(points!=0){
        point_location = points;
        return;
    }
    for(int i = 0; i < number_of_v*3; i++){
        point_location[i] = random()%100;
    }
}
void Graph::input_c_cp(float *c, float *cp){
    if(c!=0){
        tri_cost = c;
        tri_cost_p = cp;
        return;
    }
    for(int i = 0; i < number_of_v *number_of_v *number_of_v; i++){
        tri_cost[i] = random()%100;
        tri_cost_p[i] = random()%100;
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
        plotter.log("float2D", x, number_of_e, "X", number_of_v);
    case 't':
        plotter.log("float", theta, number_of_v, "Parameter THETA", 0);
        break;
    case 'p':
        plotter.log("int", p, number_of_v *number_of_v *number_of_v, "Partation PI", 0);
    case 'c':
        plotter.log("float", tri_cost, number_of_t, "c", 0);
        plotter.log("float", tri_cost_p, number_of_t, "c'", 0);
        int t[3];
        for (int i = 0; i < number_of_v; i++){
            for (int j = i+1; j < number_of_v; j++){
                for (int k = j+1; k < number_of_v; k++){
                    t[0] = i;t[1] = j;t[2] = k;
                    std::cout<<get_n_er_relation_id(t, 3)<<" ";
                }
            }
        }
        std::cout<<" \n"<<std::flush;
        break;
    default:
        plotter.print_to_qtTextField();
    }
}
//************************************************************
//    Move an Element to Group U
//    Make sure group Number tight  {0,1,4,2}-->{0,1,3,2}
//    Tight: (if 0 < n < |U|, n in pi)
//************************************************************
int* Graph::move(int a,int U, int* pi){
    int *after;
    int max = 0, u0 = pi[a];
    bool empty_group = true;
    after =  (int*)malloc(sizeof(int) * number_of_v);
    for(int i = 0; i < number_of_v; i++){
        after[i] = pi [i];
        if (max < pi[i]){
            max = pi[i];
        }
//------The New Empty Group May Be The Largest Group id-----//
        if (max < U){
            max = U;
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


//************************************************************
//      Get posotion of the bivertex relation form two nodes
//      TODO:  extend to TRIVERTEX relation
//       e |  1, 2, 3, 4, 5
//      ---|-----------------
//      1  |  0  1  2  3  4
//      2  |     5  6  7  8
//      3  |        9  10 11
//      4  |           12 13
//      5  |              14
//      ISSUE:   (1,1)(2,2) should be invilid....
//************************************************************

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

//*****************************************************************************************************
//      Get posotion of the bivertex/trivertex relation form two nodes
//      TODO:  extend to ANY-VERTEX relation
//         |  1                 //         |  2                   //       |  3
//       T |  1, 2, 3, 4, 5     //       T |  1, 2, 3, 4, 5       //     T |  1, 2, 3, 4, 5
//      ---|-------------------------------|----------------------//-------|------------------
//      1  |                    //      1  |                      //    1  |
//      2  |        1  2  3     //      2  |                      //    2  |
//      3  |           4  5     //      3  |           7  8       //    3  |
//      4  |              6     //      4  |              9       //    4  |              10
//      5  |                    //      5  |                      //    5  |
//*****************************************************************************************************
int Graph::get_n_er_relation_id(int* start, int n_er_relation){
    std::vector<int> v ;            //  Use sortfor vectors.
    for(int i = 0; i < n_er_relation; i++){
        v.push_back(start[i]);
    }
    std::sort(v.begin(), v.end(), std::less<int>());

    int id = 0;

    //******************************************************************************
    //    Use ineffecttive and only for cube version (e = |v|3)
    //    TODO: use compact version(uncomment and verify)
    //    TODO: id calculation for any n point relation.
    //******************************************************************************

    if(n_er_relation == 3){
        int a = v.data()[0]+1, b = v.data()[1]+1 ,c = v.data()[2]+1;
        assert(a < b && b < c);
        id += number_of_v * (number_of_v-1) * (number_of_v-2) / 6
                - (number_of_v+1-a) * (number_of_v-a) * (number_of_v-1-a) / 6;
        id += (2*number_of_v - a - b) * (b - a - 1) / 2;
        id += c - b + 1;
        id += -1;
    }else if (n_er_relation == 2)
        id = get_edge_id(v.data()[0], v.data()[1]);
    return id;
}

//******************************************************************************
//    (6, 19)
//    phi of y = <c, y_of_pi>
//    c  (a, a') = <phi, x[get_edge_id(a,a')]>
//******************************************************************************
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

//************************************************************
//    1.Copy the origin partition. Remember to FREE it.
//    2.Change the gruop-id to the new one.
//    3.Reassign group-id if the last element deleted.
//************************************************************

int* Graph::greedy_move(int* pi){
    int best_a=-1, best_U=-1, gruop = 0;
    int *min_pi, *new_pi;
    float phi_y_pi = phi(pi), new_phi, min_diff = 0;
    gruop = num_of_gruops(pi);                                       //num_of_gruops = 1 for initial pi {0,0,0,0,0}

    for(int i = 0; i < number_of_v; i++){
        for(int j = 0; j < gruop+1; j++){
            new_pi = move(i, j, pi);
            new_phi = phi(new_pi);
            if(min_diff < phi_y_pi - new_phi){
                best_a = i;
                best_U = j;
                min_diff = phi_y_pi - new_phi;
            }

            plotter.log("diff", new_pi, number_of_v, "", phi_y_pi - new_phi);
            free(new_pi);
        }
    }

    if(min_diff > 0 && best_U >=0){
        min_pi = move(best_a, best_U, pi);
        plotter.log("diff", min_pi, number_of_v, "MIN_PI TO FIND NEXT MOVE", min_diff);
        free(pi);
        return greedy_move(min_pi);
    }else{
        plotter.log("diff", pi, number_of_v, "FINAL PARTITION", min_diff);
        plotter.print_to_cout();
        return pi;
    }
}

int* Graph::kl(int* pi){


    //*****************************************************************************
    //    Initialization
    //    There are Total |V| + 1  PIs and Therefore |V| + 1 PHIs  (including initial state)
    //    There are Total |V| DELTAs between them and |V| As
    //*****************************************************************************
    float min_delta = 0, *delta_seq = new float[number_of_v],
            *phi_seq = new float[number_of_v + 1];
    int *min_pi = 0, *a = new int[number_of_v];
    int **pi_seq = new int*[number_of_v + 1];
    for(int i = 0; i < number_of_v; i++){               //initialize to {1,2,3,4,5}
        a[i] = i;
        delta_seq[i] = 0;                                   //initialize to 0 ????
    }
    pi_seq[0] = pi;

    plotter.log("int", pi_seq[0], number_of_v, "PI - Sequence", 0);
    //*****************************************************************************
    //    Main Loop
    //*****************************************************************************

    int *new_pi, total_groups, best_a = -1, best_U = -1;
    float new_phi, new_delta;
    for(int i = 0; i < number_of_v; i++){
    //------------The rest initialization will be done until first delta is calculated------//
        best_a = -1;

        total_groups = num_of_gruops(pi_seq[i]);
        phi_seq[i] = phi(pi_seq[i]);
        for(int j = 0; j < number_of_v; j++){
            //skip moved items;
            while(a[j] < 0){
                j++;
            }
            for(int k = 0; k < total_groups + 1; k++){
                //*****************************************************************************
                //    Prevent Fake Move:  Move that dont REALLY change of the partition
                //                          1. Move to the same group that it comes from.
                //                          2. Move from single element set to empty set.
                //                  ->lead to trapped in to local-opt ->lost kl tech advantages
                //*****************************************************************************
                if(pi_seq[i][j] == k){
                    continue;
                }
                if(k == total_groups){
                    bool best_a_is_solo = true;
                    for(int l = 0; l < number_of_v; l++){
                        if(pi_seq[i][j] == pi_seq[i][l] && j!=l){
                             best_a_is_solo = false;
                        }
                    }
                    if(best_a_is_solo){
                        continue;
                    }
                }


                new_pi = move(j, k, pi_seq[i]);
                new_phi = phi(new_pi);
                new_delta = new_phi - phi_seq[i];

                if(new_delta < min_delta || best_a < 0){
                    best_a = j;
                    best_U = k;
                    min_delta = new_delta;
                    plotter.log("diff", new_pi, number_of_v, "Better Move Found", min_delta);
                }
                free(new_pi);
            }
        }
        if(best_a<0){exit(1);}          //assert(best_a>=0);
        //-----------Update Parameters of t+1-------------------//
        pi_seq[i+1] = move(best_a, best_U, pi_seq[i]);
        delta_seq[i] = min_delta;
        a[i] = -1;
        plotter.log("diff", pi_seq[i+1], number_of_v, "Best Move", min_delta);
    }

    //************************************************************
    //                                        ^
    //    Select the Maximum-gain Subsequence t
    //************************************************************

    min_delta = 0;
    min_pi = pi_seq[0];
    for(int i = 0; i < number_of_v - 1; i++){
        delta_seq[i+1] = delta_seq[i+1] + delta_seq[i];
    }
    for(int i = 0; i < number_of_v; i++){
        if(delta_seq[i] < min_delta){
            min_delta = delta_seq[i];
            free(min_pi);
            min_pi = pi_seq[i+1];
        }else{
            free(pi_seq[i+1]);              //Free number_of_v times in total
        }
    }

    plotter.log("float", delta_seq, number_of_v, "Delta_seq", 0);
    delete[] pi_seq;
    delete[] delta_seq;
    delete[] a;


    plotter.log("diff", min_pi, number_of_v, "Best Move Of The Sequence", min_delta);
    if(min_delta < 0){
        return kl(min_pi);
    }else{
        plotter.log("diff", min_pi, number_of_v, "Final Selected Partition", min_delta);
        plotter.print_to_cout();
        return min_pi;
    }
}

float Graph::cost_generator(float *p1, float *p2, float *p3){
    float x_bar = (p1[0] + p2[0] + p3[0]) /3;
    float y_bar = (p1[1] + p2[1] + p3[1]) /3;
    float z_bar = (p1[2] + p2[2] + p3[2]) /3;
    if(-0.001 < x_bar < 0.001){
        x_bar = 0.001;
    }
    float a = (y_bar + z_bar)/x_bar;
    float d = abs(a * p1[0] + p1[1] + p1[2]) +
            abs(a * p2[0] + p2[1] + p2[2]) +
            abs(a * p3[0] + p3[1] + p3[2]) /sqrt(a*a+1+1);
    return d;
}

float Graph::cube_phi(int *pi){
    assert(number_of_v>3);
    float phi = 0;
    for(int i = 0; i < number_of_v; i++){
        for(int j = i+1; j < number_of_v; j++){
            for(int k = j+1; k < number_of_v; k++){
                int loc[3] = {i,j,k};
                if(pi[i] == pi[j] && pi[k] == pi[j]){
                    phi += tri_cost[get_n_er_relation_id(loc,3)];
                }else if(pi[i] != pi[j] && pi[j] != pi[k] && pi[k] != pi[i]){
                    phi += tri_cost_p[get_n_er_relation_id(loc,3)];
                }
            }
        }
    }
    return phi;
}

float Graph::cube_phi_d(int *pi){
    assert(number_of_v>3);
    float phi = 0;
    for(int i = 0; i < number_of_v; i++){
        for(int j = i+1; j < number_of_v; j++){
            for(int k = j+1; k < number_of_v; k++){
                if(pi[i] == pi[j] && pi[k] == pi[j]){
                    phi += cost_generator(point_location+i*3,point_location+j*3,point_location+k*3);
                }
            }
        }
    }
    return phi;
}
float Graph::cube_phi_test(int *pi){
    assert(number_of_v>3);
    float phi = 0;
    for(int i = 0; i < number_of_v; i++){
        for(int j = i+1; j < number_of_v; j++){
            for(int k = j+1; k < number_of_v; k++){
                std::cout<<"i: "<<i
                         <<"  j: "<<j
                         <<"  k: "<<k
                         <<"  cost: "
                         <<cost_generator(point_location+i*3,point_location+j*3,point_location+k*3)
                         <<"\n";

            }
        }
    }
    return phi;
}


int* Graph::cube_clustering(){
    int ttl = 5;
    pi0();
    number_of_e = number_of_v *number_of_v *number_of_v; //Compact version in TODO list;

    float min_delta = -1, new_phi, new_delta,
            *delta_seq,
            *phi_seq;
    int best_phi = 0, best_a = -1, best_U = -1, total_groups = 1;
    int *min_pi,
        *new_pi,
        *a;
    int **pi_seq;

    delta_seq = (float*)malloc (number_of_v * sizeof (float));
    phi_seq = (float*)malloc ((number_of_v+1) * sizeof (float));
    a = (int*)malloc (number_of_v * sizeof (int));
    pi_seq = (int**)malloc ((number_of_v+1) * sizeof (int*));

    do{
        best_phi = 0;

        for(int i = 0; i < number_of_v; i++){               //initialize to {1,2,3,4,5}
            a[i] = i;
            delta_seq[i] = 0;                                   //initialize to 0 ????
        }
        pi_seq[0] = p;

        for(int i = 0; i < number_of_v; i++){
            //sequence length -nov unmoved items to start
            //------------The rest initialization will be done until first delta is calculated------//
            best_a = -1;

            total_groups = num_of_gruops(pi_seq[i]);
            phi_seq[i] = cube_phi(pi_seq[i]);
            for(int j = 0; j < number_of_v; j++){
                //Try to move every items in V
                while(a[j] < 0){
                    j++;
                }

                for(int k = 0; k < number_of_v; k++){
                    //Try to move to |U|+1 groups
                    //*****************************************************************************
                    //    Prevent Fake Move:  Move that dont REALLY change of the partition
                    //                          1. Move to the same group that it comes from.
                    //                          2. Move from single element set to empty set.
                    //                  ->lead to trapped in to local-opt ->lost kl tech advantages
                    //*****************************************************************************
                    if(pi_seq[i][j] == k){
                        continue;
                    }
                    if(k == total_groups){
                        bool best_a_is_solo = true;
                        for(int l = 0; l < number_of_v; l++){
                            if(pi_seq[i][j] == pi_seq[i][l] && j!=l){
                                 best_a_is_solo = false;
                            }
                        }
                        if(best_a_is_solo){
                            continue;
                        }
                    }

                    new_pi = move(j, k, pi_seq[i]);
                    new_phi = cube_phi(new_pi);
                    new_delta = new_phi - phi_seq[i];

                    if(new_delta < min_delta || best_a < 0){
                        best_a = j;
                        best_U = k;
                        min_delta = new_delta;
                        //std::cout<<"a:"<<best_a<<"u:"<<best_U;
                        plotter.log("diff", new_pi, number_of_v, "Better Move Found", min_delta);
                    }
                    free(new_pi);
                }
            }
            if(best_a<0){exit(1);}          //assert(best_a>=0);
            //-----------Update Parameters of t+1-------------------//
            pi_seq[i+1] = move(best_a, best_U, pi_seq[i]);
            delta_seq[i] = min_delta;
            a[i] = -1;
        }


        //************************************************************
        //                                        ^
        //    Select the Maximum-gain Subsequence t
        //************************************************************

        min_delta = 0;
        min_pi = pi_seq[0];
        for(int i = 0; i < number_of_v - 1; i++){
            delta_seq[i+1] = delta_seq[i+1] + delta_seq[i];
        }
        for(int i = 0; i < number_of_v; i++){
            if(delta_seq[i] < min_delta){
                min_delta = delta_seq[i];
                min_pi = pi_seq[i+1];
            }else{
                free(pi_seq[i+1]);
            }
        }

        plotter.log("float", delta_seq, number_of_v, "Delta_seq", 0);
        plotter.log("diff", min_pi, number_of_v, "Best Move Of The Sequence", min_delta);
        if(min_delta >= 0){
            plotter.log("diff", min_pi, number_of_v, "Final Selected Partition", min_delta);
        }else{
            free(p);
            p = min_pi;
        }
    }while(min_delta < 0 && ttl--);


    free(pi_seq);
    free(delta_seq);
    free(phi_seq);
    free(a);
    plotter.print_to_cout();
    return p;
}






int* Graph::cube_clustering_d(){
    plotter.log("float", point_location, number_of_v*3, "Point-Location", 0);
    int ttl = 100;
    //TODO: remove hard coding group number
    int groups = 4;
    pi0();
    number_of_e = number_of_v *number_of_v *number_of_v; //Compact version in TODO list;

    float min_delta = -1, new_phi, new_delta,
            *delta_seq,
            *phi_seq;
    int best_phi = 0, best_a = -1, best_U = -1, total_groups = 1;
    int *min_pi,
        *new_pi,
        *a;
    int **pi_seq;

    delta_seq = (float*)malloc (number_of_v * sizeof (float));
    phi_seq = (float*)malloc ((number_of_v+1) * sizeof (float));
    a = (int*)malloc (number_of_v * sizeof (int));
    pi_seq = (int**)malloc ((number_of_v+1) * sizeof (int*));

    do{
        best_phi = 0;

        for(int i = 0; i < number_of_v; i++){               //initialize to {1,2,3,4,5}
            a[i] = i;
            delta_seq[i] = 0;                                   //initialize to 0 ????
        }
        pi_seq[0] = p;

        for(int i = 0; i < number_of_v; i++){
            //sequence length -nov unmoved items to start
            //------------The rest initialization will be done until first delta is calculated------//
            best_a = -1;

            total_groups = num_of_gruops(pi_seq[i]);
            phi_seq[i] = cube_phi_d(pi_seq[i]);
            for(int j = 0; j < number_of_v; j++){
                //Try to move every items in V
                while(a[j] < 0){
                    j++;
                }

                for(int k = 0; k < number_of_v; k++){
                    //Try to move to |U|+1 groups
                    //*****************************************************************************
                    //    Prevent Fake Move:  Move that dont REALLY change of the partition
                    //                          1. Move to the same group that it comes from.
                    //                          2. Move from single element set to empty set.
                    //                  ->lead to trapped in to local-opt ->lost kl tech advantages
                    //*****************************************************************************
                    if(pi_seq[i][j] == k){
                        continue;
                    }
                    if(k == total_groups){
                        bool best_a_is_solo = true;
                        for(int l = 0; l < number_of_v; l++){
                            if(pi_seq[i][j] == pi_seq[i][l] && j!=l){
                                 best_a_is_solo = false;
                            }
                        }
                        if(best_a_is_solo){
                            continue;
                        }
                    }

                    new_pi = move(j, k, pi_seq[i]);
                    new_phi = cube_phi_d(new_pi);
                    new_delta = new_phi - phi_seq[i];

                    if((new_delta < min_delta && num_of_gruops(new_pi)<=groups) || best_a < 0){
                        best_a = j;
                        best_U = k;
                        min_delta = new_delta;
                        plotter.log("diff", new_pi, number_of_v, "Better Move Found", min_delta);
                    }
                    free(new_pi);
                }
            }
            if(best_a<0){exit(1);}          //assert(best_a>=0);
            //-----------Update Parameters of t+1-------------------//
            pi_seq[i+1] = move(best_a, best_U, pi_seq[i]);
            delta_seq[i] = min_delta;
            a[i] = -1;
        }


        //************************************************************
        //                                        ^
        //    Select the Maximum-gain Subsequence t
        //************************************************************

        min_delta = 0;
        min_pi = pi_seq[0];
        for(int i = 0; i < number_of_v - 1; i++){
            delta_seq[i+1] = delta_seq[i+1] + delta_seq[i];
        }
        for(int i = 0; i < number_of_v; i++){
            if(delta_seq[i] < min_delta){
                min_delta = delta_seq[i];
                min_pi = pi_seq[i+1];
            }else{
                free(pi_seq[i+1]);
            }
        }

        plotter.log("float", delta_seq, number_of_v, "Delta_seq", 0);
        plotter.log("diff", min_pi, number_of_v, "Best Move Of The Sequence", min_delta);
        if(min_delta >= 0){
            plotter.log("diff", min_pi, number_of_v, "Final Selected Partition", min_delta);
        }else{
            free(p);
            p = min_pi;
        }
    }while(min_delta < 0 && ttl--);


    free(pi_seq);
    free(delta_seq);
    free(phi_seq);
    free(a);
    plotter.print_to_cout();

    plotter.log("points", point_location, 20, "", 0);
    plotter.log("pi", p, 20, "", 0);
    plotter.print_to_qtTextField();
    return p;
}


















