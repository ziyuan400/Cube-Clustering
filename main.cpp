#include "mainwindow.h"
#include "graph.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>


void greedy_move();
void greedy_move_with_kl_technic();
void cubic_clustering();
void data_generator(double *p, int planes, int points);

int main()
{
    double *p = new double[60];
    data_generator(p, 4, 5);
    Graph graph = Graph(20);
    graph.input_c_cp(0,0);
    graph.cube_clustering();
}



void data_generator(double *p, int planes, int points){
    double phi, costheta, theta, r, x, y, z;
    double x1, y1, z1;
    double x2, y2, z2;
    double a,b;

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
