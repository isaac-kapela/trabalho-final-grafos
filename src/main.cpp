/**
 * DC-MST - Degree-Constrained Minimum Spanning Tree
 * Trabalho Final - Teoria dos Grafos
 */

#include "Grafo.h"
#include "LeitorInstancia.h"
#include "UtilRandomico.h"
#include "Logger.h"
#include "AlgGRandReativo.h"
#include <iostream>
#include <string>
#include <chrono>

void testarInfraestrutura() {
    try {
        std::cout << "\n=== TESTE DE INFRAESTRUTURA ===" << std::endl;
        
        // Teste 1: Randomização
        std::cout << "\n[1] Testando gerador de números aleatórios..." << std::endl;
        unsigned int semente = UtilRandomico::inicializarComTempo();
        int numeroAleatorio = UtilRandomico::inteiroAleatorio(1, 100);
        double realAleatorio = UtilRandomico::realAleatorio();
        std::cout << "✓ Número inteiro aleatório: " << numeroAleatorio << std::endl;
        std::cout << "✓ Número real aleatório: " << realAleatorio << "\n" << std::endl;
        
        // Teste 2: Criação de grafo
        std::cout << "[2] Testando criação de grafo..." << std::endl;
        Grafo grafo(5);
        grafo.adicionarAresta(0, 1, 1.5);
        grafo.adicionarAresta(1, 2, 2.0);
        grafo.adicionarAresta(2, 3, 1.0);
        grafo.adicionarAresta(3, 4, 3.0);
        grafo.adicionarAresta(4, 0, 2.5);
        grafo.definirNomeInstancia("teste");
        std::cout << "✓ Grafo criado com " << grafo.getNumeroVertices() 
                  << " vértices e " << grafo.getNumeroArestas() << " arestas" << std::endl;
        std::cout << "✓ Grafo é conexo: " << (grafo.ehConexo() ? "Sim" : "Não") << "\n" << std::endl;
        
        // Teste 3: Log CSV
        std::cout << "[3] Testando geração de log..." << std::endl;
        Logger logger("results/log_teste.csv");
        DadosExecucao dados;
        dados.timestamp = Logger::getTimestampAtual();
        dados.nomeInstancia = grafo.getNomeInstancia();
        dados.grauMaximo = 3;
        dados.algoritmo = "Teste";
        dados.semente = semente;
        dados.tempoExecucao = 0.123;
        dados.custoSolucao = 10.0;
        logger.registrar(dados);
        std::cout << "✓ Log CSV criado com sucesso!\n" << std::endl;
        
        // Teste 4: Exportação para Graph Editor
        std::cout << "[4] Testando exportação para Editor de Grafos..." << std::endl;
        grafo.exportarParaGraphEditor("results/teste_grafo.txt");
        std::cout << "✓ Grafo exportado para results/teste_grafo.txt" << std::endl;
        std::cout << "  Você pode copiar o conteúdo e colar em:" << std::endl;
        std::cout << "  http://csacademy.com/app/graph_editor/\n" << std::endl;
        
        std::cout << "=== TODOS OS TESTES PASSARAM! ===" << std::endl;
        std::cout << "\nArquivos gerados em results/:" << std::endl;
        std::cout << "  - log_teste.csv (log de execução)" << std::endl;
        std::cout << "  - teste_grafo.txt (grafo completo)" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "\n✗ ERRO: " << e.what() << std::endl;
        return;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Uso: " << argv[0] << " <comando> [parâmetros]" << std::endl;
        std::cout << "\nComandos disponíveis:" << std::endl;
        std::cout << "  teste              - Testa a infraestrutura básica" << std::endl;
        std::cout << "  ler <arquivo>      - Lê uma instância de arquivo (formato padrão)" << std::endl;
        std::cout << "  orlib <arquivo> <grau_max> - Lê instância OR-Library (coordenadas)" << std::endl;
        std::cout << "  resolver <arquivo> <grau_max> <n iteracoes> <n blocos> - Algoritmo Guloso Randomizado Reativo" << std::endl;
        std::cout << "\nExemplos:" << std::endl;
        std::cout << "  " << argv[0] << " teste" << std::endl;
        std::cout << "  " << argv[0] << " ler instances/exemplo.txt" << std::endl;
        std::cout << "  " << argv[0] << " orlib dcmst/Data/crd101 3" << std::endl;
        std::cout << "  " << argv[0] << " resolver dcmst/Data/crd101 3 100 20" << std::endl;
        return 1;
    }
    
    std::string comando = argv[1];
    
    if (comando == "teste") {
        testarInfraestrutura();
    } else if (comando == "ler" && argc >= 3) {
        try {
            std::string arquivo = argv[2];
            int grauMaximo;
            
            std::cout << "\n=== LEITURA DE INSTÂNCIA ===" << std::endl;
            std::cout << "Arquivo: " << arquivo << std::endl;
            
            Grafo grafo = LeitorInstancia::lerInstancia(arquivo, grauMaximo);
            
            std::cout << "✓ Instância carregada com sucesso!" << std::endl;
            std::cout << "  Nome: " << grafo.getNomeInstancia() << std::endl;
            std::cout << "  Vértices: " << grafo.getNumeroVertices() << std::endl;
            std::cout << "  Arestas: " << grafo.getNumeroArestas() << std::endl;
            std::cout << "  Grau máximo: " << grauMaximo << std::endl;
            std::cout << "  Conexo: " << (grafo.ehConexo() ? "Sim" : "Não") << std::endl;
            
            std::string arquivoSaida = "results/" + grafo.getNomeInstancia() + "_grafo.txt";
            grafo.exportarParaGraphEditor(arquivoSaida);
            std::cout << "\n✓ Grafo exportado para: " << arquivoSaida << std::endl;
            
            Logger logger("results/log_leitura.csv");
            DadosLeitura dados;
            dados.timestamp = Logger::getTimestampAtual();
            dados.nomeInstancia = grafo.getNomeInstancia();
            dados.numVertices = grafo.getNumeroVertices();
            dados.numArestas = grafo.getNumeroArestas();
            dados.grauMaximo = grauMaximo;
            dados.formato = "Padrão";
            dados.conexo = grafo.ehConexo();
            logger.registrarLeitura(dados);
            std::cout << "✓ Log registrado em: results/log_leitura.csv" << std::endl;
            
        } catch (const std::exception& e) {
            std::cerr << "✗ ERRO: " << e.what() << std::endl;
            return 1;
        }
    } else if (comando == "orlib" && argc >= 4) {
        try {
            std::string arquivo = argv[2];
            int grauMaximo = std::stoi(argv[3]);
            
            std::cout << "\n=== LEITURA DE INSTÂNCIA OR-LIBRARY ===" << std::endl;
            std::cout << "Arquivo: " << arquivo << std::endl;
            std::cout << "Grau máximo: " << grauMaximo << std::endl;
            
            Grafo grafo = LeitorInstancia::lerInstanciaORLibrary(arquivo, grauMaximo);
            
            std::cout << "✓ Instância carregada com sucesso!" << std::endl;
            std::cout << "  Nome: " << grafo.getNomeInstancia() << std::endl;
            std::cout << "  Vértices: " << grafo.getNumeroVertices() << std::endl;
            std::cout << "  Arestas: " << grafo.getNumeroArestas() << std::endl;
            std::cout << "  Conexo: " << (grafo.ehConexo() ? "Sim" : "Não") << std::endl;
            
            std::string arquivoSaida = "results/" + grafo.getNomeInstancia() + "_grafo.txt";
            grafo.exportarParaGraphEditor(arquivoSaida);
            std::cout << "\n✓ Grafo exportado para: " << arquivoSaida << std::endl;
            
            Logger logger("results/log_leitura.csv");
            DadosLeitura dados;
            dados.timestamp = Logger::getTimestampAtual();
            dados.nomeInstancia = grafo.getNomeInstancia();
            dados.numVertices = grafo.getNumeroVertices();
            dados.numArestas = grafo.getNumeroArestas();
            dados.grauMaximo = grauMaximo;
            dados.formato = "OR-Library";
            dados.conexo = grafo.ehConexo();
            logger.registrarLeitura(dados);
            std::cout << "✓ Log registrado em: results/log_leitura.csv" << std::endl;
            
        } catch (const std::exception& e) {
            std::cerr << "✗ ERRO: " << e.what() << std::endl;
            return 1;
        }
    } else if (comando == "resolver" && argc >= 4) {
        try {
            std::string arquivo = argv[2];
            
            int grauMaximoOverride = std::stoi(argv[3]);
            int maxIter = (argc >= 5) ? std::stoi(argv[4]) : 1000; // Padrão 1000
            int bloco   = (argc >= 6) ? std::stoi(argv[5]) : 50;   // Padrão 50

            std::cout << "\n=== RESOLVENDO DC-MST ===" << std::endl;
            std::cout << "Arquivo: " << arquivo << std::endl;
            std::cout << "Grau Máximo: " << grauMaximoOverride << std::endl;
            std::cout << "Iterações: " << maxIter << std::endl;
            std::cout << "Tamanho do Bloco: " << bloco << std::endl;

            int grauLido = 0;
            Grafo grafo(0);
            if (arquivo.find("Data") != std::string::npos) 
                 grafo = LeitorInstancia::lerInstanciaORLibrary(arquivo, grauMaximoOverride);
            else 
                 grafo = LeitorInstancia::lerInstancia(arquivo, grauLido);

            int d = grauMaximoOverride;

            std::vector<double> listaAlfas = {0.05, 0.10, 0.15, 0.20, 0.25, 0.30, 0.40, 0.50};
            
            GeradoraMinimaGRR solver(grafo, d, listaAlfas);
            
            auto inicio = std::chrono::high_resolution_clock::now();
            
            auto resultado = solver.resolver(maxIter, bloco);
            
            auto fim = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duracao = fim - inicio;

            double custoMelhor = resultado.first;
            std::set<int> arestasMelhor = resultado.second;

            if (custoMelhor == std::numeric_limits<double>::max()) {
                std::cout << "\nNão foi possível encontrar solução viável." << std::endl;
            } else {
                std::cout << "\n=== SOLUÇÃO FINAL ===" << std::endl;
                std::cout << "Custo: " << custoMelhor << std::endl;
                std::cout << "Tempo: " << duracao.count() << "s" << std::endl;
                std::cout << "Validando solução... ";
                if (grafo.ehArvoreGeradoraValida(arestasMelhor, d)) {
                    std::cout << "OK!" << std::endl;
                } else {
                    std::cout << "INVÁLIDA!" << std::endl;
                }

                std::string arqSaida = "results/solucao_" + grafo.getNomeInstancia() + ".txt";
                grafo.exportarSolucaoParaGraphEditor(arestasMelhor, arqSaida);
                std::cout << "Solução exportada para: " << arqSaida << std::endl;

                Logger logger("results/log_execucao.csv");
                DadosExecucao dados;
                dados.timestamp = Logger::getTimestampAtual();
                dados.nomeInstancia = grafo.getNomeInstancia();
                dados.grauMaximo = d;
                dados.algoritmo = "ReactiveGRASP";
                dados.iteracoes = maxIter;
                dados.tamanhoBloco = bloco;
                dados.semente = UtilRandomico::obterSementeAtual();
                dados.tempoExecucao = duracao.count();
                dados.custoSolucao = custoMelhor;
                dados.melhorAlpha = solver.getMelhorAlpha();
                logger.registrar(dados);
            }

        } catch (const std::exception& e) {
            std::cerr << "ERRO: " << e.what() << std::endl;
            return 1;
        }
    }else {
        std::cerr << "Comando desconhecido: " << comando << std::endl;
        return 1;
    }
    
    return 0;
}
