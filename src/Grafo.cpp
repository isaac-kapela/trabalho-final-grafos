#include "Grafo.h"
#include <fstream>
#include <stdexcept>
#include <algorithm>

Grafo::Grafo(int n) : numeroVertices(n), numeroArestas(0) {
    listaAdjacencia.resize(n);
}

void Grafo::adicionarAresta(int origem, int destino, double peso) {
    if (origem < 0 || origem >= numeroVertices || destino < 0 || destino >= numeroVertices) {
        throw std::out_of_range("Vértice fora do intervalo válido");
    }
    listaAdjacencia[origem].push_back({destino, peso});
    listaAdjacencia[destino].push_back({origem, peso});
    arestas.push_back(Aresta(origem, destino, peso));
    numeroArestas++;
}

int Grafo::getNumeroVertices() const {
    return numeroVertices;
}

int Grafo::getNumeroArestas() const {
    return numeroArestas;
}

const std::vector<std::pair<int, double>>& Grafo::getVizinhos(int vertice) const {
    if (vertice < 0 || vertice >= numeroVertices) {
        throw std::out_of_range("Vértice fora do intervalo válido");
    }
    return listaAdjacencia[vertice];
}

const std::vector<Aresta>& Grafo::getArestas() const {
    return arestas;
}

int Grafo::getGrauNaSolucao(int vertice, const std::set<int>& solucao) const {
    int grau = 0;
    for (int idxAresta : solucao) {
        const Aresta& a = arestas[idxAresta];
        if (a.origem == vertice || a.destino == vertice) {
            grau++;
        }
    }
    return grau;
}

void Grafo::dfs(int vertice, std::vector<bool>& visitado) const {
    visitado[vertice] = true;
    for (const auto& vizinho : listaAdjacencia[vertice]) {
        int v = vizinho.first;
        if (!visitado[v]) {
            dfs(v, visitado);
        }
    }
}

bool Grafo::ehConexo() const {
    if (numeroVertices == 0) return true;
    std::vector<bool> visitado(numeroVertices, false);
    dfs(0, visitado);
    for (bool v : visitado) {
        if (!v) return false;
    }
    return true;
}

void Grafo::dfsNaSolucao(int vertice, const std::set<int>& solucao, std::vector<bool>& visitado) const {
    visitado[vertice] = true;
    for (int idxAresta : solucao) {
        const Aresta& a = arestas[idxAresta];
        int vizinho = -1;
        if (a.origem == vertice && !visitado[a.destino]) {
            vizinho = a.destino;
        } else if (a.destino == vertice && !visitado[a.origem]) {
            vizinho = a.origem;
        }
        if (vizinho != -1) {
            dfsNaSolucao(vizinho, solucao, visitado);
        }
    }
}

double Grafo::calcularCusto(const std::set<int>& solucao) const {
    double custo = 0.0;
    for (int idxAresta : solucao) {
        custo += arestas[idxAresta].peso;
    }
    return custo;
}

bool Grafo::ehArvoreGeradoraValida(const std::set<int>& solucao, int grauMaximo) const {
    if (solucao.size() != static_cast<size_t>(numeroVertices - 1)) {
        return false;
    }
    for (int v = 0; v < numeroVertices; v++) {
        if (getGrauNaSolucao(v, solucao) > grauMaximo) {
            return false;
        }
    }
    std::vector<bool> visitado(numeroVertices, false);
    int inicio = -1;
    for (int idxAresta : solucao) {
        inicio = arestas[idxAresta].origem;
        break;
    }
    if (inicio == -1) return false;
    dfsNaSolucao(inicio, solucao, visitado);
    for (bool v : visitado) {
        if (!v) return false;
    }
    return true;
}

void Grafo::definirNomeInstancia(const std::string& nome) {
    nomeInstancia = nome;
}

std::string Grafo::getNomeInstancia() const {
    return nomeInstancia;
}

void Grafo::exportarParaGraphEditor(const std::string& arquivoSaida) const {
    std::ofstream arquivo(arquivoSaida);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Não foi possível abrir o arquivo para escrita: " + arquivoSaida);
    }
    arquivo << numeroVertices << "\n";
    for (const Aresta& a : arestas) {
        arquivo << (a.origem + 1) << " " << (a.destino + 1) << " " << a.peso << "\n";
    }
    arquivo.close();
}

void Grafo::exportarSolucaoParaGraphEditor(const std::set<int>& solucao, const std::string& arquivoSaida) const {
    std::ofstream arquivo(arquivoSaida);
    if (!arquivo.is_open()) {
        throw std::runtime_error("Não foi possível abrir o arquivo para escrita: " + arquivoSaida);
    }
    arquivo << "# Solução DC-MST - " << nomeInstancia << "\n";
    arquivo << "# Custo: " << calcularCusto(solucao) << "\n";
    arquivo << "# Número de arestas: " << solucao.size() << "\n";
    arquivo << numeroVertices << "\n";
    for (int idxAresta : solucao) {
        const Aresta& a = arestas[idxAresta];
        arquivo << (a.origem + 1) << " " << (a.destino + 1) << " " << a.peso << "\n";
    }
    arquivo.close();
}
