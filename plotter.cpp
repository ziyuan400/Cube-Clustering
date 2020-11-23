#include "plotter.h"
#include "stdio.h"

Plotter::Plotter()
{

}

void Plotter::print_to_cout(std::string d_type, void *pointer, int lenth, std::string name, float diff){
    if(name.compare("")!=0){
        std::cout << "**************************";
        std::cout << name;
        std::cout << "**************************\n";
    }

    //******************************************************************************
    //    Print Pi and the diffrence between phi(Pi) and a given min value
    //******************************************************************************

    if(d_type.compare("diff") == 0){

        std::cout << "After Move: ";
        int *pi = (int*)pointer;
        std::cout << "{" ;
        for(int i = 0; i < lenth-1; i++){
            std::cout << pi[i];
            std::cout << ", " ;
        }
        std::cout << pi[lenth-1];
        std::cout << "} Diffrence ---->" ;
        std::cout << diff;
    }

    //***************************************
    //   Default
    //***************************************
    else{
        std::cout << "diff Print UNSUCCESSFUL: print option ";
        std::cout << name;
        std::cout << "not found\n";
    }
    std::cout <<("\n");
    std::cout<<std::flush;

    if(name.compare("")!=0){
        std::cout << "\n\n " ;
    }

}


void Plotter::print_to_cout(std::string d_type, void *pointer, int lenth, std::string name, int second_dim){
    if(name.compare("")!=0){                                            //Seprater if needed
        std::cout << "**************************";
        std::cout << name;
        std::cout << "**************************\n";
    }
    //***************************************
    //    Print A Set Of Int Array
    //***************************************
    if(d_type.compare("int") == 0){
        int* p = (int*)pointer;
        std::cout << name;
        std::cout << ": \n{";
        for(int i = 0; i < lenth-1; i++){
            std::cout << p[i];
            std::cout << ", " ;
        }
        std::cout << p[lenth-1];
        std::cout << "}" ;
    }

    //***************************************
    //    Print A Set Of Float Array
    //***************************************

    else if(d_type.compare("float") == 0){
        float *theta = (float*)pointer;
        std::cout << name;
        std::cout << ": \n";
        for(int i = 0; i < lenth-1; i++){
            std::cout << theta[i];
            std::cout << " << " ;
        }
        std::cout << theta[lenth-1];
    }

    //***************************************
    //    Print A 2D Float Array for x
    //***************************************

    else if(d_type.compare("float2D") == 0){
        float **x = (float**)pointer;
        std::cout << name;
        std::cout << ": \n";

        for(int i = 0; i < lenth; i++){
            std::cout <<(i);
            std::cout <<(" : ");
            for(int j = 0; j < second_dim -1; j++){
                std::cout <<(x[i][j]);
                std::cout <<(" << ");
            }
            std::cout <<(x[i][second_dim-1]);
            std::cout <<("\n");
        }
    }




    //***************************************
    //   Default
    //***************************************
    else{
        std::cout << "Print UNSUCCESSFUL: print option ";
        std::cout << name;
        std::cout << "not found\n";
    }
    std::cout <<("\n");
    std::cout<<std::flush;
}
