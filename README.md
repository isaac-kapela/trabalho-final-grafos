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

**Grafo (ADT)** - Classe completa para manipulação de grafos
- Estrutura de dados eficiente (lista de adjacência)
- Métodos para adicionar arestas, verificar conectividade
- Cálculo de custo e validação de soluções
- Exportação para visualização

**LeitorInstancia** - Parser de arquivos de instância
- Lê formato padrão OR-Library DC-MST
- Valida conectividade do grafo
- Extrai nome da instância automaticamente

**UtilRandomico** - Controle de randomização
- Geração de semente baseada em timestamp
- Suporte para semente manual
- Números aleatórios inteiros e reais

**Logger** - Sistema de logging CSV
- Registro automático de experimentos
- Formato compatível com análise de dados
- Cabeçalho padronizado

## Algoritmos Implementados

**AlgGuloso** - Algoritmo Guloso Construtivo
- Baseado em Kruskal adaptado para restrição de grau
- Usa Union-Find para detecção de ciclos
- Ordena arestas por peso crescente
- Adiciona arestas respeitando grau máximo

**AlgGRandReativo** - GRASP Reativo
- Construção randomizada com parâmetro α adaptativo
- Fase de busca local para refinamento
- Atualização de probabilidades baseada em qualidade
- Múltiplas iterações para convergência

## Compilação

```bash
# Compilar o projeto
make

# Limpar arquivos compilados
make clean

# Limpar tudo (incluindo resultados)
make cleanall
```

## Como Usar

### 1. Testar a Infraestrutura

```bash
./bin/dcmst teste
```

Testa todos os componentes básicos: grafo, randomização, logger e exportação.

### 2. Instâncias OR-Library (Recomendado)

As instâncias oficiais já estão disponíveis em `dcmst/Data/`:

```bash
# Formato: ./bin/dcmst orlib <arquivo> <grau_maximo>
./bin/dcmst orlib dcmst/Data/crd101 3
./bin/dcmst orlib dcmst/Data/crd103 5
./bin/dcmst orlib dcmst/Data/rand200 4
./bin/dcmst orlib dcmst/Data/str305 5
```

**Instâncias disponíveis:**
- `crd101, crd103, crd105, crd108, crd109, crd500, crd700, crd705`
- `rand200`
- `str305, str505, str509, str701, str702, str1001, str1005`
- `sym306, sym708`
- `shrd259`
- `genhard`

Formato OR-Library: usa coordenadas (x,y) e calcula distâncias euclidianas automaticamente para criar grafo completo.

### 3. Instâncias Customizadas (Opcional)

Para usar suas próprias instâncias:

```bash
# Formato: ./bin/dcmst ler <arquivo>
./bin/dcmst ler instances/exemplo.txt
```

**Formato do arquivo:**
```
<num_vertices> <num_arestas> <grau_maximo>
<origem> <destino> <peso>
<origem> <destino> <peso>
...
```

**Importante:** Os vértices devem ser numerados a partir de 1.

**Exemplo (instances/exemplo.txt):**
```
5 7 3
1 2 1.5
2 3 2.0
3 4 1.0
4 5 3.0
5 1 2.5
1 3 4.0
2 5 1.8
```

## Visualização de Grafos

Os arquivos `.txt` gerados em `results/` podem ser visualizados no CS Academy Graph Editor:

🔗 https://csacademy.com/app/graph_editor/

Basta copiar o conteúdo do arquivo e colar no editor.

## Arquivos Gerados

Após executar o programa, os seguintes arquivos são criados em `results/`:

- `<nome_instancia>_grafo.txt` - Grafo completo para visualização
- `log_teste.csv` - Log das execuções (quando implementados os algoritmos)

## Como Executar Os Experimentos

Este projeto conta com um pipeline de execução automatizado. Não é necessário rodar o executável C++ manualmente para cada instância.

### 1. Executar a Bateria de Testes
O script Python gerencia a execução, aplica timeout para instâncias grandes e salva os resultados em results/log_execucao.csv.

```bash
# Executa 10 vezes cada instância configurada
python3 scripts/run_experiments.py
```
### 2. Gerar Tabelas para o Relatório
Após a execução, utilize o gerador de tabelas para criar o arquivo LaTeX com os comparativos de Gap e Tempo.

```bash
python3 scripts/generate_tables.py > results/tabelas_finais.tex
```

O arquivo gerado pode ser importado diretamente no Overleaf com \input{tabelas_finais}.

## Referências

Instâncias oficiais da OR-Library:
https://andreas-ernst.github.io/Mathprog-ORlib/info/readmeDCMST.html