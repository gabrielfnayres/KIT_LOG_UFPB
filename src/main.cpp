#include "Data.h"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

using namespace std;

typedef struct Solucao
{

    vector<int> sequence;
    double valorObj;

}Solucao;

typedef struct InsertionInfo
{
    int noInserido;
    int arestaRemovida;
    double custo;   

}InsertionInfo;

void exibirSolucao(Solucao *s)
{
    for(int i = 0; i <s->sequence.size() - 1; i++)
    {
        cout << s->sequence[i] << " -> ";
        cout << s->sequence.back() << endl;
    }
}

void calcularValorObj(Solucao *s, Data &data)
{
    s->valorObj = 0.0;

    for(int i = 0; i < s->sequence.size() - 1; i++)
    {
        s->valorObj += data.getDistance(s->sequence[i], s->sequence[i+1]);
    }
}

vector<InsertionInfo> calcularCustoInsercao(Solucao s, vector<int>& CL, Data &data)
{
    vector<InsertionInfo> custoInsercao((s.sequence.size()-1)*CL.size());

    int l = 0;
    for(int a = 0; a < s.sequence.size() - 1; a++)
    {
        int i = s.sequence[a];
        int j = s.sequence[a+1];

        for(auto k : CL)
        {
            custoInsercao[l].custo = data.getDistance(i, k) + data.getDistance(k, j) - data.getDistance(i, j);
            custoInsercao[l].noInserido = k;
            custoInsercao[l].arestaRemovida = a;
            l++;
        }
    } 
    return custoInsercao;
}


vector<int> escolher3NosAleatorios(vector<int>& CL)
{
    vector<int> nosEscolhidos(3);
    bool mesmo = false;

    for(int i = 0; i < CL.size() - 1; i++)
    {
        nosEscolhidos[i] = CL[rand() % CL.size()];

        for(int j = 0; j < i; j++)
        {
            if(nosEscolhidos[i] == nosEscolhidos[j])
            {
                mesmo = true;
                break;
            }
        }
    }
    return nosEscolhidos;
}

void InserirNaSolucao(Solucao &s, int k)
{
    for(int i = 0; i < s.sequence.size() - 1; i++)
    {
        if(s.sequence[i] == k)
        {
            s.sequence.erase(s.sequence.begin() + i);
            break;
        }
    }
}

Solucao Construcao(vector<int>&CL, Data &data)
{
    Solucao s = {{1, 1}, 0.0};
    s.sequence = escolher3NosAleatorios(CL);
    while(!CL.empty())
    {
        vector<InsertionInfo> custoInsercao = calcularCustoInsercao(s, CL, data);
        sort(custoInsercao.begin(), custoInsercao.end(), [](InsertionInfo a, InsertionInfo b){return a.custo < b.custo;});
        double alpha = (double) rand() / RAND_MAX;
        int selecionado = rand() % ((int) ceil(alpha * custoInsercao.size()));
        InserirNaSolucao(s, custoInsercao[selecionado].noInserido);
    }
    return s;
}

bool bestImprovementSwap(Solucao *s, Data &data)
{
    double bestDelta = 0;
    int best_i, best_j;

    for(int i = 1; i < s->sequence.size() -  1; i++)
    {
        int vi = s->sequence[i];
        int vi_next = s->sequence[i+1];
        int vi_prev = s->sequence[i-1];
        
        for(int j = i + 1; j < s->sequence.size()-1; j++)
        {
            int vj = s->sequence[j];
            int vj_next = s->sequence[j+1];
            int vj_prev = s->sequence[j-1];

            double delta = -data.getDistance(vi_prev, vi) - data.getDistance(vi, vi_next) - data.getDistance(vj_prev, vj)
                           - data.getDistance(vj, vj_next) + data.getDistance(vi_prev, vj) + data.getDistance(vj, vi_next)
                           + data.getDistance(vj_prev, vi) + data.getDistance(vi, vj_next);
        
            if(delta < bestDelta)
            {
                bestDelta = delta;
                best_i = i;
                best_j = j;
            }
        }
        if(bestDelta < 0)
        {
            reverse(s->sequence[best_i], s->sequence[best_j]);
            s->valorObj = s->valorObj + bestDelta;
            return true;
        }
        return false;
    }   
}

