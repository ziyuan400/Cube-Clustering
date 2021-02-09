#include "mainwindow.h"
#include "graph.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <chrono>  // for high_resolution_clock


int PLANES = 4;
int POINTS = 25;
void greedy_move();
void greedy_move_with_kl_technic();
void cubic_clustering();
void data_generator(float *p, int planes, int points);
double cost_generator(float *p);
int* data_shuffler(float *p, int planes, int points);

int main()
{
    // Record start time
    auto start = std::chrono::high_resolution_clock::now();

    // Generate Data
    float *p = new float[PLANES * POINTS*3];
    data_generator(p, PLANES, POINTS);
    int* gt =  data_shuffler(p, PLANES, POINTS);


    // Compute Cluster
    Graph graph = Graph(PLANES * POINTS);
    graph.input_points(p);
    graph.cube_clustering_d();

    // Record end time
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";

    // Visualization
    graph.addPlotInfo("gt", gt);
    graph.evaluation(gt);
}

int* data_shuffler(float *p, int planes, int points){
    float *shuffledP = new float[PLANES * POINTS*3];
    std::vector<int> order(PLANES * POINTS);
    std::iota (std::begin(order), std::end(order), 0);
    shuffle(order.begin(), order.end(), std::default_random_engine(unsigned(time(NULL))));
    int* pi = new int[PLANES * POINTS];
    int* shuffledPi = new int[PLANES * POINTS];
    for(int i = 0; i < planes; i++){
        for(int j = 0; j < points; j++){
            pi[i * POINTS + j] = i;
        }
    }
    for(int i = 0; i < PLANES * POINTS; i++){
        shuffledPi[i] = pi[order.at(i)];
        shuffledP[i * 3 + 0] = p[order.at(i) * 3 + 0];
        shuffledP[i * 3 + 1] = p[order.at(i) * 3 + 1];
        shuffledP[i * 3 + 2] = p[order.at(i) * 3 + 2];
    }
    for(int i = 0; i < PLANES * POINTS; i++){
        shuffledPi[i] = pi[order.at(i)];
        p[i * 3 + 0] = shuffledP[i * 3 + 0];
        p[i * 3 + 1] = shuffledP[i * 3 + 1];
        p[i * 3 + 2] = shuffledP[i * 3 + 2];
    }
    return shuffledPi;
}

void data_generator(float *p, int planes, int points){
    float a, b, c, r;
    float x,y;
    srand(unsigned(time(NULL)));
    for(int i = 0; i < planes; i++){
        a = ((double)rand() / RAND_MAX) * 2.0  -1;
        b =((double)rand() / RAND_MAX) * 2.0  -1;
        c = ((double)rand() / RAND_MAX) * 2.0  -1;
        r = sqrt(2+(abs(a)+abs(b))/c*(abs(a)+abs(b))/c);

        for(int j = 0; j < points; j++){
            x = ((double)rand() / RAND_MAX) * 2.0  -1;
            y = ((double)rand() / RAND_MAX) * 2.0  -1;
            p[i*3*points + j*3    ] = x/r;
            p[i*3*points + j*3 + 1] = y/r;
            p[i*3*points + j*3 + 2] = -(a*x + b*y)/c/r;
        }
    }
}

double cost_generator(double *p){
    double x_bar = (p[0] + p[3] + p[6]) /3;
    double y_bar = (p[1] + p[4] + p[7]) /3;
    double z_bar = (p[2] + p[5] + p[8]) /3;
    if(-0.001 < x_bar < 0.001){
        x_bar = 0.001;
    }
    double a = (y_bar + z_bar)/x_bar;
    double d = abs(a * p[0] + p[1] + p[2]) +
            abs(a * p[3] + p[4] + p[5]) +
            abs(a * p[6] + p[7] + p[8]) /sqrt(a*a+1+1);

    return d;
}

void greedy_move(){
    Graph graph = Graph(5);
    graph.input_theta(0);
    graph.input_x(0);
    graph.print('x');
    graph.greedy_move(graph.pi0());
}

void greedy_move_with_kl_technic(){
    Graph graph = Graph(5);
    graph.input_theta(0);
    graph.input_x(0);
    graph.print('x');
    graph.kl(graph.pi0());
}

void cubic_clustering(){
    Graph graph = Graph(5);
    graph.input_c_cp(0,0);
    graph.print('c');

    graph.cube_clustering();
}

void data_generator_no_origin(float *p, int planes, int points){
        float phi, costheta, theta, r, x, y, z;
        float x1, y1, z1;
        float x2, y2, z2;
        float a,b;
        for(int i = 0; i < planes; i++){
            phi = rand() * 1.0 / RAND_MAX * M_PI;                 //random(0, 2*pi)
            costheta = (rand() * 1.0 / RAND_MAX - 0.5) * 2;             //random(-1, 1)
            theta = acos( costheta );
            x = sin( theta) * cos( phi );
            y = sin( theta) * sin( phi );
            z = cos( theta );

            x1=1;           //assume z!=0
            y1=1;
            z1=-x-y;

            x2 = y * z1 - z * y1;          //cross product
            y2 = z * x1 - x * z1;
            z2 = x * y1 - y * x1;

            r = sqrt((x1*x1)+(y1*y1)+(z1*z1));
            x1=x1/r;
            y1=y1/r;
            z1=z1/r;

            r = sqrt((x2*x2)+(y2*y2)+(z2*z2));
            x2=x2/r;
            y2=y2/r;
            z2=z2/r;


            for(int j = 0; j < points; j++){
                a = rand() * 1.0 / RAND_MAX;
                b = rand() * 1.0 / RAND_MAX;
                p[i*3*points + j*3    ] = x1 * a + x2 * b;
                p[i*3*points + j*3 + 1] = y1 * a + y2 * b;
                p[i*3*points + j*3 + 2] = z1 * a + z2 * b;

            }

        }

}
