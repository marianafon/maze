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
bool verificarParedeIntacta(int Maze[], int posicao, int parede);
void derrubarParede(int Maze[], int posicao, int parede, int vizinho);
void criarLabirinto(int Maze[], int nColunas, int nLinhas, int paredes[], bool totalConexo);
void imprimir(int Maze[], int tamanhoTotal, int nColunas, int paredes[]);
int buscarCelulaVizinhaParede(int posicao, int nColunas, int nLinhas, int parede);
void criarCaminho(int Maze[], int nColunas, int nLinhas, int paredes[], bool totalConexo, DisjSets &conjuntos);

int main( )
{
    auto n_unit{0}; // unit test count.

    int TopWall = 0x01; //norte 
    int RightWall = 0x02; //leste
    int BottomWall = 0x04; //sul
    int LeftWall = 0x08; //oeste

    int paredes[] = {TopWall, RightWall, BottomWall, LeftWall};

    int nColunas = 2;
    int nLinhas = 2;
    bool totalConexo = 1;
    int tamanhoTotal = nColunas * nLinhas;

    //Criar os conjuntos
    DisjSets conjuntos(tamanhoTotal);

    //Criar Labirinto
    int Maze[ tamanhoTotal ];
    criarLabirinto(Maze, nColunas, nLinhas, paredes, 1);    

    imprimir(Maze, tamanhoTotal, nColunas, paredes);

    //Derrubar entrada e saída
    derrubarParede(Maze, 0, LeftWall, buscarCelulaVizinhaParede(0, nColunas, nLinhas, LeftWall));
    derrubarParede(Maze, tamanhoTotal - 1, RightWall, buscarCelulaVizinhaParede(tamanhoTotal - 1, nColunas, nLinhas, RightWall));

    imprimir(Maze, tamanhoTotal, nColunas, paredes);

    //Encontrar caminho segundo parâmetro
    criarCaminho(Maze, nColunas, nLinhas, paredes, totalConexo, conjuntos);







    //testes
    //imprimir(Maze, tamanhoTotal, nColunas, paredes);
    //derrubarParede(Maze, 0, LeftWall);
    //cout << endl;
    //imprimir(Maze, tamanhoTotal, nColunas, paredes);


    //=== Unit tests start here
    /*
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

        cout << endl << "0 -> " << Maze[0];
        verificarParedeIntacta(Maze, 0, BottomWall);
        derrubarParede(Maze, 0, BottomWall);
        verificarParedeIntacta(Maze, 0, BottomWall);
        cout << endl << "0 -> Bottom " << Maze[0];
        derrubarParede(Maze, 0, RightWall);
        verificarParedeIntacta(Maze, 0, RightWall);
        cout << endl << "0 -> Right " << Maze[0];
    }
    */
    return 0;
}

bool verificarParedeIntacta(int Maze[], int posicao, int parede){
    string nomeParede = "NomeParede";

    if ( Maze[ posicao ] & parede )  // ou if ( Maze[ 13 ] & BottomWall == BottomWall )
    {
        //cout << "Parede " << nomeParede << " da célula " << posicao << " está intacta!\n";
        return 1;
    }
    else
    {
        //cout << "Parede " << nomeParede << " da célula " << posicao << " foi demolida!\n";
        return 0;
    }
}

int buscarParedeOposta(int parede)
{
    int paredeOposta = 0;

    switch(parede)
    {
        case 1: //norte
            paredeOposta = 4;
            break;
        case 2: //leste
            paredeOposta = 8;
            break;
        case 4: //sul
            paredeOposta = 1;
            break;
        case 8: //oeste
            paredeOposta = 2;
            break;
    }

    return paredeOposta;
}

void derrubarParede(int Maze[], int posicao, int parede, int vizinho){
    int paredeOposta = buscarParedeOposta(parede);

    //Derrubar própria parede
    Maze[ posicao ] = Maze[ posicao ] & ~parede;
    //Derrubar parede do vizinho
    Maze[ vizinho ] = Maze[ vizinho ] & ~paredeOposta;

    cout << "Parede " << parede << " da posicao " << posicao << " derrubada!" << endl;
}

