#ifndef DISJ_SETS_H
#define DISJ_SETS_H
#include <vector>     //vector
#include <stdexcept>  // std::runtime_error or std::out_of_range
#include <functional> // std::less<>().
#include <cassert>    // assert().
#include <memory>     // std::unique_ptr<>.
#include <initializer_list> // std::initializer_list<>.
#include <iostream> // cout, endl
#include <algorithm> // copy.
/**
 * Disjoint set class.
 * Use union by rank and path compression.
 * Elements in the set are numbered starting at 0.
 */
using std::vector;
using std::cout;
class DisjSets
{
  public:
    // Generates the disjoint sets, one for each element.
    explicit DisjSets( int numElements )
    {
        /*Cada posição do vetor se refere 
         *a uma célular. O valor guardado
         *naquela posição, indica o conjunto
         *ao qual aquela célula pertence!
         */
        for(int i = 0; i < numElements; i++)
        {
            s.push_back(i);
        }

        disjCount = numElements;
        elementsCount = numElements;
    }

    // Return set's name containing x
    int find( int x ) const
    {
        return s[x];
    }
    // Return set's name containing x, with path compression.
    //int find( int x ); ??

    // Merge two sets, by size or by estimated heigh (also knwon as rank).
    void unionSets( int root1, int root2 )
    {
        //Unir os elementos do conjunto que contém root1 com os dos que contém root2
        //int disjRoot1 = s[root1];
        //int disjRoot2 = s[root2];
        int disjRoot1 = find(root1);
        int disjRoot2 = find(root2);

        if(disjRoot1 != disjRoot2)
        {
            for(int i = 0; i<elementsCount; i++)
            {
                //if(s[i] == disjRoot2)
                //{
                //    s[i] = disjRoot1;
                //}
                if(find(i) == disjRoot2)
                {
                    s[i] = disjRoot1;
                }
            }
            disjCount--;
        }
    }

    void print()
    {
        for(int i = 0; i<elementsCount; i++)
        {
            //for(int j = 0; j < disjCount; j++)
            cout << i << "{" << s[i] << "} ";
        }
    }

    // Return the current number of disjoint sets
    int size( ) const
    {
        return disjCount;
    }

  private:
    vector<int> s;
    int disjCount;
    int elementsCount;
};

//#include "disjSets.inl"
#endif