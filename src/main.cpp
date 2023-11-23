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

vector<int> nosRestantes(vector<int> &nosselecionados, int &tamanho)
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

void inserirNaSolucao(Solution &s, vector<int> &CL, vector<InsertionInfo> &custoInsercao)
{
    for(int i = 0; i < CL.size(); i++)
    {   
        s.sequence.insert(s.sequence.begin() + custoInsercao[i].arestaRemovida + 1, custoInsercao[i].noInserido);
        s.valorObj += custoInsercao[i].custo;
        CL.erase(CL.begin() + i + 1);   
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
        ordenarEmOrdemCrescente(custoInsercao, s.sequence);
        double alpha = (double) rand() / RAND_MAX;
        int selecionado = rand() % ((int) ceil(alpha * custoInsercao.size()));
        inserirNaSolucao(s, CL, custoInsercao);
    }
    
 
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

