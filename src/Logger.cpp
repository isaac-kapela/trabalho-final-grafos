#include "Logger.h"
#include <iomanip>
#include <sstream>
#include <iostream>
#include <sys/stat.h>

Logger::Logger(const std::string& nomeArquivo) 
    : nomeArquivo(nomeArquivo), cabecalhoEscrito(false) {
    cabecalhoEscrito = arquivoPossuiCabecalho();
}

bool Logger::arquivoPossuiCabecalho() {
    struct stat buffer;
    if (stat(nomeArquivo.c_str(), &buffer) != 0) {
        return false;
    }
    
    std::ifstream file(nomeArquivo);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    if (std::getline(file, line)) {
        cabecalhoEscrito = (line.find("timestamp") != std::string::npos || 
                           line.find("Timestamp") != std::string::npos);
    }
    
    file.close();
    return cabecalhoEscrito;
}

void Logger::escreverCabecalho() {
    std::ofstream file(nomeArquivo, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "ERRO: Não foi possível abrir o arquivo de log: " << nomeArquivo << std::endl;
        return;
    }
    
    file << "timestamp,instance,max_degree,algorithm,alpha,iterations,block_size,"
         << "seed,execution_time,solution_cost,average_cost,best_alpha\n";
    
    file.close();
    cabecalhoEscrito = true;
}

std::string Logger::getTimestampAtual() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void Logger::registrar(const DadosExecucao& dados) {
    if (!cabecalhoEscrito) {
        escreverCabecalho();
    }
    
    std::ofstream file(nomeArquivo, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "ERRO: Não foi possível abrir o arquivo de log: " << nomeArquivo << std::endl;
        return;
    }
    
    file << dados.timestamp << ","
         << dados.nomeInstancia << ","
         << dados.grauMaximo << ","
         << dados.algoritmo << ",";
    
    if (dados.alpha >= 0.0) {
        file << dados.alpha;
    }
    file << ",";
    
    if (dados.iteracoes > 0) {
        file << dados.iteracoes;
    }
    file << ",";
    
    if (dados.tamanhoBloco > 0) {
        file << dados.tamanhoBloco;
    }
    file << ",";
    
    file << dados.semente << ","
         << std::fixed << std::setprecision(6) << dados.tempoExecucao << ","
         << std::fixed << std::setprecision(2) << dados.custoSolucao << ",";
    
    if (dados.custoMedio >= 0.0) {
        file << std::fixed << std::setprecision(2) << dados.custoMedio;
    }
    file << ",";
    
    if (dados.melhorAlpha >= 0.0) {
        file << dados.melhorAlpha;
    }
    file << "\n";
    
    file.close();
    std::cout << "Log registrado em " << nomeArquivo << std::endl;
}
