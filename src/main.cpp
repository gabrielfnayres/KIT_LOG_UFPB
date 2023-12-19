#include "Data.h"
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;

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
    for(int i = 0; i < s->sequence.size() - 1; i++)
    {
        cout << s->sequence[i] << " -> ";
    }
    cout << s->sequence.back() << endl;
}

void calcularValorObj(Solucao *s, Data &data)
{
    s->valorObj = 0.0;
    for(int i = 0; i < s->sequence.size() - 1; i++)
    {
        s->valorObj += data.getDistance(s->sequence[i], s->sequence[i+1]);
    }

    cout << s->valorObj << endl;

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


vector<int> escolher3NosAleatorios(int dim)
{
    vector<int> nosEscolhidos(5);
    int nos[3];
    bool mesmo;

    for(int i = 0; i < 3; i++)
    {
         
        nos[i] = rand() % dim + 1;
        if(nos[i] == 1)
        {
            nos[i] = rand() % dim + 1;
            
        }
        mesmo = 1;
        while(mesmo)
        {
            mesmo = 0;
            
        
            for(int j = 0; j < i; j++)
            {
                if(nos[i] == nos[j])
                {
                    nos[i] = rand() % dim + 1;
                    mesmo = 1;
                    break;
                }
            }
        }
    }

    nosEscolhidos[0] = 1;
    if(nos[0] == 1)
    {
        nos[0] = rand() % dim + 1;
        nosEscolhidos[1] = nos[0];
    }
    else
    {
        nosEscolhidos[1] = nos[0];
    }
    nosEscolhidos[2] = nos[1];
    if(nos[2] == 1)
    {
        nos[2] = rand() % dim + 1;
        nosEscolhidos[3] = nos[2];
    }
    else
    {
        nosEscolhidos[3] = nos[2];
    }
    nosEscolhidos[4] = 1;
    

    return nosEscolhidos;
}

void InserirNaSolucao(Solucao &s, InsertionInfo &k, vector<int>& CL)
{
    s.sequence.insert(s.sequence.begin() + k.arestaRemovida + 1, k.noInserido);
    s.valorObj += k.custo;
    for(auto a  =  CL.begin(); a != CL.end(); a++)
    {
        if(*a == k.noInserido)
        {
            CL.erase(a);
            break;
        }
    }
}

vector<int> restoDosNos(vector<int> sequence, int dim)
{
    vector<int> CL(dim - (sequence.size() - 1));
    int index = 0;
    for(int i = 1; i <=dim; i++)
    {
       if(i != sequence[0] && i != sequence[1] && i != sequence[2] && i != sequence[3])
       {
           CL[index++] = i;
       }
    }
    return CL;
}

bool caroSouN(InsertionInfo a, InsertionInfo b)
{
    return (a.custo < b.custo);
}

Solucao Construcao(Data &data, int dim)
{
    double alpha;
    int selecionado, sla;
    Solucao s = {{}, 0.0};
    s.sequence = escolher3NosAleatorios(dim);
    vector <int> CL = restoDosNos(s.sequence, dim);
    while(!CL.empty())
    {
        vector<InsertionInfo> custoInsercao = calcularCustoInsercao(s, CL, data);
        sort(custoInsercao.begin(), custoInsercao.end(), caroSouN);
        alpha = (double) (rand() % RAND_MAX + 1)/ RAND_MAX;
        alpha += 0.0000000001;
        sla =  (int) (alpha * custoInsercao.size());
        if(sla == 0)
        {
            selecionado = 0;
        }
        else{
            selecionado = rand() % sla;
        }
        InserirNaSolucao(s, custoInsercao[selecionado], CL);
    }
    return s;
}

bool bestImprovementSwap(Solucao &s, Data &data)
{
    int best_i, best_j;
    int i, j;
    double delta, best_delta = 0.0;

    for( i = 1; i < s.sequence.size() - 1; i++)
    {
        
        for( j = i + 1; j < s.sequence.size() - 1; j++)
        {
            
            if(j-i == 1)
            {
                delta = -data.getDistance(s.sequence[i-1], s.sequence[i]) - data.getDistance(s.sequence[j],s.sequence[j+1])
                        +data.getDistance(s.sequence[i-1], s.sequence[j]) + data.getDistance(s.sequence[j], s.sequence[i+1]);
            }
            else{
                delta = -data.getDistance(s.sequence[i-1], s.sequence[i]) - data.getDistance(s.sequence[i], s.sequence[i+1])
                        - data.getDistance(s.sequence[j-1], s.sequence[j]) - data.getDistance(s.sequence[j], s.sequence[j+1])
                        +data.getDistance(s.sequence[i-1], s.sequence[j]) + data.getDistance(s.sequence[j] , s.sequence[i+1])
                        +data.getDistance(s.sequence[j-1], s.sequence[i]) + data.getDistance(s.sequence[i], s.sequence[j+1]);
            }
            if(delta < best_delta)
            {
                best_delta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }
    if(best_delta < 0)
    {
        swap(s.sequence[best_i], s.sequence[best_j]);
        s.valorObj += best_delta;
        return true;
    }
    return false;
}


bool bestImprovement2Opt(Solucao &s, Data &data)
{
    int best_i, best_j;
    int i, j;
    double delta, best_delta = 0.0;

    for(i = 0; i < s.sequence.size() - 1; i++)
    {
        
        for(j = i + 2; j < s.sequence.size() - 1; j++)
        {
            
            if(i == 0 && j == s.sequence.size() - 1)
            {
                continue;
            }

            delta =  - data.getDistance(s.sequence[i], s.sequence[i+1]) - data.getDistance(s.sequence[j], s.sequence[j+1])
                     + data.getDistance(s.sequence[i], s.sequence[j])  + data.getDistance(s.sequence[i+1], s.sequence[j+1]);

            if(delta < best_delta)
            {
                best_delta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }
    if(best_delta < 0)
    {
        reverse(s.sequence.begin() + best_i + 1, s.sequence.begin() + best_j + 1);
        s.valorObj += best_delta;
        return true;
    }
    return false;
}

bool bestImprovementOrOpt(Solucao &s, Data &data, int n)
{

    double best_delta = 0.0;
    int best_i, best_j;
    int i, j;
    double delta;

    switch (n)
    {
    case 1://REINSERTION 
    for(i = 1; i < s.sequence.size() - 1; i++)
    {
        for(j = 1; j < s.sequence.size() - 1; j++)
        {
            if(j > i)
            {
                delta = -data.getDistance(s.sequence[i-1], s.sequence[i]) - data.getDistance(s.sequence[i], s.sequence[i+1])
                        - data.getDistance(s.sequence[j], s.sequence[j+1]) + data.getDistance(s.sequence[i], s.sequence[j+1])
                        + data.getDistance(s.sequence[i-1], s.sequence[i+1]) + data.getDistance(s.sequence[j], s.sequence[i]);
            }
            else if(j < i)
            {
                delta = -data.getDistance(s.sequence[j-1], s.sequence[j]) - data.getDistance(s.sequence[j], s.sequence[j+1])
                        - data.getDistance(s.sequence[i], s.sequence[i+1]) + data.getDistance(s.sequence[j], s.sequence[i+1])
                        + data.getDistance(s.sequence[j-1], s.sequence[j+1]) + data.getDistance(s.sequence[i], s.sequence[j]);
            }
            else if(i == j-1)
            {
                delta = -data.getDistance(s.sequence[i-1], s.sequence[i]) - data.getDistance(s.sequence[j], s.sequence[j+1])
                        + data.getDistance(s.sequence[i-1], s.sequence[j]) + data.getDistance(s.sequence[i], s.sequence[j+1]);
            }
            else
            {
                continue;
            }
            if(delta < best_delta)
            {
                best_delta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }
    if(best_delta < 0)
    {
        if(best_j > best_i)
        {
            s.sequence.insert(s.sequence.begin() + best_j, s.sequence[best_i]);
            s.sequence.erase(s.sequence.begin() + best_i);
        }
        else
        {
            s.sequence.insert(s.sequence.begin() + best_j + 1, s.sequence[best_i]);
            s.sequence.erase(s.sequence.begin() + best_i);
        }

        s.valorObj += best_delta;
        return true;
    }
    return false;

    case 2: //OR-OPT2
    for(i = 1; i < s.sequence.size() - 2; i++)
    {
        for(j = 1; j < s.sequence.size() - 2; j++)
        {
            if(j > i)
            {
                delta = -data.getDistance(s.sequence[i-1], s.sequence[i]) - data.getDistance(s.sequence[i+1], s.sequence[i+2])
                        - data.getDistance(s.sequence[j], s.sequence[j+1]) + data.getDistance(s.sequence[i-1], s.sequence[i+2])
                        + data.getDistance(s.sequence[j], s.sequence[i]) + data.getDistance(s.sequence[i+1], s.sequence[j+1]);
            }
            else if(j < i)
            {
                delta = -data.getDistance(s.sequence[j-1], s.sequence[j]) - data.getDistance(s.sequence[j+1], s.sequence[j+2])
                        - data.getDistance(s.sequence[i], s.sequence[i+1]) + data.getDistance(s.sequence[j-1], s.sequence[j+2])
                        + data.getDistance(s.sequence[i], s.sequence[j]) + data.getDistance(s.sequence[j+1], s.sequence[i+1]);
            }
            else if(i == j-2)
            {
                delta = -data.getDistance(s.sequence[i-1], s.sequence[i]) - data.getDistance(s.sequence[i+1], s.sequence[j])
                        + data.getDistance(s.sequence[i-1], s.sequence[j]) + data.getDistance(s.sequence[i+1], s.sequence[j+1]);
            }
            else
            {
                continue;
            }


            if(delta < best_delta)
            {
                best_delta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }
    if(best_delta < 0)
    {
        if(best_j > best_i) // depois do j
        {

            s.sequence.insert(s.sequence.begin() + best_j + 1, s.sequence[best_i]);
            s.sequence.insert(s.sequence.begin() + best_j + 2, s.sequence[best_i + 1]);
            s.sequence.erase(s.sequence.begin() + best_i + 1);
            s.sequence.erase(s.sequence.begin() + best_i);
        }
        else
        {
            s.sequence.insert(s.sequence.begin() + best_j, s.sequence[best_i]);
            s.sequence.insert(s.sequence.begin() + best_j + 1, s.sequence[best_i + 2]);
            s.sequence.erase(s.sequence.begin() + best_i + 2);
            s.sequence.erase(s.sequence.begin() + best_i + 2);
        }       

        s.valorObj += best_delta;
        return true;
    }
    return false;

    case 3: //OR-OPT3
    for(i =  1; i < s.sequence.size()- 3; i++)
    {
        for(j = 1; j < s.sequence.size() - 3; j++)
        {
            if(j > i)
            {
                delta = -data.getDistance(s.sequence[i-1], s.sequence[i]) - data.getDistance(s.sequence[i+2], s.sequence[i+3])
                        - data.getDistance(s.sequence[j], s.sequence[j + 1]) + data.getDistance(s.sequence[i-1],s.sequence[i+3])
                        + data.getDistance(s.sequence[j], s.sequence[i]) + data.getDistance(s.sequence[i+2], s.sequence[j+1]);
            }
            else if(j < i)
            {
                delta = -data.getDistance(s.sequence[j-1], s.sequence[j]) - data.getDistance(s.sequence[j+2], s.sequence[j+3])
                        - data.getDistance(s.sequence[i], s.sequence[i+1]) + data.getDistance(s.sequence[j-1], s.sequence[j+3])
                        + data.getDistance(s.sequence[i], s.sequence[j]) + data.getDistance(s.sequence[j+2], s.sequence[i+1]);
            }
            else if(j == i-3)
            {
                delta = -data.getDistance(s.sequence[i-1], s.sequence[i]) - data.getDistance(s.sequence[i+2], s.sequence[j])
                        + data.getDistance(s.sequence[i-1], s.sequence[j]) + data.getDistance(s.sequence[i+2], s.sequence[j+1]);
            }
            else
            {
                continue;
            }
            if(delta < best_delta)
            {
                best_delta = delta;
                best_i = i;
                best_j = j;
            }
        }
    }
    if(best_delta < 0)  //inserindo sempre depois do j
    {
        if(best_j > best_i)
        {
            s.sequence.insert(s.sequence.begin() + best_j + 1, s.sequence[best_i]);
            s.sequence.insert(s.sequence.begin() + best_j + 2, s.sequence[best_i + 1]);
            s.sequence.insert(s.sequence.begin() + best_j + 3, s.sequence[best_i + 2]);
            s.sequence.erase(s.sequence.begin() + best_i+2);
            s.sequence.erase(s.sequence.begin() + best_i + 1);
            s.sequence.erase(s.sequence.begin() + best_i);
        }
        else
        {
            s.sequence.insert(s.sequence.begin() + best_j, s.sequence[best_i]);
            s.sequence.insert(s.sequence.begin() + best_j + 1, s.sequence[best_i + 1]);
            s.sequence.insert(s.sequence.begin() + best_j + 2, s.sequence[best_i + 2]);
            s.sequence.erase(s.sequence.begin() + best_i + 3);
            s.sequence.erase(s.sequence.begin() + best_i + 3);
            s.sequence.erase(s.sequence.begin() + best_i + 3);
        }
        s.valorObj += best_delta;
        return true;
    }
    return false;
    }
    return false;
}

void BuscaLocal(Solucao &s, Data &data, int a)
{
    vector<int> NL = {1, 2, 3, 4, 5};  
    bool improved = false;
    int n;
    int iterA = 0;
    const int maxiter = a;
    while(NL.empty() == false && iterA < maxiter)
    {
        n = rand() % NL.size();
  
        switch (NL[n])
        {
            case 1:
                improved = bestImprovementSwap(s, data); // SWAP
                break;
            case 2:
                improved = bestImprovement2Opt(s, data); // 2-OPT
                break;
            case 3:
               improved = bestImprovementOrOpt(s, data, 1); // OR-OPT- REINSERTION
               break;
            case 4:
                improved = bestImprovementOrOpt(s, data, 2); // OR-OPT2
                break;
            case 5:
                improved = bestImprovementOrOpt(s, data, 3); // OR-OPT3
                break;
        }

         
        
        if(improved)
        {
            NL = {1, 2, 3, 4, 5};
            iterA++;
        }
        else
        {
            NL.erase(NL.begin() + n);
        }
    }
}


Solucao Perturbacao(const Solucao s, Data &data)
{
    Solucao pert = s;
    int i = 0, j = 0;

    int n = pert.sequence.size() - 1;

    int tam1, tam2;
    bool mesmo = 1;
    
    
    while(mesmo)// não se sobrepor
    {
        
        if((((i + tam1 - 1) < j) || ((j + tam2 - 1) < i)) && i+tam1 -1 < n && j+tam2-1 < n)
        {
            break;
        }
        //tamanho das subsequencias
        tam1 = rand() %((int)ceil(n-1/10)) + 2;
        tam2 = rand() %((int)ceil(n-1/10)) + 2;
        
        //escolher os indices de inicio das subsequencias
        i = rand() % (n - 2) + 1;
        j = rand() % (n - 2) + 1;

    } 
    

    //pegar as subsequencias

    int vi = pert.sequence[i];
    int vi_next = pert.sequence[i + tam1];
    int vi_quase = pert.sequence[i + tam1 - 1];
    int vi_prev = pert.sequence[i-1];

        
    int vj = pert.sequence[j];
    int vj_next = pert.sequence[j + tam2];
    int vj_quase = pert.sequence[j + tam2 - 1];
    int vj_prev = pert.sequence[j-1];


    if(j - (i + tam1 - 1) == 1)
    {
        pert.valorObj += -data.getDistance(vi_prev, vi) - data.getDistance(vj_prev, vj) - data.getDistance(vj_quase, vj_next)
                         +data.getDistance(vi_prev, vj) + data.getDistance(vi_next, vj_quase) + data.getDistance(vj_prev, vi);
    }
    else if(i - (j + tam2 - 1) == 1)
    {
        pert.valorObj += -data.getDistance(vj_prev, vj) - data.getDistance(vi_prev, vi) - data.getDistance(vi_quase, vi_next)
                         +data.getDistance(vj_prev, vi) + data.getDistance(vj_next, vi_quase) + data.getDistance(vi_prev, vj);
    }
    else
    {
       pert.valorObj += -data.getDistance(vi_prev, vi) - data.getDistance(vj_prev,vj) - data.getDistance(vi_quase, vi_next)
                      - data.getDistance(vj_quase, vj_next) + data.getDistance(vi_prev, vj) + data.getDistance(vj_quase, vi_next)
                      + data.getDistance(vj_prev, vi) + data.getDistance(vi_quase, vj_next);
    }

    if(i < j)
    {
        pert.sequence.insert(pert.sequence.begin() + j, pert.sequence.begin() + i, pert.sequence.begin() + i + tam1);
        pert.sequence.erase(pert.sequence.begin() + i, pert.sequence.begin() + i + tam1);
        pert.sequence.insert(pert.sequence.begin() + i, pert.sequence.begin() + j + tam2, pert.sequence.begin() + j + tam2 + tam2);
        pert.sequence.erase(pert.sequence.begin() + j + tam2, pert.sequence.begin() + j + tam2 + tam2);
    }
    else if(i > j)
    {
        pert.sequence.insert(pert.sequence.begin() + i, pert.sequence.begin() + j, pert.sequence.begin() + j + tam2);
        pert.sequence.erase(pert.sequence.begin() + j, pert.sequence.begin() + j + tam2);
        pert.sequence.insert(pert.sequence.begin() + j, pert.sequence.begin() + i + tam1, pert.sequence.begin() + i + tam1 + tam1);
        pert.sequence.erase(pert.sequence.begin() + i + tam1, pert.sequence.begin() + i + tam1 + tam1);
    }

    return pert;
}


Solucao ILS(int maxIter, int maxIterIls,int n,  Data &data)
{
    Solucao bestOfAll;

    bestOfAll.valorObj = INFINITY;
    int iterILS;

    for(int i = 0; i < maxIter; i++)
    {
        Solucao s = Construcao(data, n);
        
        Solucao best = s;
    
        iterILS = 0;
    
        while (iterILS <= maxIterIls)
        {
            
            BuscaLocal(s, data, n);
            calcularValorObj(&s, data);
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
    calcularValorObj(&bestOfAll, data);
    return bestOfAll;
}

int main(int argc, char** argv) 
{

    srand(time(NULL));

    Solucao constr;
    Solucao ilts;

    int maxIterIls;
    double cto = 0.0;
    
    auto data = Data(argc, argv[1]);
    data.read();
    size_t n = data.getDimension();

    if(n >= 150)
    {
        maxIterIls = (n)/2;
    }
    else
    {
        maxIterIls = n;
    }

    // cout << "Dimension: " << n << endl;
    // cout << "DistanceMatrix: " << endl;
    // data.printMatrixDist();

    //cout << "Exemplo de Solucao s = ";
    auto start = high_resolution_clock::now();
    for(int k = 0; k < 10; k++)
    {
        ilts = ILS(50, maxIterIls, n, data);
        cto += ilts.valorObj;
    }
    auto stop = high_resolution_clock::now();
    auto elapsed = stop - start;
    
    exibirSolucao(&ilts);
    cout << "Tempo de execucao: " << duration_cast<milliseconds>(elapsed).count()/10 << "ms" << endl;
    cout << "Valor: "<< cto/10 << endl;
    
    
    return 0;
}