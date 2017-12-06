#include <iostream>// cout, endl
#include <sstream>
#include <cassert> // assert()
#include <algorithm> // copy, sort
#include <iterator> // begin(), end().
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>

#include "../include/disjSets.h"

//#define TopWall 0x01; //norte 
//#define RightWall 0x02; //leste
//#define BottomWall 0x04; //sul
//#define LeftWall 0x08; //oeste

using namespace std;
bool verificarParedeIntacta(int maze[], int posicao, int parede);
void derrubarParede(int maze[], int posicao, int parede);
//void criarLabirinto(int maze[], int nColunas, int nLinhas, int TopWall, int RightWall, int BottomWall, int LeftWall, bool totalConexo);
void criarLabirinto(int maze[], int nColunas, int nLinhas, int paredes[], bool totalConexo);

int main( )
{
    auto n_unit{0}; // unit test count.

    int TopWall = 0x01; //norte 
    int RightWall = 0x02; //leste
    int BottomWall = 0x04; //sul
    int LeftWall = 0x08; //oeste

    int paredes[] = {TopWall, RightWall, BottomWall, LeftWall};

    int nColunas = 40;
    int nLinhas = 66;
    int tamanhoTotal = nColunas * nLinhas;

    int Maze[ tamanhoTotal ];
    criarLabirinto(Maze, nColunas, nLinhas, paredes, 1);
    


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
        derrubarParede(Maze, 15, BottomWall);
        verificarParedeIntacta(Maze, 15, BottomWall);
    }
    
    return 0;
}

bool verificarParedeIntacta(int maze[], int posicao, int parede){
    string nomeParede = "NomeParede";

    if ( maze[ posicao ] & parede )  // ou if ( Maze[ 13 ] & BottomWall == BottomWall )
    {
        cout << "Parede " << nomeParede << " da célula " << posicao << " está intacta!\n";
        return 1;
    }
    else
    {
        cout << "Parede " << nomeParede << " da célula " << posicao << " foi demolida!\n";
        return 0;
    }
}

void derrubarParede(int maze[], int posicao, int parede){
    maze[ posicao ] = maze[ posicao ] & ~parede;
}

void criarLabirinto(int maze[], int nColunas, int nLinhas, int paredes[], bool totalConexo){
    //int TopWall, int RightWall, int BottomWall, int LeftWall
    int tamanhoTotal = nColunas * nLinhas;

    //Criar os conjuntos
    DisjSets conjuntos(tamanhoTotal);

    //Criar o labirinto em si
    for (int i=0; i < (tamanhoTotal); i++ )
    {
        if((i % nColunas) == 0 && i > 0)
            cout << endl;

        //maze[ i ] = TopWall | RightWall | BottomWall | LeftWall;
        maze [ i ] = paredes[0] | paredes[1] | paredes[2] | paredes[3];
        cout << maze[i] << "|";        
    }

    //Criar caminho válido ou totalmente conexo

    //conjuntos.print();
    int posicaoInicial = 0;
    int posicaoFinal = tamanhoTotal - 1;
    int posicaoRand = 0;
    bool naoConcluido = true;
    int k = 0; //posicao auxiliar para definir parede
    int posicaoVizinho;

    
    while(naoConcluido){
        srand (time(0));
        posicaoRand = rand() % posicaoFinal + posicaoInicial;
        srand (time(0));
        k = rand() % 3 + 1; //entre 1 e 3, pois pode ser uma das 4 paredes. 0->Norte, 1->Leste, 2->Sul, 3->Oeste

        switch(k)
        {
            case 1: //norte
                posicaoVizinho = posicaoRand - nColunas;
                break;
            case 2: //leste
                posicaoVizinho = posicaoRand + 1;
                break;
            case 3: //sul
                posicaoVizinho = posicaoRand + nColunas;
                break;
            case 4: //oeste
                posicaoVizinho = posicaoRand - 1;
                break;
        }
        //se não estiver derrubada, derruba
        if(! verificarParedeIntacta(maze, posicaoRand, paredes[k]))
        {
            derrubarParede(maze, posicaoRand, paredes[k]);
            conjuntos.unionSets(posicaoRand, posicaoVizinho);
        }
        //testa se já concluiu de acordo com as condições: válido ou totalmente conexo
        

    }
    
    //conjuntos.s[]
}