#include <iostream>// cout, endl
#include <sstream>
#include <cassert> // assert()
#include <algorithm> // copy, sort
#include <iterator> // begin(), end().
#include <vector>

#include "../include/disjSets.h"

//#define TopWall 0x01; //norte 
//#define RightWall 0x02; //leste
//#define BottomWall 0x04; //sul
//#define LeftWall 0x08; //oeste

using namespace std;
void verificarParedeIntacta(int maze[], int posicao, int parede);
void derrubarParece(int maze[], int posicao, int parede);

int main( )
{
    auto n_unit{0}; // unit test count.

    int TopWall = 0x01; //norte 
    int RightWall = 0x02; //leste
    int BottomWall = 0x04; //sul
    int LeftWall = 0x08; //oeste

    int nColunas = 40;
    int nLinhas = 66;
    int tamanhoTotal = nColunas * nLinhas;

    int Maze[ tamanhoTotal ];
    for (int i=0; i < (tamanhoTotal); i++ )
    {
        if((i % nColunas) == 0 && i > 0)
            cout << endl;

        Maze[ i ] = TopWall | RightWall | BottomWall | LeftWall;
        cout << Maze[i] << "|";        
    }


    //=== Unit tests start here
    {
        cout << endl;
        cout << ">>> Unit teste #" << ++n_unit << ": empty.\n";

        // Create a empty PQ.
        //PQ<int, decltype(compare) > h( compare );

        //assert( h.size() == 0 );
        //assert( h.empty() == true );
        //h.push(1);
        //assert( h.empty() == false );

        //std::cout << ">>> Passed!\n\n";

        verificarParedeIntacta(Maze, 15, BottomWall);
        derrubarParece(Maze, 15, BottomWall);
        verificarParedeIntacta(Maze, 15, BottomWall);
    }
    
    return 0;
}

void verificarParedeIntacta(int maze[], int posicao, int parede){
    if ( maze[ posicao ] & parede )  // ou if ( Maze[ 13 ] & BottomWall == BottomWall )
        cout << "Parede Sul da célula 13 está intacta!\n";
    else
        cout << "Parede Sul da célula 13 foi demolida!\n";
}

void derrubarParece(int maze[], int posicao, int parede){
    maze[ posicao ] = maze[ posicao ] & ~parede;
}