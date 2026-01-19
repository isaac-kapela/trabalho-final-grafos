#include "AlgGRandReativo.h"
#include <iostream>
#include <algorithm>
#include <cmath>

GeradoraMinimaGRR::GeradoraMinimaGRR(const Grafo& g, int d, std::vector<double> opcoesAlfas) 
    : grafo(g), grauMaximo(d) {
    preProcessarGrafo();
    inicializarAlfas(opcoesAlfas);
}

void GeradoraMinimaGRR::preProcessarGrafo() {
    int n = grafo.getNumeroVertices();
    adjPreProcessada.resize(n);
    
    const std::vector<Aresta>& todasArestas = grafo.getArestas();
    for (size_t i = 0; i < todasArestas.size(); ++i) {
        int u = todasArestas[i].origem;
        int v = todasArestas[i].destino;
        double w = todasArestas[i].peso;
        
        adjPreProcessada[u].push_back({v, (int)i, w});
        adjPreProcessada[v].push_back({u, (int)i, w});
    }
}

void GeradoraMinimaGRR::inicializarAlfas(const std::vector<double>& opcoesAlfas) {
    if (opcoesAlfas.empty()) {
        alfas.push_back({0.1, 0.0, 0, 1.0});
        return;
    }
    double probInicial = 1.0 / opcoesAlfas.size();

    for (double v : opcoesAlfas) {
        alfas.push_back({v, 0.0, 0, probInicial});
    }
}

int GeradoraMinimaGRR::selecionarIndiceAlfa() {
    double r = UtilRandomico::realAleatorio();
    double acumulada = 0.0;
    
    for (size_t i = 0; i < alfas.size(); ++i) {
        acumulada += alfas[i].probabilidade;
        if (r <= acumulada) {
            return i;
        }
    }
    return alfas.size() - 1;
}

DadosSolucao GeradoraMinimaGRR::construirSolucao(double alpha) {
    int n = grafo.getNumeroVertices();
    std::set<int> arestasArvore;
    std::vector<bool> naArvore(n, false);
    std::vector<int> graus(n, 0);
    double custoTotal = 0;
    
    int raiz = UtilRandomico::inteiroAleatorio(0, n - 1);
    naArvore[raiz] = true;
    
    struct Candidato {
        double peso;
        int u; 
        int v;
        int idx;
    };
    
    std::vector<Candidato> candidatos;
    
    for (const auto& aresta : adjPreProcessada[raiz]) {
        candidatos.push_back({aresta.peso, raiz, aresta.vizinho, aresta.indiceAresta});
    }
    
    for (int i = 0; i < n - 1; ++i) {
        std::vector<Candidato> validos;
        double minW = std::numeric_limits<double>::max();
        double maxW = -std::numeric_limits<double>::max();
        
        for (const auto& c : candidatos) {
            if (!naArvore[c.v] && graus[c.u] < grauMaximo) {
                validos.push_back(c);
                if (c.peso < minW) minW = c.peso;
                if (c.peso > maxW) maxW = c.peso;
            }
        }
        
        if (validos.empty()) {
            return {{}, 0.0, false};
        }
        
        double limite = minW + alpha * (maxW - minW);
        std::vector<int> rclIndices;
        
        for (size_t k = 0; k < validos.size(); ++k) {
            if (validos[k].peso <= limite) {
                rclIndices.push_back(k);
            }
        }
        
        int escolhidoIdx = rclIndices[UtilRandomico::inteiroAleatorio(0, rclIndices.size() - 1)];
        Candidato escolhido = validos[escolhidoIdx];
        
        arestasArvore.insert(escolhido.idx);
        custoTotal += escolhido.peso;
        naArvore[escolhido.v] = true;
        graus[escolhido.u]++;
        graus[escolhido.v]++;
        
        if (graus[escolhido.v] < grauMaximo) {
            for (const auto& aresta : adjPreProcessada[escolhido.v]) {
                if (!naArvore[aresta.vizinho]) {
                    candidatos.push_back({aresta.peso, escolhido.v, aresta.vizinho, aresta.indiceAresta});
                }
            }
        }
        
    }
    
    return {arestasArvore, custoTotal, true};
}

void GeradoraMinimaGRR::atualizarProbabilidades(double melhorCustoGlobal, double sigma) {
    std::vector<double> q(alfas.size(), 0.0);
    double sumQ = 0.0;
    
    for (size_t i = 0; i < alfas.size(); ++i) {
        if (alfas[i].numSolucoes > 0) {
            double media = alfas[i].somaCustos / alfas[i].numSolucoes;
            q[i] = std::pow(melhorCustoGlobal / media, sigma);
        } else {
            q[i] = alfas[i].probabilidade; 
        }
        sumQ += q[i];
    }
    
    for (size_t i = 0; i < alfas.size(); ++i) {
        alfas[i].probabilidade = q[i] / sumQ;
        alfas[i].somaCustos = 0;
        alfas[i].numSolucoes = 0;
    }
}

std::pair<double, std::set<int>> GeradoraMinimaGRR::resolver(int maxIteracoes, int tamanhoBloco) {
    double melhorCusto = std::numeric_limits<double>::max();
    std::set<int> melhorArvore;
    
    for (int iter = 1; iter <= maxIteracoes; ++iter) {
        int idxAlfa = selecionarIndiceAlfa();
        double alphaAtual = alfas[idxAlfa].valor;
        
        DadosSolucao sol = construirSolucao(alphaAtual);
        
        if (sol.viavel) {
            alfas[idxAlfa].somaCustos += sol.custo;
            alfas[idxAlfa].numSolucoes++;
            
            if (sol.custo < melhorCusto) {
                melhorCusto = sol.custo;
                melhorArvore = sol.arestas;
                std::cout << "[Novo Melhor] Iter: " << iter 
                          << " | Custo: " << melhorCusto 
                          << " | Alfa: " << alphaAtual << std::endl;
            }
        }
        
        if (iter % tamanhoBloco == 0 && melhorCusto < std::numeric_limits<double>::max()) {
            atualizarProbabilidades(melhorCusto);
        }
    }
    
    return {melhorCusto, melhorArvore};
}

double GeradoraMinimaGRR::getMelhorAlpha() const {
    double maiorProb = -1.0;
    double melhorVal = 0.0;
    for (const auto& a : alfas) {
        if (a.probabilidade > maiorProb) {
            maiorProb = a.probabilidade;
            melhorVal = a.valor;
        }
    }
    return melhorVal;
}