int buscarCelulaVizinhaParede(int posicao, int nColunas, int nLinhas, int parede)
{
    //k é a posicao do vetor de paredes
    int celulaVizinha = 0;

    if(((posicao+1) % nColunas == 0) && (parede == 2)) //parede leste de uma borda direita
    {
        return -1;
    }

    if(((posicao) % nColunas == 0) && (parede == 8)) //parede oeste de uma borda esquerda
    {
        return -1;
    }

    cout << "Parede " << parede << " de " << posicao << ": ";
    switch(parede)
    {
        case 1: //norte
            celulaVizinha = posicao - nColunas;
            cout << "Norte : " << celulaVizinha;
            break;
        case 2: //leste
            celulaVizinha = posicao + 1;
            cout << "Leste : " << celulaVizinha;
            break;
        case 4: //sul
            celulaVizinha = posicao + nColunas;
            cout << "Sul : " << celulaVizinha;
            break;
        case 8: //oeste
            celulaVizinha = posicao - 1;
            cout << "Oeste : " << celulaVizinha;
            break;
    }

    cout << endl;

    if(celulaVizinha > 0 && celulaVizinha < (nColunas * nLinhas)){
        return celulaVizinha;
    }else{
        return -1;  
    }
}

/*
void imprimirDesenho(int Maze[], int tamanhoTotal, int nColunas, int paredes[])
{
    for (int i=0; i < (tamanhoTotal); i++ )
    {
        if((i % nColunas) == 0 && i > 0)
            cout << endl;

        //cout << Maze[i] << "|";        

        switch(Maze[i])
        {
            case paredes[0]: //norte
                break;
        }
    }

    cout << endl << endl;   
}
*/

void imprimir(int Maze[], int tamanhoTotal, int nColunas, int paredes[])
{
    for (int i=0; i < (tamanhoTotal); i++ )
    {
        if((i % nColunas) == 0 && i > 0)
            cout << endl;

        //Maze[ i ] = TopWall | RightWall | BottomWall | LeftWall;
        cout << Maze[i] << "|";        
    }

    cout << endl << endl;
}

void criarLabirinto(int Maze[], int nColunas, int nLinhas, int paredes[], bool totalConexo){
    //int TopWall, int RightWall, int BottomWall, int LeftWall
    int tamanhoTotal = nColunas * nLinhas;

    //Criar o labirinto em si
    for (int i=0; i < (tamanhoTotal); i++ )
    {
        //if((i % nColunas) == 0 && i > 0)
        //   cout << endl;

        //Maze[ i ] = TopWall | RightWall | BottomWall | LeftWall;
        Maze [ i ] = paredes[0] | paredes[1] | paredes[2] | paredes[3];
        //cout << Maze[i] << "|";        
    }
    //conjuntos.s[]
}

void criarCaminho(int Maze[], int nColunas, int nLinhas, int paredes[], bool totalConexo, DisjSets &conjuntos)
{
    int tamanhoTotal = nColunas * nLinhas;
    //Criar caminho válido ou totalmente conexo

    //conjuntos.print();
    int posicaoInicial = 0;
    int posicaoFinal = tamanhoTotal - 1;
    int celulaRand = 0;
    bool concluido = 0;
    int k = 0; //posicao auxiliar para definir parede
    int celulaVizinha;

    srand (time(0));

    while(!concluido){
        celulaRand = rand() % posicaoFinal + posicaoInicial; //numero aleatorio entre posicao inicial e final
        //srand (time(0));
        k = rand() % 3 + 0; //entre 0 e 3, pois pode ser uma das 4 paredes. 0->Norte, 1->Leste, 2->Sul, 3->Oeste

        cout << "Celula rand: " << celulaRand << " k rand: " << k << " posicao rand: " << paredes[k] << endl;

        celulaVizinha = buscarCelulaVizinhaParede(celulaRand, nColunas, nLinhas, paredes[k]); //
        bool dentro = 0;
        if(celulaVizinha != -1){        
            //cout << endl << "Verificando " << celulaRand << "k " << k << " parede " << paredes[k] << " cel " << celulaVizinha << endl;
    
            //se estiver intacta, derruba
            if(verificarParedeIntacta(Maze, celulaRand, paredes[k]))
            {
                derrubarParede(Maze, celulaRand, paredes[k], celulaVizinha);
                conjuntos.unionSets(celulaRand, celulaVizinha);
            }
            //testa se já concluiu de acordo com as condições: válido ou totalmente conexo
            //Totalmente conexo
            if(totalConexo && conjuntos.size() == 1)
            {
                concluido = 1;
            }
            //Depois: Válido
    
            conjuntos.print();
            cout << endl;
            imprimir(Maze, tamanhoTotal, nColunas, paredes);
            dentro = 1;
        }

        int qualquerCoisa = 0;
        //cout << "Dentro : " << dentro;
        //cin >> qualquerCoisa;

    }

    cout << "CONCLUIDO" << endl;
    
    imprimir(Maze, tamanhoTotal, nColunas, paredes);
}