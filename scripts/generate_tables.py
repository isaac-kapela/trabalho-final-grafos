import pandas as pd
import sys

# Configuração
INPUT_CSV = "results/log_execucao.csv"

# Melhores soluções conhecidas (Extraído do seu bestSolutions.txt)
BEST_KNOWN = {
    # CRD
    "crd100": 6194, "crd101": 7004, "crd102": 6831, "crd103": 7041, "crd104": 6946,
    "crd105": 6449, "crd106": 6248, "crd107": 7125, "crd108": 6531, "crd109": 6613,
    "crd300": 3634, "crd301": 3277, "crd302": 4001, "crd303": 3861, "crd304": 3931,
    "crd305": 3774, "crd306": 3801, "crd307": 3927, "crd308": 3714, "crd309": 3730,
    "crd500": 4931, "crd501": 5126, "crd502": 4898, "crd503": 4543, "crd504": 4733,
    "crd505": 4892, "crd506": 4691, "crd507": 4761, "crd508": 5008, "crd509": 4607,
    "crd700": 5789, "crd701": 5848, "crd702": 6167, "crd703": 5878, "crd704": 5590,
    "crd705": 5853, "crd706": 6078, "crd707": 5985, "crd708": 5803, "crd709": 5502,

    # RAND
    "rand200": 668, "rand300": 678, "rand400": 676, # Assumindo grau 4 para rand

    # SHRD
    "shrd150": 430, "shrd159": 430, "shrd200": 802, "shrd209": 799,
    "shrd258": 1276, "shrd259": 1016, "shrd300": 1905, "shrd309": 1474,

    # STR
    "str300": 3761, "str301": 3753, "str302": 5237, "str303": 5152, "str304": 6559,
    "str305": 6467, "str306": 8066, "str307": 7964, "str308": 9223, "str309": 9367,
    "str500": 3956, "str501": 4095, "str502": 5634, "str503": 5530, "str504": 7105,
    "str505": 6880, "str506": 8645, "str507": 8582, "str508": 10138, "str509": 10123,
    "str700": 4245, "str701": 4217, "str702": 5877, "str703": 5880, "str704": 7694,
    "str705": 7496, "str706": 9153, "str707": 9256, "str708": 10674, "str709": 10561,
    
    # SYM
    "sym300": 965, "sym301": 1219, "sym302": 1256, "sym303": 1090, "sym304": 1686,
    "sym305": 904, "sym306": 976, "sym307": 1271, "sym308": 1506, "sym309": 1302,
    "sym500": 1105, "sym501": 1065, "sym502": 1416, "sym503": 1367, "sym504": 1241,
    "sym505": 1276, "sym506": 1256, "sym507": 1102, "sym508": 1266, "sym509": 1160,
    "sym700": 1234, "sym701": 1198, "sym702": 1234, "sym703": 992, "sym704": 1335,
    "sym705": 1298, "sym706": 963, "sym707": 1447, "sym708": 1284, "sym709": 1044
}

def generate_latex_tables():
    try:
        df = pd.read_csv(INPUT_CSV)
    except FileNotFoundError:
        print(f"Erro: Arquivo {INPUT_CSV} não encontrado.")
        return

    # Limpar espaços em branco e garantir nomes corretos
    df.columns = df.columns.str.strip()
    
    # Tenta converter solution_cost para numérico
    df['solution_cost'] = pd.to_numeric(df['solution_cost'], errors='coerce')

    # CORREÇÃO DO BUG DE TUPLA: Agrupar por string simples
    grouped = df.groupby('instance')

    print("% --- TABELA 1: RESULTADOS COMPARATIVOS ---")
    print("\\begin{table}[h]")
    print("\\centering")
    print("\\caption{Resultados obtidos e comparação com a literatura}")
    print("\\label{tab:resultados_gap}")
    print("\\begin{tabular}{l c c c}")
    print("\\toprule")
    print("\\textbf{Instância} & \\textbf{Lit.} & \\textbf{Melhor Sol.} & \\textbf{Gap (\\%)} \\\\")
    print("\\midrule")

    # Lista para calcular média dos gaps
    gaps = []

    for instance, group in grouped:
        best_found = group['solution_cost'].min()
        best_known_val = BEST_KNOWN.get(instance, None)
        
        gap_str = "-"
        lit_str = "-"
        
        if best_known_val:
            lit_str = f"{best_known_val}"
            if pd.notna(best_found):
                # Cálculo do Gap
                gap = ((best_found - best_known_val) / best_known_val) * 100
                gaps.append(gap)
                gap_str = f"{gap:.2f}"
                if abs(gap) <= 0.001:
                     gap_str = "\\textbf{0.00}"
            else:
                gap_str = "Erro"
        
        best_found_str = f"{best_found:.2f}" if pd.notna(best_found) else "Falha"

        # Imprime a linha corrigida (sem parênteses no nome)
        print(f"{instance} & {lit_str} & {best_found_str} & {gap_str} \\\\")

    # Linha de Média
    if gaps:
        avg_gap = sum(gaps) / len(gaps)
        print("\\midrule")
        print(f"\\textbf{{Média}} & - & - & \\textbf{{{avg_gap:.2f}}} \\\\")

    print("\\bottomrule")
    print("\\end{tabular}")
    print("\\end{table}")
    
    print("\n% --- TABELA 2: TEMPOS DE EXECUÇÃO ---")
    print("\\begin{table}[h]")
    print("\\centering")
    print("\\caption{Tempo médio de execução (s)}")
    print("\\label{tab:tempos}")
    print("\\begin{tabular}{l c}")
    print("\\toprule")
    print("\\textbf{Instância} & \\textbf{Tempo Médio (s)} \\\\")
    print("\\midrule")
    
    for instance, group in grouped:
        avg_time = group['execution_time'].mean()
        print(f"{instance} & {avg_time:.4f} \\\\")
        
    print("\\bottomrule")
    print("\\end{tabular}")
    print("\\end{table}")

if __name__ == "__main__":
    try:
        import pandas
        generate_latex_tables()
    except ImportError:
        print("Erro: Biblioteca 'pandas' não instalada.")