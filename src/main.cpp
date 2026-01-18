/**
 * DC-MST - Degree-Constrained Minimum Spanning Tree
 * Trabalho Final - Teoria dos Grafos
 */

#include "Grafo.h"
#include "LeitorInstancia.h"
#include "UtilRandomico.h"
#include "Logger.h"
#include <iostream>
#include <string>

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
        std::cout << "  ler <arquivo>      - Lê uma instância de arquivo" << std::endl;
        std::cout << "\nExemplos:" << std::endl;
        std::cout << "  " << argv[0] << " teste" << std::endl;
        std::cout << "  " << argv[0] << " ler instances/exemplo.txt" << std::endl;
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
            
        } catch (const std::exception& e) {
            std::cerr << "✗ ERRO: " << e.what() << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Comando desconhecido: " << comando << std::endl;
        return 1;
    }
    
    return 0;
}
