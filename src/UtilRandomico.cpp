#include "UtilRandomico.h"
#include <iostream>

std::mt19937 UtilRandomico::gerador;
unsigned int UtilRandomico::sementeAtual = 0;
bool UtilRandomico::inicializado = false;

unsigned int UtilRandomico::inicializarComTempo() {
    auto agora = std::chrono::high_resolution_clock::now();
    auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(
        agora.time_since_epoch()
    ).count();
    
    sementeAtual = static_cast<unsigned int>(timestamp);
    gerador.seed(sementeAtual);
    inicializado = true;
    
    std::cout << "=== Semente de Randomização ===" << std::endl;
    std::cout << "Semente: " << sementeAtual << std::endl;
    std::cout << "===============================" << std::endl;
    
    return sementeAtual;
}

void UtilRandomico::inicializarComSemente(unsigned int semente) {
    sementeAtual = semente;
    gerador.seed(sementeAtual);
    inicializado = true;
    
    std::cout << "=== Semente de Randomização ===" << std::endl;
    std::cout << "Semente: " << sementeAtual << " (manual)" << std::endl;
    std::cout << "===============================" << std::endl;
}

unsigned int UtilRandomico::obterSementeAtual() {
    if (!inicializado) {
        inicializarComTempo();
    }
    return sementeAtual;
}

int UtilRandomico::inteiroAleatorio(int min, int max) {
    if (!inicializado) {
        inicializarComTempo();
    }
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gerador);
}

double UtilRandomico::realAleatorio() {
    if (!inicializado) {
        inicializarComTempo();
    }
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    return dist(gerador);
}

double UtilRandomico::realAleatorio(double min, double max) {
    if (!inicializado) {
        inicializarComTempo();
    }
    std::uniform_real_distribution<double> dist(min, max);
    return dist(gerador);
}

std::mt19937& UtilRandomico::obterGerador() {
    if (!inicializado) {
        inicializarComTempo();
    }
    return gerador;
}
