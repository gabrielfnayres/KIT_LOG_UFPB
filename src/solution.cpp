#include "Data.h"
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

typedef struct 
{
    vector<int> sequence;
    double cost;
    double **matrizAdj;
} Solution;

typedef struct
{
    int noInserido; // k
    int arestaRemovida; // {i, j}
    double custo; // c{i, k} + c{k, j} - c{i, j}
}InsertionInfo;

void showSolution(Solution *s)
{

    for(int i = 0; i < s->sequence.size(); i++)
    {
        cout << s->sequence[i] << "->";
        cout << s->sequence.back() << endl;
    }
}

void objCost(Solution *s)
{
    s->cost = 0;
    for(int i = 0; i < s->sequence.size() - 1; i++)
    {
        s->cost += s->matrizAdj[s->sequence[i]][s->sequence[i+1]];
    }
}

vector<InsertionInfo> CalcularCustoDeInsercao(Solution &s, vector<int> &CL)
{
    vector<InsertionInfo> custoInsercao((s.sequence.size() - 1) * CL.size());

    int l = 0;
    for(int a = 0; a < s.sequence.size() - 1; a++)
    {
        int i = s.sequence[a];
        int j = s.sequence[a+1];
        for(auto y : CL)
        {
            custoInsercao[l].custo = s.matrizAdj[i][y] + s.matrizAdj[y][j] - s.matrizAdj[i][j];
            custoInsercao[l].arestaRemovida = a;
            custoInsercao[l].noInserido = y;    
            l++;        
        }
    }
    return custoInsercao;
}

vector<int> escolher3NosAleatorios(Solution *s)
{
    int tamanho = s->sequence.size();
    vector<int> nosSelecionados;
    
    for(int i = 0; i < 3; i++)
    {
        int noAleatorio;
        noAleatorio = rand() % tamanho;
        nosSelecionados.push_back(noAleatorio);
    }
    sort(nosSelecionados.begin(), nosSelecionados.end());
    return nosSelecionados;
}

Solution Construcao()
{
    Solution s;
    s.sequence = escolher3NosAleatorios(&s);

}

