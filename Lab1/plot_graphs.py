import os
import plotly.graph_objects as go
from plotly.subplots import make_subplots

if not os.path.exists("benchmark_results.py"):
    print("Ошибка: benchmark_results.py не найден!")
    exit(1)

from benchmark_results import results

n_list = sorted(results.keys())
time_pointwise = [results[n]["pointwise"] for n in n_list]

# Базовые блоки для общего графика
display_r = [16, 32, 56, 64, 128, 256]
palette = ["#4A90E2", "#50E3C2", "#2ECC71", "#9B59B6", "#E67E22", "#34495E"]

# Создаем дашборд из 3 графиков
fig = make_subplots(
    rows=3,
    cols=1,
    shared_xaxes=True,
    vertical_spacing=0.08,
    subplot_titles=(
        "<b>1. Общее время выполнения от N (мс)</b>",
        "<b>2. Ускорение относительно точечного (Speedup)</b>",
        "<b>3. SIMD-эффект: Сравнение r = 54, 55 и 56 (Кратность 8 для AVX2)</b>",
    ),
)

# =====================================================================
# 1. ОБЩЕЕ ВРЕМЯ (без r=1)
# =====================================================================
fig.add_trace(
    go.Scatter(
        x=n_list,
        y=time_pointwise,
        mode="lines+markers",
        name="Точечный",
        line=dict(color="black", width=2.5, dash="dash"),
        marker=dict(size=6, symbol="x"),
    ),
    row=1,
    col=1,
)

for idx, r in enumerate(display_r):
    times = [results[n]["blocks"].get(r, None) for n in n_list]
    fig.add_trace(
        go.Scatter(
            x=n_list,
            y=times,
            mode="lines+markers",
            name=f"Блок r={r}",
            line=dict(color=palette[idx], width=1.8),
            marker=dict(size=4),
        ),
        row=1,
        col=1,
    )

# =====================================================================
# 2. УСКОРЕНИЕ
# =====================================================================
fig.add_trace(
    go.Scatter(
        x=n_list,
        y=[1.0] * len(n_list),
        mode="lines",
        name="1.0x (Паритет)",
        line=dict(color="gray", width=1, dash="dot"),
        showlegend=False,
    ),
    row=2,
    col=1,
)

for idx, r in enumerate(display_r):
    speedups = [
        results[n]["pointwise"] / results[n]["blocks"][r]
        if r in results[n]["blocks"]
        else None
        for n in n_list
    ]
    fig.add_trace(
        go.Scatter(
            x=n_list,
            y=speedups,
            mode="lines+markers",
            name=f"Ускорение r={r}",
            line=dict(color=palette[idx], width=1.8),
            marker=dict(size=4),
            showlegend=False,
        ),
        row=2,
        col=1,
    )

# =====================================================================
# 3. БИТВА: r = 54 vs 55 vs 56 (КРАТНОСТЬ 8)
# =====================================================================
# r = 54 (остаток 6)
t_54 = [results[n]["blocks"].get(54, None) for n in n_list]
fig.add_trace(
    go.Scatter(
        x=n_list,
        y=t_54,
        mode="lines+markers",
        name="r = 54 (остаток 6 элементов)",
        line=dict(color="#E74C3C", width=2, dash="dash"),  # красный пунктир
        marker=dict(size=6),
        hovertemplate="r=54<br>N=%{x}<br>Время: %{y:.1f} мс<extra></extra>",
    ),
    row=3,
    col=1,
)

# r = 55 (остаток 7)
t_55 = [results[n]["blocks"].get(55, None) for n in n_list]
fig.add_trace(
    go.Scatter(
        x=n_list,
        y=t_55,
        mode="lines+markers",
        name="r = 55 (остаток 7 элементов)",
        line=dict(color="#F39C12", width=2, dash="dot"),  # оранжевый пунктир
        marker=dict(size=6),
        hovertemplate="r=55<br>N=%{x}<br>Время: %{y:.1f} мс<extra></extra>",
    ),
    row=3,
    col=1,
)

# r = 56 (КРАТНО 8: 8 * 7 - ПОЛНАЯ СКОРОСТЬ AVX2)
t_56 = [results[n]["blocks"].get(56, None) for n in n_list]
fig.add_trace(
    go.Scatter(
        x=n_list,
        y=t_56,
        mode="lines+markers",
        name="★ r = 56 (КРАТНО 8: AVX2 идеал)",
        line=dict(color="#27AE60", width=3.5),  # толстая зеленая линия
        marker=dict(size=8, symbol="star"),
        hovertemplate="<b>r=56 (AVX2 ВЫИГРЫШ)</b><br>N=%{x}<br>Время: %{y:.1f} мс<extra></extra>",
    ),
    row=3,
    col=1,
)

# Настройки оформления
fig.update_xaxes(
    tickvals=n_list,
    ticktext=[str(n) for n in n_list],
    tickangle=-45,
    title_text="<b>Порядок матрицы N</b>",
    gridcolor="#EBEBEB",
    row=3,
    col=1,
)
fig.update_xaxes(gridcolor="#EBEBEB", row=1, col=1)
fig.update_xaxes(gridcolor="#EBEBEB", row=2, col=1)

fig.update_yaxes(
    title_text="<b>Время (мс)</b>", gridcolor="#EBEBEB", row=1, col=1
)
fig.update_yaxes(
    title_text="<b>Ускорение (x)</b>", gridcolor="#EBEBEB", row=2, col=1
)
fig.update_yaxes(
    title_text="<b>Время (мс)</b>", gridcolor="#EBEBEB", row=3, col=1
)

fig.update_layout(
    title=dict(
        text="<b>Исследование блочного алгоритма (Усреднение 3 запуска)</b>",
        font=dict(size=18),
        x=0.5,
    ),
    template="plotly_white",
    hovermode="x unified",
    height=1100,  # Увеличили высоту для 3 графиков
    legend=dict(
        orientation="h",
        yanchor="bottom",
        y=1.02,
        xanchor="center",
        x=0.5,
        font=dict(size=11),
    ),
    margin=dict(l=60, r=40, t=100, b=60),
)

output_file = "benchmark_dashboard.html"
fig.write_html(output_file)
print(f"[OK] Дашборд с анализом AVX2 сохранен в: {output_file}")

import webbrowser

webbrowser.open(os.path.abspath(output_file))