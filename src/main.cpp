#include "Data.h"
#include <vector>
#include <algorithm>
#include <random>
#include <set>


using namespace std;

typedef struct 
{    

    vector<int> sequence;
    double valorObj;

} Solution;

typedef struct
{
    int noInserido; // k
    int arestaRemovida; // {i, j}
    double custo; // c{i, k} + c{k, j} - c{i, j}
}InsertionInfo;

void showSolution( Solution s)
{

    for(int i = 0; i < s.sequence.size(); i++)
    {
        cout << s.sequence[i] << "->";
        cout << s.sequence.back() << endl;
    }
}

void objCost(Solution s, Data &data)
{
    s.valorObj = 0;
    for(int i = 0; i < s.sequence.size() - 1; i++)
    {
        s.valorObj += data.getDistance(s.sequence[i], s.sequence[i+1]);
    }
}

vector<InsertionInfo> CalcularCustoDeInsercao(Solution s,  vector<int> &CL,  Data &data)
{
    vector<InsertionInfo> custoInsercao((s.sequence.size() - 1) * CL.size());

    int l = 0;
    for(int a = 0; a < s.sequence.size() - 1; a++)
    {
        int i = s.sequence[a];
        int j = s.sequence[a+1];
        for(auto y : CL)
        {
            custoInsercao[l].custo = data.getDistance(i, y) + data.getDistance(j, y) - data.getDistance(i, j);
            custoInsercao[l].arestaRemovida = a;
            custoInsercao[l].noInserido = y;    
            l++;        
        }
    }
    return custoInsercao;
}

vector<int> escolher3NosAleatorios(Solution &s)
{
    int tamanho = s.sequence.size();
    int no[3];
    bool encontrou;

    vector<int> nosSelecionados(4);
    
    for(int i = 0; i < 3; i++)
    {
        no[i] = rand() % tamanho - 1;
        encontrou = true;
        while(encontrou)
        {
            for(int j = 0; j < i; j++)
            {
                if(no[i] == no[j])
                {
                    no[i] = rand() % tamanho + 1;
                    encontrou = false;
                    break; 
                }
            }
        }    
    }
    nosSelecionados[0] = no[0];
    nosSelecionados[1] = no[1];
    nosSelecionados[2] = no[2];
    nosSelecionados[3] = no[3];
    nosSelecionados[0] = no[0];
    return nosSelecionados;        
}

vector<int> nosRestantes(vector<int> &nosselecionados,  Data &data)
{
    vector<int> CL(data.getDimension() - (nosselecionados.size() - 1));

    int count = 0; 

    for(int i = 1; i <= data.getDimension(); i++)
    {
        if((i != nosselecionados[0]) && (i != nosselecionados[1]) && (i != nosselecionados[2]))
        {
            CL[count++] = i;
        }
    }
    return CL;

}

void inserirNaSolucao(Solution &s,  vector<int> &CL,  InsertionInfo &custoInsercao)
{
    
    s.sequence.insert(s.sequence.begin() + custoInsercao.arestaRemovida + 1, custoInsercao.noInserido);
    s.valorObj += custoInsercao.custo;
    
    for(auto k = CL.begin(); k != CL.end(); k++)
    {
        if(*k == custoInsercao.noInserido)
        {
            CL.erase(k);
            break;
        }
    }
}

Solution Construcao(Solution s, Data &data)
{
    Solution s = {{}, 0.0};
    s.sequence = escolher3NosAleatorios(s);
    vector<int> CL = nosRestantes(s.sequence, data);
    while (!CL.empty())
    {
        vector<InsertionInfo> custoInsercao = CalcularCustoDeInsercao(s, CL, data);
        sort(custoInsercao.begin(), custoInsercao.end(), [](InsertionInfo &a, InsertionInfo &b) { return a.custo < b.custo; });
        double alpha = (double) rand() / RAND_MAX;
        int selecionado = rand() % ((int) ceil(alpha * custoInsercao.size()));
        inserirNaSolucao(s, CL, custoInsercao[selecionado]);
    }
}

