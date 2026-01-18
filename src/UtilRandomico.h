#ifndef UTILRANDOMICO_H
#define UTILRANDOMICO_H

#include <random>
#include <chrono>

/**
 * Classe utilitária para geração de números aleatórios
 */
class UtilRandomico {
private:
    static std::mt19937 gerador;
    static unsigned int sementeAtual;
    static bool inicializado;

public:
    static unsigned int inicializarComTempo();
    static void inicializarComSemente(unsigned int semente);
    static unsigned int obterSementeAtual();
    static int inteiroAleatorio(int min, int max);
    static double realAleatorio();
    static double realAleatorio(double min, double max);
    static std::mt19937& obterGerador();
};

#endif // UTILRANDOMICO_H
