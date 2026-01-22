#include "LeitorInstancia.h"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

// --- FUNÇÕES AUXILIARES ---
std::string LeitorInstancia::extrairNomeInstancia(const std::string& arquivo) {
    size_t posUltimaBarra = arquivo.find_last_of("/\\");
    size_t posPonto = arquivo.find_last_of(".");
    if (posUltimaBarra == std::string::npos) posUltimaBarra = 0; else posUltimaBarra++;
    if (posPonto == std::string::npos || posPonto < posUltimaBarra) return arquivo.substr(posUltimaBarra);
    return arquivo.substr(posUltimaBarra, posPonto - posUltimaBarra);
}
// --- LÓGICA DE DETECÇÃO ROBUSTA ---
Grafo LeitorInstancia::lerInstancia(const std::string& arquivo, int& grauMaximo) {
    std::string nomeInst = extrairNomeInstancia(arquivo);
    
    // Tenta extrair N do nome (ex: rand200 -> 200)
    int N_nome = 0;
    std::string numStr;
    bool digitFound = false;
    for(char c : nomeInst) {
        if(isdigit(c)) { numStr += c; digitFound = true; }
        else if(digitFound) break;
    }
    if(!numStr.empty()) N_nome = std::stoi(numStr);

    std::ifstream file(arquivo);
    if (!file.is_open()) throw std::runtime_error("Erro ao abrir: " + arquivo);

    // 1. Ler TUDO para a memória (seguro e rápido)
    std::vector<double> tokens;
    double val;
    while (file >> val) tokens.push_back(val);
    file.close();

    if (tokens.empty()) throw std::runtime_error("Arquivo vazio: " + arquivo);

    // --- DETECÇÃO INTELIGENTE POR TAMANHO ---
    int N = (N_nome > 0) ? N_nome : 0;
    size_t total = tokens.size();
    size_t k = 0;

    // Pula cabeçalho se o primeiro token for igual a N
    if (N > 0 && total > 0 && std::abs(tokens[0] - N) < 0.1) {
        k++;
        total--;
    }

    // Se N não veio do nome, estima pela raiz quadrada (para matrizes)
    if (N == 0) N = (int)std::sqrt(total * 2);

    // CRITÉRIO MATEMÁTICO:
    // Coordenadas tem ~2*N tokens. Matriz tem ~N*N/2 tokens.
    // Se tiver muito mais que 3*N, é Matriz com certeza.
    bool ehMatriz = (total > (size_t)(3 * N));

    std::cout << "[Leitor] " << nomeInst << " (N=" << N << ") -> Detectado: " 
              << (ehMatriz ? "MATRIZ (Pesos)" : "COORDENADAS (Euclidiano)") << std::endl;

    Grafo grafo(N);
    grafo.definirNomeInstancia(nomeInst);

    if (ehMatriz) {

        std::cout << "DEBUG: Primeiros 5 tokens brutos: ";
        for(size_t x=0; x<5 && x<tokens.size(); x++) std::cout << tokens[x] << " ";
        std::cout << std::endl;
        
        std::cout << "DEBUG: K começa em: " << k << " (Token[k] = " << tokens[k] << ")" << std::endl;

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < i; ++j) { // Triângulo superior
                if (k < tokens.size()) {
                    grafo.adicionarAresta(i, j, tokens[k++]);
                }
            }
        }
    } else {
        // Leitura de Coordenadas (str, crd)
        std::vector<std::pair<double, double>> coords;
        while (k + 1 < tokens.size() && (int)coords.size() < N) {
            coords.push_back({tokens[k], tokens[k+1]});
            k += 2;
        }
        // Gera Grafo Completo com distâncias
        for (int i = 0; i < N; ++i) {
            for (int j = i + 1; j < N; ++j) {
                double dist = std::sqrt(std::pow(coords[i].first - coords[j].first, 2) + 
                                      std::pow(coords[i].second - coords[j].second, 2));
                grafo.adicionarAresta(i, j, dist);
            }
        }
    }
    return grafo;
}

Grafo LeitorInstancia::lerInstanciaORLibrary(const std::string& arquivo, int grauMaximo) {
    int g = grauMaximo;
    return lerInstancia(arquivo, g);
}