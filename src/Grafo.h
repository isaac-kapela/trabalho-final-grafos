#ifndef GRAFO_H
#define GRAFO_H

#include <vector>
#include <string>
#include <set>

/**
 * Estrutura para representar uma aresta
 */
struct Aresta {
    int origem;           // vértice origem
    int destino;          // vértice destino
    double peso;          // peso da aresta

    Aresta(int origem, int destino, double peso) 
        : origem(origem), destino(destino), peso(peso) {}

    bool operator<(const Aresta& outra) const {
        return peso < outra.peso;
    }
};

/**
 * Tipo Abstrato de Dados: Grafo
 * Representa um grafo não-direcionado ponderado nas arestas
 * Adequado para o problema DC-MST
 */
class Grafo {
private:
    int numeroVertices;
    int numeroArestas;
    std::vector<std::vector<std::pair<int, double>>> listaAdjacencia;
    std::vector<Aresta> arestas;
    std::string nomeInstancia;

public:
    Grafo(int n);
    void adicionarAresta(int origem, int destino, double peso);
    
    int getNumeroVertices() const;
    int getNumeroArestas() const;
    const std::vector<std::pair<int, double>>& getVizinhos(int vertice) const;
    const std::vector<Aresta>& getArestas() const;
    int getGrauNaSolucao(int vertice, const std::set<int>& solucao) const;
    
    bool ehConexo() const;
    double calcularCusto(const std::set<int>& solucao) const;
    bool ehArvoreGeradoraValida(const std::set<int>& solucao, int grauMaximo) const;
    
    void definirNomeInstancia(const std::string& nome);
    std::string getNomeInstancia() const;
    
    void exportarParaGraphEditor(const std::string& arquivoSaida) const;
    void exportarSolucaoParaGraphEditor(const std::set<int>& solucao, const std::string& arquivoSaida) const;

private:
    void dfs(int vertice, std::vector<bool>& visitado) const;
    void dfsNaSolucao(int vertice, const std::set<int>& solucao, std::vector<bool>& visitado) const;
};

#endif // GRAFO_H
