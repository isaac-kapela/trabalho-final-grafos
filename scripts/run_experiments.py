import subprocess
import os
import time

# Configurações
BIN_PATH = "./bin/dcmst"
INSTANCES_DIR = "dcmst/Data"
NUM_EXECUTIONS = 10
ALGORITHM_PARAMS = {
    "iteracoes": "300",
    "blocos": "50"
}

# Lista sugerida baseada no README.md do projeto
# Formato: ("nome_arquivo", grau_maximo)
instances = [
    # Grupo CRD (Grau sugerido: varia, mas 3 a 5 é comum. Verifique se o grupo definiu)
    ("crd101", 3), ("crd103", 5), ("crd500", 5), ("crd700", 5),
    
    # Grupo RAND
    ("rand200", 4), 
    
    # Grupo STR
    ("str305", 5), ("str505", 5), ("str701", 5),
    
    # Grupo SYM
    ("sym306", 5), ("sym708", 5),
    
    # Grupo SHRD
    ("shrd259", 5)
]

def run_tests():
    if not os.path.exists("results"):
        os.makedirs("results")

    print(f"Iniciando bateria de testes: {NUM_EXECUTIONS} execuções por instância.")
    
    for nome, grau in instances:
        iteracoes = "300"

        if "str505" in nome or "sym708" in nome or "str701" in nome:
            print(f"  [Aviso] Instância grande detectada: Reduzindo iterações para 30.")
            iteracoes = "30"
        
        instance_path = os.path.join(INSTANCES_DIR, nome)
        
        print(f"\n--- Processando {nome} (Grau Max: {grau}) ---")
        
        for i in range(1, NUM_EXECUTIONS + 1):
            print(f"  Execução {i}/{NUM_EXECUTIONS}...", end="", flush=True)
            
            # Comando: ./bin/dcmst resolver <arquivo> <grau> <iter> <blocos>
            cmd = [
                BIN_PATH, "resolver", instance_path, 
                str(grau), iteracoes, ALGORITHM_PARAMS["blocos"]
            ]
            
        try:
            # Removemos o check=True para não travar o script inteiro
            result = subprocess.run(cmd, check=False, stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
            
            if result.returncode == 0:
                print(" OK")
            else:
                # Imprime que falhou mas CONTINUA para a próxima
                print(f" FALHOU (Código {result.returncode})")
                # Opcional: imprimir o erro real para você ver
                print(f"Erro: {result.stderr.decode('utf-8')}")

        except Exception as e:
            print(f" Erro de execução: {e}")

if __name__ == "__main__":
    run_tests()