bool bestImprovement2Opt(Solucao *s, Data &data)
{

    double bestDelta = 0;
    int best_i, best_j;
    double delta;

    for(int i = 1; i < s->sequence.size() - 2; i++)
    {
        int vi = s->sequence[i];
        int vi_next = s->sequence[i+1];
        int vi_prev = s->sequence[i-1];

        for(int j = i + 2; j < s->sequence.size(); j++)
        {
            int vj = s->sequence[j];
            int vj_next = s->sequence[j+1];
            int vj_prev = s->sequence[j-1];

            delta =  -data.getDistance(vi, vi_next) - data.getDistance(vj, vj_next) 
                     + data.getDistance(vj, vi_next) + data.getDistance(vi, vj_next);


            if(delta < bestDelta)
            {
                bestDelta = delta;
                best_i = i;
                best_j = j;
            }
        }

        if(bestDelta < 0)
        {

            reverse(s->sequence.begin() + best_i, s->sequence.begin() + best_j);
            s->valorObj = s->valorObj + bestDelta;
            return true;

        }
        return false;
    }
}

bool bestImprovementOrOpt(Solucao *s, Data &data, int n)
{

    double bestDelta = 0;
    int best_i, best_j;
    double delta;

    switch (n)
    {
    case 1: //REINSERTION
        for(int i = 1; i < s->sequence.size(); i++)
        {
            int vi = s->sequence[i];
            int vi_prev = s->sequence[i-1];
            int vi_next = s->sequence[i+1];

            for(int j = 1; j < s->sequence.size(); j++)
            {
                int vj = s->sequence[j];
                int vj_next = s->sequence[j+1];
                int vj_prev = s->sequence[j-1];

                if(j < i)
                {
                    delta = -data.getDistance(vi_prev, vi) - data.getDistance(vi, vi_next) - data.getDistance(vj_prev, vj)
                            + data.getDistance(vi_prev, vi_next) + data.getDistance(vi, vj) + data.getDistance(vj_prev, vi);  
                }
                else if(j > i)
                {
                    delta = -data.getDistance(vi_prev, vi) - data.getDistance(vi, vi_next) - data.getDistance(vj, vj_next)
                        + data.getDistance(vi_prev, vi_next) + data.getDistance(vi, vj) + data.getDistance(vi, vj_next);
                }
                else if(i == j - 1)
                {

                    delta = -data.getDistance(vi_prev, vi) - data.getDistance(vj, vj_next) + data.getDistance(vi, vj)
                            + data.getDistance(vi_prev, vj) +  data.getDistance(vi, vj_next);
                }        

                if(delta < bestDelta)
                {
                    bestDelta = delta;
                    best_i = i;
                    best_j = j;
                }
            }
        }
        if(delta < 0)
        {
            if(best_i > best_j)
            {
                s->sequence.insert(s->sequence.begin() + best_j, s->sequence[best_i]);
                s->sequence.erase(s->sequence.begin() + best_i + 1);
            }
            else
            {
                s->sequence.insert(s->sequence.begin() + best_j + 1, s->sequence[best_i]);
                s->sequence.erase(s->sequence.begin() + best_i);
            }
            s->valorObj += bestDelta;
            return true;
        }
        return false;

    case 2: //OR-OPT-2
        for(int i = 1; i < s->sequence.size() - 2; i++)
        {
            int vi = s->sequence[i];
            int vi_next = s->sequence[i+1];
            int vi_next2 = s->sequence[i+2];
            int vi_prev = s->sequence[i-1];

            for(int j = 1; j < s->sequence.size() - 1; j++)
            {
                int vj = s->sequence[j];
                int vj_next = s->sequence[j+1];
                int vj_next2 = s->sequence[j+2];
                int vj_prev = s->sequence[j-1];

                if(j > i)
                {
                    delta =  -data.getDistance(vi_prev, vi) - data.getDistance(vi_next, vi_next2) - data.getDistance(vj_prev, vj)
                             - data.getDistance(vj_next, vj_next2) + data.getDistance(vi_prev, vj) + data.getDistance(vj_next, vi_next2)
                             + data.getDistance(vj_prev, vi) + data.getDistance(vi_next, vj_next2);
                }
                else if(j < i)
                {
                    delta = -data.getDistance(vj_prev, vj) - data.getDistance(vj_next, vj_next2) - data.getDistance(vi_prev, vi)
                            - data.getDistance(vi_next, vi_next2) + data.getDistance(vi_prev, vj) + data.getDistance(vj_next, vi_next2)
                            + data.getDistance(vj_prev, vi) + data.getDistance(vi_next, vj_next2);
                }
                else if (i == j || i == j - 1 || i == j + 1)
                {
                    delta = 0;
                    continue;
                }

                if(delta < bestDelta)
                {
                    bestDelta = delta;
                    best_i = i;
                    best_j = j;
                }
            }         
        }    
        if(bestDelta < 0)
        {
            if(best_i > best_j)
            {
                s->sequence.insert(s->sequence.begin() + best_j + 1, s->sequence[best_i]);
                s->sequence.insert(s->sequence.begin() + best_j + 2, s->sequence[best_i + 1]);
                s->sequence.erase(s->sequence.begin() + best_i);
                s->sequence.erase(s->sequence.begin() + best_i + 1);
            }
            else
            {
                s->sequence.insert(s->sequence.begin() + best_j, s->sequence[best_i]);
                s->sequence.insert(s->sequence.begin() + best_j + 1, s->sequence[best_i + 2]);
                s->sequence.erase(s->sequence.begin() + best_i);
                s->sequence.erase(s->sequence.begin() + best_i + 2);
            }
            s->valorObj += bestDelta;
            return true;
        }
        return false;

    case 3: //OR-OPT-3
        for(int i = 1; i < s->sequence.size() - 3; i++)
        {
            int vi = s->sequence[i];
            int vi_next = s->sequence[i+1];
            int vi_next2 = s->sequence[i+2];
            int vi_next3 = s->sequence[i+3];
            int vi_prev = s->sequence[i-1];

            for(int j = 1; j < s->sequence.size() - 2; j++)
            {
                int vj = s->sequence[j];
                int vj_next = s->sequence[j+1];
                int vj_next2 = s->sequence[j+2];
                int vj_next3 = s->sequence[j+3];
                int vj_prev = s->sequence[j-1];

                if(j > i)
                {
                    delta = -data.getDistance(vi_prev, vi) - data.getDistance(vi_next2, vi_next3) - data.getDistance(vj_prev, vj)
                            - data.getDistance(vj_next2, vj_next3) + data.getDistance(vi_prev, vj) + data.getDistance(vj_next2, vi_next3)
                            + data.getDistance(vj_prev, vi) + data.getDistance(vi_next2, vj_next3);
                }
                else if(j < i)
                {

                    delta = -data.getDistance(vj_prev, vj) - data.getDistance(vj_next2, vj_next3) - data.getDistance(vi_prev, vi)
                            - data.getDistance(vi_next2, vi_next3) + data.getDistance(vi_prev, vj) + data.getDistance(vj_next2, vi_next3)
                            + data.getDistance(vj_prev, vi) + data.getDistance(vi_next2, vj_next3);
                }
                else if (i == j || i == j - 2 || i == j + 2)
                {
                    delta = 0;
                    continue;
                }

                if(delta < bestDelta)
                {
                    bestDelta = delta;
                    best_i = i;
                    best_j = j;
                }
            }
        }
        if(bestDelta < 0)
        {
            if(best_i > best_j)
            {
                s->sequence.insert(s->sequence.begin() + best_j + 1, s->sequence[best_i]);
                s->sequence.insert(s->sequence.begin() + best_j + 2, s->sequence[best_i + 1]);
                s->sequence.insert(s->sequence.begin() + best_j + 3, s->sequence[best_i + 2]);
                s->sequence.erase(s->sequence.begin() + best_i);
                s->sequence.erase(s->sequence.begin() + best_i + 1);
                s->sequence.erase(s->sequence.begin() + best_i + 2);
            }
            else
            {
                s->sequence.insert(s->sequence.begin() + best_j, s->sequence[best_i + 1]);
                s->sequence.insert(s->sequence.begin() + best_j + 1, s->sequence[best_i + 2]);
                s->sequence.insert(s->sequence.begin() + best_j + 2, s->sequence[best_i + 3]);
                s->sequence.erase(s->sequence.begin() + best_i + 1);
                s->sequence.erase(s->sequence.begin() + best_i + 2);
                s->sequence.erase(s->sequence.begin() + best_i + 3);
            }
            s->valorObj += bestDelta;
            return true;
        }
        return false;
    }
}

