#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <chrono>

/**
 * Estrutura para armazenar dados de uma execução
 */
struct DadosExecucao {
    std::string timestamp;
    std::string nomeInstancia;
    int grauMaximo;
    std::string algoritmo;
    double alpha;
    int iteracoes;
    int tamanhoBloco;
    unsigned int semente;
    double tempoExecucao;
    double custoSolucao;
    double custoMedio;
    double melhorAlpha;

    DadosExecucao() : grauMaximo(0), alpha(-1.0), iteracoes(0), tamanhoBloco(0),
                      semente(0), tempoExecucao(0.0), custoSolucao(0.0),
                      custoMedio(-1.0), melhorAlpha(-1.0) {}
};

/**
 * Estrutura para armazenar dados de leitura de instâncias
 */
struct DadosLeitura {
    std::string timestamp;
    std::string nomeInstancia;
    int numVertices;
    int numArestas;
    int grauMaximo;
    std::string formato;
    bool conexo;

    DadosLeitura() : numVertices(0), numArestas(0), grauMaximo(0), conexo(false) {}
};

/**
 * Classe para gerenciar logs de execução em formato CSV
 */
class Logger {
private:
    std::string nomeArquivo;
    bool cabecalhoEscrito;

public:
    Logger(const std::string& nomeArquivo);
    void registrar(const DadosExecucao& dados);
    void registrarLeitura(const DadosLeitura& dados);
    static std::string getTimestampAtual();

private:
    void escreverCabecalho();
    bool arquivoPossuiCabecalho();
};

#endif // LOGGER_H
