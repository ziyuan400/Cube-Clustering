#include "mainwindow.h"
#include "graph.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Graph graph = Graph(5);
    graph.input_theta(0);
    graph.input_x(0);
    graph.print('x');
    graph.greedy_move(graph.pi0());

    return a.exec();
}
