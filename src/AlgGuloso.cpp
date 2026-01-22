#include "AlgGuloso.h"
#include "UtilRandomico.h"
#include <algorithm>
#include <iostream>
#include <limits>

// --- GULOSO DETERMINÍSTICO ---
std::pair<double, std::set<int>> AlgGuloso::executarGuloso(const Grafo& grafo, int grauMaximo) {
    int numVertices = grafo.getNumeroVertices();
    const auto& arestasOriginais = grafo.getArestas();
    
    // Preparar arestas com seus índices originais
    std::vector<ArestaIndexada> arestas;
    for (size_t i = 0; i < arestasOriginais.size(); ++i) {
        arestas.push_back({arestasOriginais[i], (int)i});
    }

    // Ordenar pelo peso (menor para maior) - Passo 1 do Kruskal
    std::sort(arestas.begin(), arestas.end(), [](const ArestaIndexada& a, const ArestaIndexada& b) {
        return a.aresta.peso < b.aresta.peso;
    });

    UnionFind uf(numVertices);
    std::vector<int> grau(numVertices, 0);
    std::set<int> solucao;
    double custoTotal = 0.0;
    int arestasAdicionadas = 0;

    // Passo 2: Iterar e adicionar
    for (const auto& item : arestas) {
        int u = item.aresta.origem;
        int v = item.aresta.destino;

        // Verifica Ciclo e Restrição de Grau
        if (uf.find(u) != uf.find(v) && grau[u] < grauMaximo && grau[v] < grauMaximo) {
            uf.unite(u, v);
            grau[u]++;
            grau[v]++;
            solucao.insert(item.indiceOriginal);
            custoTotal += item.aresta.peso;
            arestasAdicionadas++;

            if (arestasAdicionadas == numVertices - 1) break; // Árvore completa
        }
    }

    // Se não formou árvore, retorna custo infinito
    if (arestasAdicionadas < numVertices - 1) {
        return {std::numeric_limits<double>::max(), {}};
    }

    return {custoTotal, solucao};
}

// --- GULOSO RANDOMIZADO (1 iteração) ---
std::pair<double, std::set<int>> AlgGuloso::executarGulosoRandomizado(const Grafo& grafo, int grauMaximo, double alpha) {
    int numVertices = grafo.getNumeroVertices();
    const auto& arestasOriginais = grafo.getArestas();
    
    std::vector<ArestaIndexada> arestasRestantes;
    for (size_t i = 0; i < arestasOriginais.size(); ++i) {
        arestasRestantes.push_back({arestasOriginais[i], (int)i});
    }

    UnionFind uf(numVertices);
    std::vector<int> grau(numVertices, 0);
    std::set<int> solucao;
    double custoTotal = 0.0;
    int arestasAdicionadas = 0;

    while (arestasAdicionadas < numVertices - 1 && !arestasRestantes.empty()) {
        // 1. Filtrar arestas válidas (não formam ciclo e respeitam grau)
        std::vector<ArestaIndexada> candidatosValidos;
        double min_peso = std::numeric_limits<double>::max();
        double max_peso = -std::numeric_limits<double>::max();

        for (const auto& item : arestasRestantes) {
            int u = item.aresta.origem;
            int v = item.aresta.destino;
            if (uf.find(u) != uf.find(v) && grau[u] < grauMaximo && grau[v] < grauMaximo) {
                candidatosValidos.push_back(item);
                min_peso = std::min(min_peso, item.aresta.peso);
                max_peso = std::max(max_peso, item.aresta.peso);
            }
        }

        if (candidatosValidos.empty()) break; // Sem saída (grafo desconexo ou restrição muito rígida)

        // 2. Criar a Lista de Candidatos Restrita (LCR)
        double limite = min_peso + alpha * (max_peso - min_peso);
        std::vector<ArestaIndexada> lcr;
        for (const auto& item : candidatosValidos) {
            if (item.aresta.peso <= limite) {
                lcr.push_back(item);
            }
        }

        // 3. Escolher aleatoriamente da LCR
        int indexSorteado = UtilRandomico::inteiroAleatorio(0, lcr.size() - 1);
        ArestaIndexada escolhida = lcr[indexSorteado];

        // 4. Adicionar à solução
        int u = escolhida.aresta.origem;
        int v = escolhida.aresta.destino;
        uf.unite(u, v);
        grau[u]++;
        grau[v]++;
        solucao.insert(escolhida.indiceOriginal);
        custoTotal += escolhida.aresta.peso;
        arestasAdicionadas++;

        // Remover a aresta escolhida da lista de restantes
        auto it = std::remove_if(arestasRestantes.begin(), arestasRestantes.end(), [&](const ArestaIndexada& a) {
            return a.indiceOriginal == escolhida.indiceOriginal;
        });
        arestasRestantes.erase(it, arestasRestantes.end());
    }

    if (arestasAdicionadas < numVertices - 1) return {std::numeric_limits<double>::max(), {}};
    return {custoTotal, solucao};
}

// --- WRAPPER PARA RODAR N ITERAÇÕES (padrão 30) ---
std::pair<double, std::set<int>> AlgGuloso::resolverRandomizado(const Grafo& grafo, int grauMaximo, double alpha, int iteracoes) {
    double melhorCusto = std::numeric_limits<double>::max();
    std::set<int> melhorSolucao;

    // Especificação: "No algoritmo randomizado, o algoritmo construtivo deve ser chamado pelo menos 30 vezes."
    for (int i = 0; i < iteracoes; ++i) {
        auto solAtual = executarGulosoRandomizado(grafo, grauMaximo, alpha);
        
        // Se achou uma solução válida e melhor que a atual
        if (solAtual.first < melhorCusto) {
            melhorCusto = solAtual.first;
            melhorSolucao = solAtual.second;
        }
    }

    return {melhorCusto, melhorSolucao};
}