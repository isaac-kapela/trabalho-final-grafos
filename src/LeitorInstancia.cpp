#include "LeitorInstancia.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

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
