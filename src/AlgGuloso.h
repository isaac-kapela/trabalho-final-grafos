#ifndef ALG_GULOSO_H
#define ALG_GULOSO_H

#include "Grafo.h"
#include <set>
#include <vector>
#include <utility>

class AlgGuloso {
private:
    struct UnionFind {
        std::vector<int> pai;
        std::vector<int> rank;

        UnionFind(int n) {
            pai.resize(n);
            rank.assign(n, 0);
            for (int i = 0; i < n; i++) pai[i] = i;
        }

        int find(int i) {
            if (pai[i] == i) return i;
            return pai[i] = find(pai[i]);
        }

        void unite(int i, int j) {
            int raiz_i = find(i);
            int raiz_j = find(j);
            if (raiz_i != raiz_j) {
                if (rank[raiz_i] < rank[raiz_j]) pai[raiz_i] = raiz_j;
                else if (rank[raiz_i] > rank[raiz_j]) pai[raiz_j] = raiz_i;
                else {
                    pai[raiz_j] = raiz_i;
                    rank[raiz_i]++;
                }
            }
        }
    };

    struct ArestaIndexada {
        Aresta aresta;
        int indiceOriginal;
    };

public:
    static std::pair<double, std::set<int>> executarGuloso(const Grafo& grafo, int grauMaximo);

    static std::pair<double, std::set<int>> executarGulosoRandomizado(const Grafo& grafo, int grauMaximo, double alpha);

    static std::pair<double, std::set<int>> resolverRandomizado(const Grafo& grafo, int grauMaximo, double alpha, int iteracoes = 30);
};

#endif