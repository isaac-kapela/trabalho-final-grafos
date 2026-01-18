#include "LeitorInstancia.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <cmath>

Grafo LeitorInstancia::lerInstancia(const std::string& arquivo, int& grauMaximo) {
    std::ifstream file(arquivo);
    if (!file.is_open()) {
        throw std::runtime_error("Não foi possível abrir o arquivo: " + arquivo);
    }

    int numeroVertices, numeroArestas;
    file >> numeroVertices >> numeroArestas >> grauMaximo;

    Grafo grafo(numeroVertices);
    grafo.definirNomeInstancia(extrairNomeInstancia(arquivo));

    for (int i = 0; i < numeroArestas; i++) {
        int origem, destino;
        double peso;
        file >> origem >> destino >> peso;
        // Converter para indexação base 0
        grafo.adicionarAresta(origem - 1, destino - 1, peso);
    }

    file.close();

    if (!grafo.ehConexo()) {
        throw std::runtime_error("ERRO: O grafo lido não é conexo!");
    }

    return grafo;
}

std::string LeitorInstancia::extrairNomeInstancia(const std::string& arquivo) {
    size_t posUltimaBarra = arquivo.find_last_of("/\\");
    size_t posPonto = arquivo.find_last_of(".");
    
    if (posUltimaBarra == std::string::npos) {
        posUltimaBarra = 0;
    } else {
        posUltimaBarra++;
    }
    
    if (posPonto == std::string::npos || posPonto < posUltimaBarra) {
        return arquivo.substr(posUltimaBarra);
    }
    
    return arquivo.substr(posUltimaBarra, posPonto - posUltimaBarra);
}

double LeitorInstancia::calcularDistanciaEuclidiana(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

Grafo LeitorInstancia::lerInstanciaORLibrary(const std::string& arquivo, int grauMaximo) {
    std::ifstream file(arquivo);
    if (!file.is_open()) {
        throw std::runtime_error("Não foi possível abrir o arquivo: " + arquivo);
    }

    // Ler todas as coordenadas
    std::vector<double> coordenadas;
    double valor;
    while (file >> valor) {
        coordenadas.push_back(valor);
    }
    file.close();

    // Verificar se temos um número par de coordenadas
    if (coordenadas.size() % 2 != 0) {
        throw std::runtime_error("Número ímpar de coordenadas no arquivo!");
    }

    int numeroVertices = coordenadas.size() / 2;
    std::cout << "Lendo " << numeroVertices << " vértices (" << coordenadas.size() << " coordenadas)..." << std::endl;

    // Extrair pares (x, y)
    std::vector<std::pair<double, double>> pontos;
    for (size_t i = 0; i < coordenadas.size(); i += 2) {
        pontos.push_back({coordenadas[i], coordenadas[i + 1]});
    }

    // Criar grafo completo
    Grafo grafo(numeroVertices);
    grafo.definirNomeInstancia(extrairNomeInstancia(arquivo));

    // Adicionar todas as arestas (grafo completo) com distâncias Euclidianas
    int numeroArestas = 0;
    for (int i = 0; i < numeroVertices; i++) {
        for (int j = i + 1; j < numeroVertices; j++) {
            double distancia = calcularDistanciaEuclidiana(
                pontos[i].first, pontos[i].second,
                pontos[j].first, pontos[j].second
            );
            grafo.adicionarAresta(i, j, distancia);
            numeroArestas++;
        }
    }

    std::cout << "Grafo completo criado com " << numeroArestas << " arestas" << std::endl;

    if (!grafo.ehConexo()) {
        throw std::runtime_error("ERRO: O grafo lido não é conexo!");
    }

    return grafo;
}
