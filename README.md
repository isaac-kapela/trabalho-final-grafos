# Projeto DC-MST - Degree-Constrained Minimum Spanning Tree

Trabalho Final - Teoria dos Grafos
UFJF - DCC059

## Estrutura do Projeto

```
trabalho-final-grafos/
├── src/              # Código fonte
├── bin/              # Executável compilado
├── build/            # Arquivos objeto (.o)
├── instances/        # Instâncias de teste
├── results/          # Logs e saídas
└── Makefile          # Sistema de compilação
```

## Infraestrutura Implementada

✅ **Grafo (ADT)** - Classe completa para manipulação de grafos
- Estrutura de dados eficiente (lista de adjacência)
- Métodos para adicionar arestas, verificar conectividade
- Cálculo de custo e validação de soluções
- Exportação para visualização

✅ **LeitorInstancia** - Parser de arquivos de instância
- Lê formato padrão OR-Library DC-MST
- Valida conectividade do grafo
- Extrai nome da instância automaticamente

✅ **UtilRandomico** - Controle de randomização
- Geração de semente baseada em timestamp
- Suporte para semente manual
- Números aleatórios inteiros e reais

✅ **Registrador** - Sistema de logging CSV
- Registro automático de experimentos
- Formato compatível com análise de dados
- Cabeçalho padronizado

## Compilação

```bash
# Compilar o projeto
make

# Limpar arquivos compilados
make clean

# Limpar tudo (incluindo resultados)
make cleanall
```

## Execução

```bash
# Testar infraestrutura
./bin/dcmst teste

# Ler uma instância de arquivo
./bin/dcmst ler instances/exemplo.txt
```

## Formato das Instâncias

```
<num_vertices> <num_arestas> <grau_maximo>
<origem> <destino> <peso>
<origem> <destino> <peso>
...
```

Os vértices devem ser numerados a partir de 1.

## Próximos Passos

- [ ] Implementar algoritmo guloso
- [ ] Implementar algoritmo guloso randomizado
- [ ] Implementar algoritmo guloso randomizado reativo
- [ ] Baixar instâncias oficiais da OR-Library
- [ ] Executar experimentos
- [ ] Gerar relatório

## Instâncias Oficiais

As instâncias de referência podem ser obtidas em:
https://andreas-ernst.github.io/Mathprog-ORlib/info/readmeDCMST.html
