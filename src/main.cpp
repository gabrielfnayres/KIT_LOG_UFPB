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

void showSolution(Solution &s)
{

    for(int i = 0; i < s.sequence.size(); i++)
    {
        cout << s.sequence[i] << "->";
        cout << s.sequence.back() << endl;
    }
}

void objCost(Solution &s, Data &data)
{
    s.valorObj = 0;
    for(int i = 0; i < s.sequence.size() - 1; i++)
    {
        s.valorObj += data.getDistance(s.sequence[i], s.sequence[i+1]);
    }
}

vector<InsertionInfo> CalcularCustoDeInsercao(Solution &s, vector<int> &CL, Data &data)
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

vector<int> nosRestantes(const vector<int> &nosselecionados, const int &tamanho)
{
    vector<int> CL(tamanho - (nosselecionados.size() - 1));

    int count = 0; 

    for(int i = 1; i <= tamanho; i++)
    {
        if((i != nosselecionados[0]) && (i != nosselecionados[1]) && (i != nosselecionados[2]))
        {
            CL[count++] = i;
        }
    }
    return CL;

}

vector<InsertionInfo> ordenarEmOrdemCrescente(vector<InsertionInfo> &custoInsercao,vector<int> &sequencia)
{

    vector<InsertionInfo> CustoOrdenado = custoInsercao;

    for (int i = 0; i < sequencia.size() - 1; i++) 
    {
        for (int j = 0; j < sequencia.size() - 1 - i; j++) 
        {
            if (CustoOrdenado[j].custo > CustoOrdenado[j + 1].custo) 
            {
                swap(CustoOrdenado[j], CustoOrdenado[j + 1]);
            }
        }
    }

    return CustoOrdenado;
}

void inserirNaSolucao(Solution &s, vector<int> &CL, InsertionInfo &custoInsercao)
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

Solution Construcao(int &dimensao, Data &data)
{
    Solution s = {{}, 0.0};
    s.sequence = escolher3NosAleatorios(s);
    vector<int> CL = nosRestantes(s.sequence,dimensao);

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

        for(int j = i + 2; j < s->sequence.size(); j++)
        {
            int vj = s->sequence[j];
            int vj_next = s->sequence[j+1];
            int vj_prev = s->sequence[j-1];

            double delta = -data.getDistance(vi_prev,vi) - data.getDistance(vi,vi_next) + data.getDistance(vi_prev,vj) + data.getDistance(vj,vi_next) - data.getDistance(vj_prev,vj) - data.getDistance(vj,vj_next) + data.getDistance(vj_prev,vi) + data.getDistance(vi,vj_next); 

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


int main(int argc, char** argv)
{

    auto data = Data(argc, argv[1]);
    data.read();
    size_t n = data.getDimension();

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

