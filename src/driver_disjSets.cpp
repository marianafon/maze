#include <iostream>// cout, endl
#include <sstream>
#include <cassert> // assert()
#include <algorithm> // copy, sort
#include <iterator> // begin(), end().
#include <vector>
#include <string>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

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
void criarCaminho(int Maze[], int nColunas, int nLinhas, int paredes[], bool totalConexo, DisjSets &conjuntos, string MazeSolution[]);
void imprimirDesenho(int Maze[], int tamanhoTotal, int nColunas, int paredes[], int nLinhas, string MazeSolution[]);
bool encontrarSolucao(int Maze[], int nColunas, int nLinhas, int paredes[], string MazeSolution[]);
void iinicializarMazeSolution(string MazeSolution[], int tamanhoTotal);

int main( )
{
    auto n_unit{0}; // unit test count.

    int TopWall = 0x01; //norte 
    int RightWall = 0x02; //leste
    int BottomWall = 0x04; //sul
    int LeftWall = 0x08; //oeste

    int paredes[] = {TopWall, RightWall, BottomWall, LeftWall};

    int nColunas = 10;
    int nLinhas = 10;
    bool totalConexo = 0;
    int tamanhoTotal = nColunas * nLinhas;

    //Criar os conjuntos
    cout << "Criando conjuntos..." << endl;
    DisjSets conjuntos(tamanhoTotal);

    //Criar vetor de solução
    string MazeSolution[ tamanhoTotal ];

    //Inicializar solution
    iinicializarMazeSolution(MazeSolution, tamanhoTotal);

    //Criar Labirinto
    cout << "Criando labirinto..." << endl;
    int Maze[ tamanhoTotal ];
    criarLabirinto(Maze, nColunas, nLinhas, paredes, 1);    

    //imprimir(Maze, tamanhoTotal, nColunas, paredes);
    imprimirDesenho(Maze, tamanhoTotal, nColunas, paredes, nLinhas, MazeSolution);

    //Aguarda, abre entrada e saída e inicia criação do caminho
    cout << "Criando caminho ..." << endl;
    sleep(3);
    
    //Encontrar caminho segundo parâmetro e imprimindo andamento
    criarCaminho(Maze, nColunas, nLinhas, paredes, totalConexo, conjuntos, MazeSolution);

    return 0;
}

void iinicializarMazeSolution(string MazeSolution[], int tamanhoTotal)
{
    for(int i = 0; i < tamanhoTotal; i++)
        MazeSolution[i] = "  ";
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

    //cout << "Parede " << parede << " da posicao " << posicao << " derrubada!" << endl;
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

void imprimirParede(int parede)
{

}


void imprimirDesenho(int Maze[], int tamanhoTotal, int nColunas, int paredes[], int nLinhas, string MazeSolution[])
{
    bool condicao = true;
    int count = 0; //conta as linhas que estão sendo impressas
    int celulaInicial = 0;
    string espacoAux = "";
    while(condicao)
    {
        celulaInicial = count * nColunas;

        if(count == 0){ //Só imprime o norte da primeira linha (evitar paredes duplas)
            cout << "-"; //para alinhar com colunas
            for(int i = celulaInicial; i < (celulaInicial + nColunas); i++)
            {
                //Imprime norte
                if(verificarParedeIntacta(Maze, i, paredes[0]))
                {
                    cout << "---";
                }else{
                    cout << "  -";
                }    
            }
            cout << endl;
        }
        //Percorre a linha inteira
        for(int i = celulaInicial; i < (celulaInicial + nColunas); i++)
        {
            //Imprime oeste
            if(verificarParedeIntacta(Maze, i, paredes[3]))
            {
                if((i % nColunas) == 0) //Só imprime oeste se for a primeira coluna (evitar pardes duplas)
                    cout << "|";

                //Imprime leste
                if(verificarParedeIntacta(Maze, i, paredes[1]))
                {
                    cout << espacoAux << "  |";
                    //cout << espacoAux << MazeSolution[i] << "|";
                }else{
                    cout << espacoAux << "   ";
                    //cout << espacoAux << " " << MazeSolution[i] << " ";
                }
            }else{
                if((i % nColunas) == 0) //espaço onde ficaria a parede oeste da primeira coluna
                    cout << " ";

                //Imprime leste
                if(verificarParedeIntacta(Maze, i, paredes[1]))
                {
                    cout << "  |";
                    //cout << MazeSolution[i] << "|";
                }else{
                    cout << "   ";
                    //cout << MazeSolution[i] << " ";
                }
            }
        }
        cout << endl;   
        cout << "-"; //para alinhar com colunas
        //Percorre a linha inteira
        for(int i = celulaInicial; i < (celulaInicial + nColunas); i++)
        {
            //Imprime sul
            if(verificarParedeIntacta(Maze, i, paredes[2]))
            {
                cout << "---";
            }else{
                cout << "  -";
            }    
        }
        cout << endl;

        if(count == nLinhas - 1){
            condicao = false;
        }

        count++;        
    }

    cout << endl << endl;   
}


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
    //cout << "Criando Labirinto " << nLinhas << "x" << nColunas << " ..." << endl;
    
    int tamanhoTotal = nColunas * nLinhas;

    //Criar o labirinto em si
    for (int i=0; i < (tamanhoTotal); i++ )
    {
        //Maze[ i ] = TopWall | RightWall | BottomWall | LeftWall;
        Maze [ i ] = paredes[0] | paredes[1] | paredes[2] | paredes[3];
     
    }    
}

