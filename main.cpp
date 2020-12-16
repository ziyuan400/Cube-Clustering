#include "mainwindow.h"
#include "graph.h"

#include <QApplication>


void greedy_move();
void greedy_move_with_kl_technic();
void cubic_clustering();

int main(int argc, char *argv[])
{
    Graph graph = Graph(7);
    graph.input_c_cp(0,0);
    graph.print('c');

    graph.cube_clustering();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
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
