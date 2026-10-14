"""Построение графика производительности умножения матриц (OpenMP)."""
import matplotlib.pyplot as plt

# Данные из бенчмарка
sizes = [200, 400, 800, 1200, 1600, 2000]

gflops_1 = [6.57, 5.86, 6.42, 3.55, 2.59, 2.65]
gflops_2 = [8.12, 10.18, 12.08, 7.66, 4.48, 5.41]
gflops_4 = [10.88, 20.48, 16.06, 18.02, 8.77, 10.80]
gflops_8 = [10.22, 15.92, 16.74, 25.03, 12.90, 21.36]

plt.figure(figsize=(12, 7))

plt.plot(sizes, gflops_1, 'bo-', linewidth=2, markersize=8, label='1 поток')
plt.plot(sizes, gflops_2, 'rs-', linewidth=2, markersize=8, label='2 потока')
plt.plot(sizes, gflops_4, 'g^-', linewidth=2, markersize=8, label='4 потока')
plt.plot(sizes, gflops_8, 'mD-', linewidth=2, markersize=8, label='8 потоков')

# Подписи значений над точками для 8 потоков (максимум)
for x, y in zip(sizes, gflops_8):
    plt.annotate(f'{y:.2f}', (x, y), textcoords="offset points",
                 xytext=(0, 10), ha='center', fontsize=9)

plt.xlabel('Размер матрицы (n x n)', fontsize=12)
plt.ylabel('Производительность (GFLOPS)', fontsize=12)
plt.title('Производительность умножения матриц (OpenMP)', fontsize=14)
plt.legend(fontsize=11)
plt.grid(True, alpha=0.3)
plt.savefig('performance_graph.png', dpi=300, bbox_inches='tight')
print("График сохранён как performance_graph.png")
plt.show()
