# Compilador e flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g

# Diretórios
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin
RESULTS_DIR = results
INSTANCES_DIR = instances

# Arquivos fonte
SOURCES = $(SRC_DIR)/main.cpp \
          $(SRC_DIR)/Grafo.cpp \
          $(SRC_DIR)/LeitorInstancia.cpp \
          $(SRC_DIR)/UtilRandomico.cpp \
          $(SRC_DIR)/Logger.cpp \
          $(SRC_DIR)/AlgGuloso.cpp \
		  $(SRC_DIR)/AlgGRandReativo.cpp

# Arquivos objeto
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

# Executável
TARGET = $(BIN_DIR)/dcmst

# Regra padrão
all: directories $(TARGET)

# Cria diretórios necessários
directories:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR) $(RESULTS_DIR) $(INSTANCES_DIR)

# Compila o executável
$(TARGET): $(OBJECTS)
	@echo "Linkando $(TARGET)..."
	@$(CXX) $(OBJECTS) -o $(TARGET)
	@echo "Compilação concluída com sucesso!"

# Compila arquivos .cpp em .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compilando $<..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpeza
clean:
	@echo "Limpando arquivos compilados..."
	@rm -rf $(BUILD_DIR)/*.o $(TARGET)
	@echo "Limpeza concluída!"

# Remove também resultados
cleanall: clean
	@rm -rf $(RESULTS_DIR)/*.csv $(RESULTS_DIR)/*.txt

# Executa teste de infraestrutura
teste: $(TARGET)
	@./$(TARGET) teste

# Ajuda
help:
	@echo "Makefile do projeto DC-MST"
	@echo ""
	@echo "Alvos disponíveis:"
	@echo "  make          - Compila o projeto"
	@echo "  make clean    - Remove arquivos compilados"
	@echo "  make cleanall - Remove arquivos compilados e resultados"
	@echo "  make teste    - Compila e executa teste de infraestrutura"
	@echo "  make help     - Mostra esta mensagem"

.PHONY: all directories clean cleanall teste help
