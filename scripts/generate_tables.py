import pandas as pd
import numpy as np

INPUT_CSV = "results/log_execucao.csv"

# Melhores soluções da Literatura (OR-Library e benchmarks)
BEST_KNOWN = {
    "crd101": 7004, "crd103": 7041, "crd500": 4931, "crd700": 5789,
    "rand200": 668,
    "str305": 6467, "str505": 6880, "str701": 4217
}

# Mapeamento de nomes
ALG_MAP = {
    'GulosoConstructive': 'Guloso',
    'RandomizedGreedy': 'Random',
    'ReactiveGRASP': 'Reativo'
}

def get_gap(value, best_known):
    if not best_known or pd.isna(value):
        return None
    return ((value - best_known) / best_known) * 100

def format_gap(val):
    if pd.isna(val): return "-"
    s = f"{val:.2f}"
    if val <= 0.01:
        return f"\\textbf{{{s}}}"
    return s

def generate_report():
    try:
        df = pd.read_csv(INPUT_CSV)
    except:
        print(f"Erro: Não foi possível ler {INPUT_CSV}")
        return

    df.columns = df.columns.str.strip()
    df['solution_cost'] = pd.to_numeric(df['solution_cost'], errors='coerce')
    df['execution_time'] = pd.to_numeric(df['execution_time'], errors='coerce')
    
    grouped = df.groupby(['instance', 'algorithm']).agg({
        'solution_cost': ['min', 'mean'],
        'execution_time': 'mean'
    })
    
    grouped.columns = ['_'.join(col).strip() for col in grouped.columns.values]
    grouped = grouped.reset_index()

    df_pivot = grouped.pivot(index='instance', columns='algorithm')
    algorithms = ['GulosoConstructive', 'RandomizedGreedy', 'ReactiveGRASP']
    instances_sorted = sorted(list(set(df['instance'])))

    # --- TABELA 1: MELHOR SOLUÇÃO ---
    print("\n% --- TABELA 1: Desvios da MELHOR Solução (Best of 10) ---")
    print("\\begin{table}[H]")
    print("\\centering")
    print("\\small")
    print("\\caption{Desvio percentual (Gap) da \\textbf{Melhor Solução} encontrada}")
    print("\\label{tab:gap_melhor}")
    print("\\begin{tabular}{l c c c}")
    print("\\toprule")
    print("\\textbf{Instância} & \\textbf{Gap Guloso (\\%)} & \\textbf{Gap Random (\\%)} & \\textbf{Gap Reativo (\\%)} \\\\")
    print("\\midrule")

    for inst in instances_sorted:
        bk = BEST_KNOWN.get(inst)
        if not bk: continue
        row_str = f"{inst}"
        for alg in algorithms:
            try:
                val = df_pivot.loc[inst, ('solution_cost_min', alg)]
                gap = get_gap(val, bk)
                row_str += f" & {format_gap(gap)}"
            except: row_str += " & -"
        print(row_str + " \\\\")
    print("\\bottomrule")
    print("\\end{tabular}")
    print("\\end{table}")

    # --- TABELA 2: MÉDIA DAS SOLUÇÕES ---
    print("\n% --- TABELA 2: Desvios da MÉDIA (Average of 10) ---")
    print("\\begin{table}[H]")
    print("\\centering")
    print("\\small")
    print("\\caption{Desvio percentual (Gap) da \\textbf{Média das Soluções}}")
    print("\\label{tab:gap_media}")
    print("\\begin{tabular}{l c c c}")
    print("\\toprule")
    print("\\textbf{Instância} & \\textbf{Gap Guloso (\\%)} & \\textbf{Gap Random (\\%)} & \\textbf{Gap Reativo (\\%)} \\\\")
    print("\\midrule")

    for inst in instances_sorted:
        bk = BEST_KNOWN.get(inst)
        if not bk: continue
        row_str = f"{inst}"
        for alg in algorithms:
            try:
                val = df_pivot.loc[inst, ('solution_cost_mean', alg)]
                gap = get_gap(val, bk)
                row_str += f" & {format_gap(gap)}"
            except: row_str += " & -"
        print(row_str + " \\\\")
    print("\\bottomrule")
    print("\\end{tabular}")
    print("\\end{table}")

    # --- TABELA 3: TEMPOS MÉDIOS ---
    print("\n% --- TABELA 3: Tempos Médios de Execução ---")
    print("\\begin{table}[H]")
    print("\\centering")
    print("\\small")
    print("\\caption{Tempo Médio de Processamento (s)}")
    print("\\label{tab:tempos}")
    print("\\begin{tabular}{l c c c}")
    print("\\toprule")
    print("\\textbf{Instância} & \\textbf{Guloso (s)} & \\textbf{Random (s)} & \\textbf{Reativo (s)} \\\\")
    print("\\midrule")

    for inst in instances_sorted:
        row_str = f"{inst}"
        for alg in algorithms:
            try:
                val = df_pivot.loc[inst, ('execution_time_mean', alg)]
                row_str += f" & {val:.4f}"
            except: row_str += " & -"
        print(row_str + " \\\\")
    print("\\bottomrule")
    print("\\end{tabular}")
    print("\\end{table}")

if __name__ == "__main__":
    generate_report()