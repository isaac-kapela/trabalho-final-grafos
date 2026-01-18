#ifndef LEITOR_INSTANCIA_H
#define LEITOR_INSTANCIA_H

#include "Grafo.h"
#include <string>

/**
 * Classe para leitura de instâncias do problema DC-MST
 */
class LeitorInstancia {
public:
    static Grafo lerInstancia(const std::string& arquivo, int& grauMaximo);
    static Grafo lerInstanciaORLibrary(const std::string& arquivo, int grauMaximo);

private:
    static std::string extrairNomeInstancia(const std::string& arquivo);
    static double calcularDistanciaEuclidiana(double x1, double y1, double x2, double y2);
};

#endif // LEITOR_INSTANCIA_H
