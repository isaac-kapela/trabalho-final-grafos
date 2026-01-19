#ifndef ALGGRANDREATIVO_H
#define ALGGRANDREATIVO_H

#include "Grafo.h"
#include "UtilRandomico.h"
#include <vector>
#include <set>
#include <limits>

struct AlphaStats {
    double valor;
    double somaCustos;
    int numSolucoes;
    double probabilidade;
};

struct DadosSolucao {
    std::set<int> arestas;
    double custo;
    bool viavel;
};

class GeradoraMinimaGRR {
private:
    const Grafo& grafo;
    int grauMaximo;
    
    struct ArestaInfo {
        int vizinho;
        int indiceAresta;
        double peso;
    };
    std::vector<std::vector<ArestaInfo>> adjPreProcessada;

    std::vector<AlphaStats> alfas;
    
    void preProcessarGrafo();
    void inicializarAlfas(const std::vector<double>& opcoesAlfas);
    int selecionarIndiceAlfa();
    void atualizarProbabilidades(double melhorCustoGlobal, double sigma = 10.0);
    
    DadosSolucao construirSolucao(double alpha);

public:
    GeradoraMinimaGRR(const Grafo& g, int d, std::vector<double> opcoesAlfas);
    
    std::pair<double, std::set<int>> resolver(int maxIteracoes, int tamanhoBloco);
    
    double getMelhorAlpha() const;
};

#endif