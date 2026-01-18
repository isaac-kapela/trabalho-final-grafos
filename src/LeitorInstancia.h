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

private:
    static std::string extrairNomeInstancia(const std::string& arquivo);
};

#endif // LEITOR_INSTANCIA_H
