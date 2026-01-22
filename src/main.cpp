/**
 * DC-MST - Degree-Constrained Minimum Spanning Tree
 * Trabalho Final - Teoria dos Grafos
 */

#include "Grafo.h"
#include "LeitorInstancia.h"
#include "UtilRandomico.h"
#include "Logger.h"
#include "AlgGRandReativo.h"
#include "AlgGuloso.h" // Incluindo sua classe
#include <iostream>
#include <string>
#include <chrono>
#include <limits> // Necessário para numeric_limits

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
        std::cout << "  orlib <diretorio> <nome_instancia> <grau_max> - Lê instância OR-Library" << std::endl;
        std::cout << "  guloso <arquivo> <grau_max>              - Algoritmo Guloso (Kruskal com grau)" << std::endl;
        std::cout << "  randomizado <arquivo> <grau_max> <alpha> [iteracoes] - Algoritmo Guloso Randomizado" << std::endl;
        std::cout << "  reativo <arquivo> <grau_max> <n iteracoes> <n blocos> - Algoritmo Guloso Randomizado Reativo" << std::endl;
        std::cout << "\nExemplos:" << std::endl;
        std::cout << "  " << argv[0] << " teste" << std::endl;
        std::cout << "  " << argv[0] << " ler instances/exemplo.txt" << std::endl;
        std::cout << "  " << argv[0] << " orlib dcmst/Data crd101 3" << std::endl;
        std::cout << "  " << argv[0] << " guloso instances/exemplo.txt 3" << std::endl;
        std::cout << "  " << argv[0] << " randomizado instances/exemplo.txt 3 0.2 30" << std::endl;
        std::cout << "  " << argv[0] << " reativo dcmst/Data/crd101 3 100 20" << std::endl;
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
    } else if (comando == "orlib" && argc >= 5) {
        try {
            std::string diretorio = argv[2];
            std::string nomeArquivo = argv[3];
            std::string arquivo = diretorio + "/" + nomeArquivo;
            int grauMaximo = std::stoi(argv[4]);
            
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

    // COMANDO: GULOSO 
    } else if (comando == "guloso" && argc >= 4) {
        try {
            std::string arquivo = argv[2];
            int grauMaximoOverride = std::stoi(argv[3]);

            std::cout << "\n=== EXECUÇÃO GULOSO DETERMINÍSTICO ===" << std::endl;
            std::cout << "Arquivo: " << arquivo << std::endl;
            std::cout << "Grau Máximo: " << grauMaximoOverride << std::endl;

            // Lógica de leitura 
            int grauLido = 0;
            Grafo grafo(0);
            if (arquivo.find("Data") != std::string::npos) 
                 grafo = LeitorInstancia::lerInstanciaORLibrary(arquivo, grauMaximoOverride);
            else 
                 grafo = LeitorInstancia::lerInstancia(arquivo, grauLido);

            // Execução
            auto inicio = std::chrono::high_resolution_clock::now();
            auto resultado = AlgGuloso::executarGuloso(grafo, grauMaximoOverride);
            auto fim = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duracao = fim - inicio;

            double custo = resultado.first;
            std::set<int> arestas = resultado.second;

            if (custo == std::numeric_limits<double>::max()) {
                std::cout << "\nSOLUÇÃO INVIÁVEL (Não foi possível respeitar o grau ou desconexo)." << std::endl;
            } else {
                std::cout << "Custo: " << custo << std::endl;
                std::cout << "Tempo: " << duracao.count() << "s" << std::endl;
                
                // Exportar solução visual
                std::string arqSaida = "results/solucao_guloso_" + grafo.getNomeInstancia() + ".txt";
                grafo.exportarSolucaoParaGraphEditor(arestas, arqSaida);
                std::cout << "Visualização salva em: " << arqSaida << std::endl;

                // Log CSV
                Logger logger("results/log_execucao.csv");
                DadosExecucao dados;
                dados.timestamp = Logger::getTimestampAtual();
                dados.nomeInstancia = grafo.getNomeInstancia();
                dados.grauMaximo = grauMaximoOverride;
                dados.algoritmo = "GulosoConstructive"; 
                dados.semente = 0; // Determinístico
                dados.tempoExecucao = duracao.count();
                dados.custoSolucao = custo;
                logger.registrar(dados);
            }

        } catch (const std::exception& e) {
            std::cerr << "ERRO: " << e.what() << std::endl;
            return 1;
        }

    // COMANDO: RANDOMIZADO 
    } else if (comando == "randomizado" && argc >= 5) {
        try {
            std::string arquivo = argv[2];
            int grauMaximoOverride = std::stoi(argv[3]);
            double alpha = std::stod(argv[4]);
            int iteracoes = (argc >= 6) ? std::stoi(argv[5]) : 30; // Padrão 30

            std::cout << "\n=== EXECUÇÃO GULOSO RANDOMIZADO ===" << std::endl;
            std::cout << "Arquivo: " << arquivo << std::endl;
            std::cout << "Grau Máximo: " << grauMaximoOverride << std::endl;
            std::cout << "Alpha: " << alpha << std::endl;
            std::cout << "Iterações: " << iteracoes << std::endl;

            // Leitura
            int grauLido = 0;
            Grafo grafo(0);
            if (arquivo.find("Data") != std::string::npos) 
                 grafo = LeitorInstancia::lerInstanciaORLibrary(arquivo, grauMaximoOverride);
            else 
                 grafo = LeitorInstancia::lerInstancia(arquivo, grauLido);

            // Execução
            auto inicio = std::chrono::high_resolution_clock::now();
            // Chama a função wrapper que roda N vezes e pega a melhor
            auto resultado = AlgGuloso::resolverRandomizado(grafo, grauMaximoOverride, alpha, iteracoes);
            auto fim = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duracao = fim - inicio;

            double custo = resultado.first;
            std::set<int> arestas = resultado.second;

            if (custo == std::numeric_limits<double>::max()) {
                std::cout << "\nSOLUÇÃO INVIÁVEL." << std::endl;
            } else {
                std::cout << "Melhor Custo Encontrado: " << custo << std::endl;
                std::cout << "Tempo Total: " << duracao.count() << "s" << std::endl;

                // Exportar
                std::string arqSaida = "results/solucao_rand_" + grafo.getNomeInstancia() + ".txt";
                grafo.exportarSolucaoParaGraphEditor(arestas, arqSaida);
                std::cout << "Visualização salva em: " << arqSaida << std::endl;

                // Log CSV
                Logger logger("results/log_execucao.csv");
                DadosExecucao dados;
                dados.timestamp = Logger::getTimestampAtual();
                dados.nomeInstancia = grafo.getNomeInstancia();
                dados.grauMaximo = grauMaximoOverride;
                dados.algoritmo = "RandomizedGreedy";
                dados.alpha = alpha;
                dados.iteracoes = iteracoes;
                dados.semente = UtilRandomico::obterSementeAtual();
                dados.tempoExecucao = duracao.count();
                dados.custoSolucao = custo;
                logger.registrar(dados);
            }

        } catch (const std::exception& e) {
            std::cerr << "ERRO: " << e.what() << std::endl;
            return 1;
        }

    // COMANDO: REATIVO
    } else if (comando == "reativo" && argc >= 4) {
        try {
            std::string arquivo = argv[2];

            int grauMaximoOverride = std::stoi(argv[3]);
            int maxIter = (argc >= 5) ? std::stoi(argv[4]) : 1000; // Padrão 1000
            int bloco   = (argc >= 6) ? std::stoi(argv[5]) : 50;   // Padrão 50

            std::cout << "\n=== RESOLVENDO DC-MST (Reativo) ===" << std::endl;
            std::cout << "Arquivo: " << arquivo << std::endl;
            std::cout << "Grau: " << grauMaximoOverride << " | Iter: " << maxIter << " | Bloco: " << bloco << std::endl;

            int d_dummy = 0; 
            Grafo grafo = LeitorInstancia::lerInstancia(arquivo, d_dummy);

            std::vector<double> listaAlfas = {0.00, 0.05, 0.10};
            
            GeradoraMinimaGRR solver(grafo, grauMaximoOverride, listaAlfas);
            
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
                
                std::string arqSaida = "results/solucao_" + grafo.getNomeInstancia() + ".txt";
                grafo.exportarSolucaoParaGraphEditor(arestasMelhor, arqSaida);

                Logger logger("results/log_execucao.csv");
                DadosExecucao dados;
                dados.timestamp = Logger::getTimestampAtual();
                dados.nomeInstancia = grafo.getNomeInstancia();
                dados.grauMaximo = grauMaximoOverride;
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
    } else {
        std::cerr << "Comando desconhecido: " << comando << std::endl;
        return 1;
    }
    
    return 0;
}
