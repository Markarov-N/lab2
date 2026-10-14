"""Генерация тестовых матриц для умножения."""
import random
import sys

def generate_matrix(filename, n, low=-10, high=10, seed=None):
    if seed is not None:
        random.seed(seed)
    with open(filename, 'w') as f:
        f.write(f"{n}\n")
        for i in range(n):
            row = [round(random.uniform(low, high), 4) for _ in range(n)]
            f.write(" ".join(map(str, row)) + "\n")
    print(f"Сгенерирована матрица {n}x{n} -> {filename}")

if __name__ == "__main__":
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 100
    generate_matrix("matrix_a.txt", n, seed=42)
    generate_matrix("matrix_b.txt", n, seed=1337)
    print(f"Готово. Файлы: matrix_a.txt, matrix_b.txt (размер {n}x{n})")