void BuscaLocal(Solucao *s, Data &data)
{
    vector<int> NL = {1, 2, 3 , 4, 5};  
    bool improved = false;
    
    while(NL.empty() == false)
    {
        int n = rand() % NL.size();

        switch (NL[n])
        {
            case 1:
                improved = bestImprovementSwap(s, data);
                break;
            case 2:
                improved = bestImprovement2Opt(s, data);
                break;
            case 3:
                improved = bestImprovementOrOpt(s, data, 1);
                break;
            case 4:
                improved = bestImprovementOrOpt(s, data, 2);
                break;
            case 5:
            {
                improved = bestImprovementOrOpt(s, data, 3);
                break;
            }
        }
        if(improved)
        {
            NL = {1, 2, 3 , 4, 5};
        }
        else
        {
            NL.erase(NL.begin() + n);
        }
    }
}

//Solucao Pertubacao(Solucao s, Data &data)
//{
  //  Solucao pert = s;

//    int n = pert.sequence.size();
  //  int t1 = 2;
    //int t2 = (n - 1)/10;


    //return pert;
//}


Solucao ILS(int maxIter, int maxIterIls,  Data &data)
{
    Solucao bestOfAll;
    bestOfAll.valorObj = INFINITY;

    for(int i = 0; i < maxIter; i++)
    {
        Solucao s = Construcao(s.sequence ,data);
        Solucao best = s;

        int iterILS = 0;

        while (iterILS <= maxIterIls)
        {
            BuscaLocal(&s, data);
            if(s.valorObj < best.valorObj)
            {
                best = s;
                iterILS = 0;
            }
          //  s = Perturbacao(best, data);
            iterILS++;
        }

        if(best.valorObj < bestOfAll.valorObj)
        {
            bestOfAll = best;
        }
    }
    return bestOfAll;
}

int main(int argc, char** argv) {

    int maxIter = 50;
    int maxIterIls;
    Solucao best;
    double custo = 0.0;
    auto data = Data(argc, argv[1]);
    data.read();
    size_t n = data.getDimension();

    if(n >= 150)
    {
        maxIterIls = n/2;
    }
    else
    {
        maxIterIls = n;
    }

    cout << "Dimension: " << n << endl;
    cout << "DistanceMatrix: " << endl;
    data.printMatrixDist();


    cout << "Exemplo de Solucao s = ";
    exibirSolucao(&best);
    calcularValorObj(&best, data);
    custo += data.getDistance(n, 1);
    cout << n << " -> " << 1 << endl;
    cout << "Custo de S: " << custo << endl;

    return 0;
}