void criarCaminho(int Maze[], int nColunas, int nLinhas, int paredes[], bool totalConexo, DisjSets &conjuntos, string MazeSolution[])
{
    //Criar caminho válido ou totalmente conexo

    int tamanhoTotal = nColunas * nLinhas;
    int posicaoInicial = 0;
    int posicaoFinal = tamanhoTotal - 1;
    int celulaRand = 0;
    bool concluido = 0;
    int k = 0; //posicao auxiliar para definir parede
    int celulaVizinha;

    //Derrubar entrada e saída
    derrubarParede(Maze, 0, paredes[3], buscarCelulaVizinhaParede(0, nColunas, nLinhas, paredes[3]));
    derrubarParede(Maze, tamanhoTotal - 1, paredes[1], buscarCelulaVizinhaParede(tamanhoTotal - 1, nColunas, nLinhas, paredes[1]));    

    imprimirDesenho(Maze, tamanhoTotal, nColunas, paredes, nLinhas, MazeSolution);
    sleep(1);

    srand (time(0));

    while(!concluido){
        celulaRand = rand() % posicaoFinal + posicaoInicial; //numero aleatorio entre posicao inicial e final
        k = rand() % 3 + 0; //entre 0 e 3, pois pode ser uma das 4 paredes. 0->Norte, 1->Leste, 2->Sul, 3->Oeste

        //cout << "Celula rand: " << celulaRand << " k rand: " << k << " posicao rand: " << paredes[k] << endl;

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
            //Apenas Válido
            if(!totalConexo && (conjuntos.find(posicaoInicial) == conjuntos.find(posicaoFinal)))
            {
                concluido = 1;
            }
            dentro = 1;


            imprimirDesenho(Maze, tamanhoTotal, nColunas, paredes, nLinhas, MazeSolution);
            usleep(150 * 1000);
        }
    }

    cout << endl;
    imprimir(Maze, tamanhoTotal, nColunas, paredes);
    conjuntos.print();
    cout << endl;
    imprimirDesenho(Maze, tamanhoTotal, nColunas, paredes, nLinhas, MazeSolution);

    cout << "CONCLUIDO" << endl;
}

bool backtracking(int Maze[], int nLinhas, int nColunas, string MazeSolution[], int posicaoAtual ,int posicaoFinal, int paredes[])
{
    int posicaoVizinha = 0;
    bool acessivel = false;

    //Encontrou solução final
    if(posicaoAtual == posicaoFinal)
    {
        MazeSolution[posicaoAtual] = "**";
        return true;
    }

    //Não encontrou a solução final, então assume que essa posição faz parte da solução e
    //verifica na ordem: leste, sul, oeste, norte
    MazeSolution[posicaoAtual] = "**";

    //1 => 2 - LESTE 
    posicaoVizinha = buscarCelulaVizinhaParede(posicaoAtual, nColunas, nLinhas, paredes[1]);
    acessivel = verificarParedeIntacta(Maze, posicaoAtual, paredes[1]);
            
    if((posicaoVizinha != -1) && acessivel)
        return true;

    //2 => 4 - SUL 
    posicaoVizinha = buscarCelulaVizinhaParede(posicaoAtual, nColunas, nLinhas, paredes[2]);
    acessivel = verificarParedeIntacta(Maze, posicaoAtual, paredes[2]);
            
    if((posicaoVizinha != -1) && acessivel)
        return true;
    
    //3 => 8 - OESTE 
    posicaoVizinha = buscarCelulaVizinhaParede(posicaoAtual, nColunas, nLinhas, paredes[3]);
    acessivel = verificarParedeIntacta(Maze, posicaoAtual, paredes[3]);
            
    if((posicaoVizinha != -1) && acessivel)
        return true;
    
    //0 => 1 - NORTE
    posicaoVizinha = buscarCelulaVizinhaParede(posicaoAtual, nColunas, nLinhas, paredes[0]);
    acessivel = verificarParedeIntacta(Maze, posicaoAtual, paredes[0]);
            
    if((posicaoVizinha != -1) && acessivel)
        return true;
    
    //Define como não fazendo parte da solução e retorna falso
    MazeSolution[posicaoAtual] = "xx";
    return false;
}

bool encontrarSolucao(int Maze[], int nColunas, int nLinhas, int paredes[], string MazeSolution[])
{
    int posicaoAtual = 0; //posicaoAtual = posicaoInicial
    int tamanhoTotal = nColunas * nLinhas;
    int posicaoFinal = tamanhoTotal - 1;

    //Faz o backtraking pra encontrar a solução
    if(backtracking(Maze, nLinhas, nColunas, MazeSolution, posicaoAtual, posicaoFinal, paredes) == false)
    {
        cout << "Não existe solução!" << endl;
        return true;
    }

    //Imprime a solução
    imprimirDesenho(Maze, tamanhoTotal, nColunas, paredes, nLinhas, MazeSolution);
}