bool bestImprovementSwap(Solution *s, Data &data)
{
    double bestDelta = 0;
    int best_i, best_j;

    for(int i = 1; i < s->sequence.size() - 1; i++)
    {
        int vi = s->sequence[i];
        int vi_next = s->sequence[i+1];
        int vi_prev = s->sequence[i-1];

        for(int j = i + 1; j < s->sequence.size() -1; j++)
        {
            int vj = s->sequence[j];
            int vj_next = s->sequence[j+1];
            int vj_prev = s->sequence[j-1];

            double delta = -data.getDistance(vi_prev,vi) - data.getDistance(vi,vi_next) 
                            + data.getDistance(vi_prev,vj) + data.getDistance(vj,vi_next) 
                            - data.getDistance(vj_prev,vj) - data.getDistance(vj,vj_next) 
                            + data.getDistance(vj_prev,vi) + data.getDistance(vi,vj_next); 

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
        swap(s->sequence[best_i], s->sequence[best_j]);
        s->valorObj = s->valorObj + bestDelta;
        return true;
    }
    return false;
}


bool bestImprovement2Opt(Solution *s, Data &data)
{
    double bestDelta = 0;
    double delta, custo;
    int best_i, best_j;

    for(int i = 1; i < s->sequence.size() - 2; i++)
    {
        int vi = s->sequence[i];
        int vi_next = s->sequence[i+1];
        int vi_prev = s->sequence[i-1];


        for(int j = i + 1; j < s->sequence.size() - 1; j++)
        {
            int vj = s->sequence[j];
            int vj_next = s->sequence[j+1];
            int vj_prev = s->sequence[j-1];

            delta = -data.getDistance(vi_prev, vi) - data.getDistance(vj, vj_next) + data.getDistance(vi_prev, vj) + data.getDistance(vi, vj_next);

            
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
        swap(s->sequence[best_i], s->sequence[best_j]);
        s->valorObj += bestDelta;
        return true;
    }
    return false;
}

bool bestImprovementOrOpt(Solution s, Data &data, int quantidade)
{

    double bestDelta = 0;
    double delta;
    double custo;
    int best_i, best_j;

    switch (quantidade)
    {
    case 1:  // reinsertion
        
        for(int i = 1; i < s.sequence.size(); i++)
        {

            int vi = s.sequence[i];
            int vi_next = s.sequence[i+1];
            int vi_prev = s.sequence[i-1];

            
            for(int j = 1; j < s.sequence.size() - 1; j++)
            {
                int vj = s.sequence[j];
                int vj_next = s.sequence[j+1];
                int vj_prev = s.sequence[j-1];


                if(i == j || i == j - 1 || i == j + 1)
                {
                    delta = 0;
                    continue;
                }
                else
                {
                    delta =  -data.getDistance(vi_prev, vi) - data.getDistance(vi, vi_next) - data.getDistance(vj, vj_next) + data.getDistance(vi, vj) + data.getDistance(vj_prev, vi) + data.getDistance(vi, vj_next);
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
                s.sequence.insert(s.sequence.begin() + best_j, s.sequence[best_i]);
                s.sequence.erase(s.sequence.begin() + best_i + 1);
            }
            else 
            {
                s.sequence.insert(s.sequence.begin() + best_j + 1, s.sequence[best_i]);
                s.sequence.erase(s.sequence.begin() + best_i);
            }

            s.valorObj  += bestDelta;

            return true;
        }
        return false;

    case 2: // OR OPT 2

        for(int i = 1; i < s.sequence.size() - 2; i++)
        {
            int vi = s.sequence[i];
            int vi_next = s.sequence[i+1];
            int vi_prev = s.sequence[i-1];

            for(int j = 1; j < s.sequence.size() - 1; i++)
            {
                int vj = s.sequence[j];
                int vj_next = s.sequence[j+1];
                int vj_prev = s.sequence[j-1];

                if(i == j || i == j - 1 || i == j + 1)
                {
                    delta = 0;
                    continue;
                }
                else
                {
                    delta = -data.getDistance(vi_prev, vi) - data.getDistance(vi, vi_next) - data.getDistance(vj, vj_next) + data.getDistance(vi, vj) + data.getDistance(vj_next, vi_next) + data.getDistance(vi_next, vj_next) + data.getDistance(vj_prev, vi_prev) + data.getDistance(vi_prev, vj_prev);
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
                s.sequence.insert(s.sequence.begin() + best_j, s.sequence[best_i]);
                s.sequence.insert(s.sequence.begin() + best_j + 1, s.sequence[best_i + 1]);
                s.sequence.erase(s.sequence.begin() + best_i + 1);
                s.sequence.erase(s.sequence.begin() + best_i + 2);
            }
            else
            {
                s.sequence.insert(s.sequence.begin() + best_j + 1, s.sequence[best_i]);
                s.sequence.insert(s.sequence.begin() + best_j + 2, s.sequence[best_i + 1]);
                s.sequence.erase(s.sequence.begin() + best_i);
                s.sequence.erase(s.sequence.begin() + best_i + 1);
            }
            s.valorObj += bestDelta;
            return true;
        }
        return false;

    case 3: // OR-Opt3
        for(int i = 1; i < s.sequence.size() - 3; i++)
        {
            int vi = s.sequence[i];
            int  vi_next = s.sequence[i+1];
            int vi_next2 = s.sequence[i+2];
            int vi_prev = s.sequence[i-1];

            for(int j = 1; j < s.sequence.size() - 2; j++)
            {
                int vj = s.sequence[j];
                int vj_next = s.sequence[j+1];
                int vj_next2 = s.sequence[j+2];
                int vj_prev = s.sequence[j-1];

                if(i == j || i == j - 1 || i == j + 1)
                {
                    delta = 0;
                    continue;
                }
                else
                {
                    delta = -data.getDistance(vi_prev, vi) - data.getDistance(vi, vi_next) -data.getDistance(vi, vi_next2) - data.getDistance(vj, vj_next2) - data.getDistance(vj, vj_next) + data.getDistance(vi, vj) + data.getDistance(vj_next, vj_next2) + data.getDistance(vj_prev, vi_prev) + data.getDistance(vi_prev, vj_prev) + data.getDistance(vi_next, vi_next2) + data.getDistance(vi_next2, vj_next2) + data.getDistance(vi_next, vj_next) + data.getDistance(vi_prev, vj_next) + data.getDistance(vi_prev, vj_next2) + data.getDistance(vi, vj_next2) + data.getDistance(vi, vj_next);
                }

                if(delta < bestDelta)
                {
                    bestDelta = delta;
                    best_i = i;
                    best_j = j;
                }
            }            

            if(bestDelta < 0)
            {

                if(best_i > best_j)
                {
                    s.sequence.insert(s.sequence.begin() + best_j, s.sequence[best_i]);
                    s.sequence.insert(s.sequence.begin() + best_j + 1, s.sequence[best_i + 1]);
                    s.sequence.insert(s.sequence.begin() + best_j + 2, s.sequence[best_i + 2]);
                    s.sequence.erase(s.sequence.begin() + best_i + 1);
                    s.sequence.erase(s.sequence.begin() + best_i + 2);
                    s.sequence.erase(s.sequence.begin() + best_i + 3);
                }
                else
                {
                    s.sequence.insert(s.sequence.begin() + best_j + 1, s.sequence[best_i]);
                    s.sequence.insert(s.sequence.begin() + best_j + 2, s.sequence[best_i + 1]);
                    s.sequence.insert(s.sequence.begin() + best_j + 3, s.sequence[best_i + 2]);
                    s.sequence.erase(s.sequence.begin() + best_i);
                    s.sequence.erase(s.sequence.begin() + best_i + 1);
                    s.sequence.erase(s.sequence.begin() + best_i + 2);
                }

                s.valorObj += bestDelta;
                return true;
            }
            return false;
        }
    }

    
}

void BuscaLocal(Solution *s, Data &data)
{
    
    vector<int> NL = {1, 2, 3, 4, 5};

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
            improved = bestImprovementOrOpt(*s, data, 1);
            break;
        case 4:
            improved = bestImprovementOrOpt(*s, data, 2);
            break;
        case 5:
            improved = bestImprovementOrOpt(*s, data, 3);
            break;
        }

        if(improved)
        {
            NL = {1, 2 ,3, 4, 5};
        }
        else 
        {
            NL.erase(NL.begin() + n);
        }
    }
}

Solution Pertubacao(Solution s, Data &data)
{
    
    bool escolhido = false;
    int inicial = 2;
    int maximo = s.sequence.size()/10;

}

Solution ILS(int maxIter, int maxIterIls,  Data &data)
{
    Solution bestOfAll;
    bestOfAll.valorObj = INFINITY;

    for(int i = 0; i < maxIter; i++)
    {
        Solution s = Construcao(s ,data);
        Solution best = s;

        int iterILS = 0;

        while (iterILS <= maxIterIls)
        {
            BuscaLocal(&s, data);
            if(s.valorObj < best.valorObj)
            {
                best = s;
                iterILS = 0;
            }
            s = Perturbacao(best, data);
            iterILS++;
        }

        if(best.valorObj < bestOfAll.valorObj)
        {
            bestOfAll = best;
        }
    }
    return bestOfAll;
}

int main(int argc, char** argv)
{

    int maxIter = 50, maxIterIls;


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

    cout << "Exemplo de Solucao s =";
    double cost = 0;
    for(int i = 0; i < n; i++)
    {
        cout << i << "->";
        cost +=  data.getDistance(i, i+1);
    }
    cost += data.getDistance(n, 1);
    cout << n << "->" << 1 << endl;
    cout << "Custo de S: "<< cost << endl;
    

    return 0;
}

