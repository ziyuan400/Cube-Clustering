#ifndef PLOTTER_H
#define PLOTTER_H

#include <iostream>

class Plotter
{
public:
    Plotter();
    void print_to_cout(std::string d_type, void *pointer, int lenth, std::string name, float diff);
    void print_to_cout(std::string d_type, void *pointer, int lenth, std::string name, int second_dim);
};

#endif // PLOTTER_H
