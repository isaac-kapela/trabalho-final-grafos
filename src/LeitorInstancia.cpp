#include "LeitorInstancia.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <limits>

// --- FUNÇÕES AUXILIARES ---

int extrairNumeroDoNome(const std::string& nome) {
    std::string numStr;
    bool encontrou = false;
    for (char c : nome) {
        if (isdigit(c)) {
            numStr += c;
            encontrou = true;
        } else if (encontrou) {
            break; 
        }
    }
    return numStr.empty() ? 0 : std::stoi(numStr);
}

std::string LeitorInstancia::extrairNomeInstancia(const std::string& arquivo) {
    size_t posUltimaBarra = arquivo.find_last_of("/\\");
    size_t posPonto = arquivo.find_last_of(".");
    if (posUltimaBarra == std::string::npos) posUltimaBarra = 0; else posUltimaBarra++;
    if (posPonto == std::string::npos || posPonto < posUltimaBarra) return arquivo.substr(posUltimaBarra);
    return arquivo.substr(posUltimaBarra, posPonto - posUltimaBarra);
}

double LeitorInstancia::calcularDistanciaEuclidiana(double x1, double y1, double x2, double y2) {
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

// --- LÓGICA DE DETECÇÃO ROBUSTA ---

Grafo LeitorInstancia::lerInstancia(const std::string& arquivo, int& grauMaximo) {
    std::string nomeInst = extrairNomeInstancia(arquivo);
    int nDoNome = extrairNumeroDoNome(nomeInst);
    
    std::ifstream file(arquivo);
    if (!file.is_open()) throw std::runtime_error("Erro ao abrir: " + arquivo);

    // 1. Ler TUDO para a memória (seguro e rápido)
    std::vector<double> tokens;
    double val;
    while (file >> val) tokens.push_back(val);
    file.close();

    if (tokens.empty()) throw std::runtime_error("Arquivo vazio: " + arquivo);

    // 2. Tentar identificar o formato pelo nome ou conteúdo
    bool ehCoordenada = (nomeInst.find("str") == 0);
    bool ehMatriz = (nomeInst.find("rand") == 0 || nomeInst.find("sym") == 0);
    bool ehLista = (nomeInst.find("crd") == 0 || nomeInst.find("shrd") == 0);

    // Se o nome não ajudar, usa heurística de tamanho
    if (!ehCoordenada && !ehMatriz && !ehLista) {
        if (nDoNome > 0) {
            if (tokens.size() <= (size_t)nDoNome * 3) ehCoordenada = true;
            else if (tokens.size() > (size_t)nDoNome * nDoNome / 3) ehMatriz = true;
            else ehLista = true;
        } else {
            ehLista = true; // Padrão mais comum
        }
    }

    Grafo grafo(nDoNome > 0 ? nDoNome : 1); // Inicializa provisório, ajusta depois
    grafo.definirNomeInstancia(nomeInst);

    // --- CASE 1: STR (Coordenadas) ---
    if (ehCoordenada) {
        // Limpa cabeçalho se houver (N no inicio)
        if (tokens.size() % 2 != 0) tokens.erase(tokens.begin());
        else if (nDoNome > 0 && (int)tokens[0] == nDoNome) tokens.erase(tokens.begin());

        int N = tokens.size() / 2;
        grafo = Grafo(N); // Reinicia com tamanho certo
        grafo.definirNomeInstancia(nomeInst);

        std::vector<std::pair<double, double>> pts;
        for(size_t i=0; i<tokens.size(); i+=2) pts.push_back({tokens[i], tokens[i+1]});

        for(int i=0; i<N; ++i) {
            for(int j=i+1; j<N; ++j) {
                double dist = calcularDistanciaEuclidiana(pts[i].first, pts[i].second, pts[j].first, pts[j].second);
                grafo.adicionarAresta(i, j, dist);
            }
        }
        std::cout << "[Leitor] STR (Coords): " << N << " vertices." << std::endl;
    }
    // --- CASE 2: RAND (Matriz/Triângulo) ---
    else if (ehMatriz) {
        int N = nDoNome;
        if (N <= 0) N = (int)std::sqrt(tokens.size()); // Fallback
        
        grafo = Grafo(N);
        grafo.definirNomeInstancia(nomeInst);
        
        size_t k = 0;
        // Pula cabeçalho se houver (ex: rand200 pode ter '200' no inicio)
        if (tokens.size() > (size_t)N*N && tokens[0] == N) k++;

        for (int i = 0; i < N; ++i) {
            for (int j = i + 1; j < N; ++j) { // Triângulo superior
                while (k < tokens.size() && tokens[k] == 0) k++; // Pula zeros/diagonal se houver
                if (k < tokens.size()) {
                    grafo.adicionarAresta(i, j, tokens[k++]);
                }
            }
        }
        std::cout << "[Leitor] RAND (Matriz): " << N << " vertices." << std::endl;
    }
    // --- CASE 3: CRD/SHRD (Lista de Arestas) ---
    else {
        // Tenta limpar cabeçalho (N M D ou N M)
        size_t startIdx = 0;
        int N = nDoNome > 0 ? nDoNome : (int)tokens[0];
        
        // Pula os primeiros tokens se parecerem cabeçalho
        // Geralmente são 3 tokens: Vertices, Arestas, Grau
        if (tokens.size() > 3) {
            // Se token[0] é o N, pula.
            if ((int)tokens[0] == N) startIdx++;
            // Se token[1] parece qtd arestas (muito grande), pula
            if (tokens.size() > startIdx && tokens[startIdx] > N*2) startIdx++;
            // Se token[2] é grau (pequeno), pula
            if (tokens.size() > startIdx && tokens[startIdx] < 20) {
                if (grauMaximo <= 0) grauMaximo = (int)tokens[startIdx]; // Aproveita pra ler grau
                startIdx++;
            }
        }

        // AUTO-DETECÇÃO DE INDEXAÇÃO (0-based ou 1-based)
        double minIdx = std::numeric_limits<double>::max();
        double maxIdx = 0;
        
        // Passada rápida para verificar limites
        for (size_t i = startIdx; i + 2 < tokens.size(); i += 3) {
            double u = tokens[i];
            double v = tokens[i+1];
            if (u < minIdx) minIdx = u;
            if (v < minIdx) minIdx = v;
            if (u > maxIdx) maxIdx = u;
            if (v > maxIdx) maxIdx = v;
        }

        int offset = 0;
        if (minIdx == 1) offset = -1; // É 1-based (1..N), converter para 0..N-1
        
        // Verifica se N bate
        if (maxIdx + offset >= N) N = (int)maxIdx + offset + 1; // Expande se necessário

        grafo = Grafo(N);
        grafo.definirNomeInstancia(nomeInst);

        for (size_t i = startIdx; i + 2 < tokens.size(); i += 3) {
            int u = (int)tokens[i] + offset;
            int v = (int)tokens[i+1] + offset;
            double w = tokens[i+2];

            if (u >= 0 && u < N && v >= 0 && v < N) {
                grafo.adicionarAresta(u, v, w);
            }
        }
        std::cout << "[Leitor] LISTA (Auto-Index " << (offset==-1?"1-based":"0-based") << "): " << N << " vertices." << std::endl;
    }

    return grafo;
}

// Wrapper legado
Grafo LeitorInstancia::lerInstanciaORLibrary(const std::string& arquivo, int grauMaximo) {
    int g = grauMaximo;
    return lerInstancia(arquivo, g);
}