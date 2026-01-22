import subprocess
import os
import time
from concurrent.futures import ThreadPoolExecutor

# Configurações
BIN_PATH = "./bin/dcmst"
INSTANCES_DIR = "dcmst/Data" 
LOG_FILE = "results/log_execucao.csv"

NUM_EXECUTIONS = 10
NUM_EXEC_DETERMINISTIC = 1

PARAMS_RANDOMIZADO = { "alpha": "0.10", "iteracoes": "50" }
PARAMS_REATIVO     = { "iteracoes": "350", "blocos": "40" }

# Lista sugerida baseada no README.md do projeto
# Formato: ("nome_arquivo", grau_maximo)
instances = [
    # Grupo CRD (Grau sugerido: varia, mas 3 a 5 é comum. Verifique se o grupo definiu)
    ("crd101", 3), ("crd103", 5), ("crd500", 5), ("crd700", 5),
    
    # Grupo RAND
    ("rand200", 4), 
    
    # Grupo STR
    ("str305", 5), ("str505", 5), ("str701", 5)
]

algorithms_to_run = ["guloso", "randomizado", "reativo"]

def clean_log_file():
    if not os.path.exists("results"):
        os.makedirs("results")
    
    header = "timestamp,instance,max_degree,algorithm,alpha,iterations,block_size,seed,execution_time,solution_cost,average_cost,best_alpha\n"
    with open(LOG_FILE, "w") as f:
        f.write(header)
    print(f"Arquivo de log limpo: {LOG_FILE}")

def run_single_experiment(params):
    i, algo, nome, grau = params
    instance_path = os.path.join(INSTANCES_DIR, nome)
    
    if not os.path.exists(instance_path):
        if os.path.exists(instance_path + ".txt"): instance_path += ".txt"
        else: return

    cmd = []
    if algo == "guloso":
        cmd = [BIN_PATH, "guloso", instance_path, str(grau)]
    elif algo == "randomizado":
        cmd = [BIN_PATH, "randomizado", instance_path, str(grau), 
               PARAMS_RANDOMIZADO["alpha"], PARAMS_RANDOMIZADO["iteracoes"]]
    elif algo == "reativo":
        cmd = [BIN_PATH, "reativo", instance_path, str(grau), 
               PARAMS_REATIVO["iteracoes"], PARAMS_REATIVO["blocos"]]

    try:
        result = subprocess.run(cmd, check=False, stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
        if result.returncode != 0:
            print(f"  [FALHA] {algo.upper()} - {nome}: {result.stderr.decode('utf-8').strip()}")
        else:
            print(".", end="", flush=True)
    except Exception as e:
        print(f"  [ERRO] {algo} {nome}: {e}")

def run_tests():
    clean_log_file()
    print(f"Iniciando bateria de testes:")
    
    tarefas = []

    for nome, grau in instances:
        for algo in algorithms_to_run:
        
            execucoes = NUM_EXEC_DETERMINISTIC if algo == "guloso" else NUM_EXECUTIONS
            
            for i in range(1, execucoes + 1):
                tarefas.append((i, algo, nome, grau))

    workers = os.cpu_count() or 2

    print(f"Total de tarefas: {len(tarefas)}")

    start_time = time.time()
    with ThreadPoolExecutor(max_workers=workers) as executor:
        list(executor.map(run_single_experiment, tarefas))
        
    total_time = time.time() - start_time
    print(f"\n\n--- FIM DOS EXPERIMENTOS ---")
    print(f"Tempo total: {total_time:.2f} segundos")

if __name__ == "__main__":
    run_tests()