import matplotlib.pyplot as plt

sizes = []
selection = []
quick = []

with open("results.txt", "r") as f:
    lines = f.readlines()
    for line in lines[1:]:  
        line = line.strip().replace(",", ".")
        parts = line.split()  
        
        if len(parts) >= 3:  
            sizes.append(int(parts[0]))
            selection.append(float(parts[1]))
            quick.append(float(parts[2]))


plt.figure(figsize=(14, 10))

plt.plot(sizes, selection, "ro-", linewidth=2, markersize=8, label="Сортировка выбором")

plt.plot(sizes, quick, "bo-", linewidth=2, markersize=8, label="Быстрая сортировка")

plt.title("Сравнение времени выполнения сортировок", fontsize=16, fontweight='bold')
plt.xlabel("Размер массива (количество элементов)", fontsize=14)
plt.ylabel("Время выполнения (секунды)", fontsize=14)


plt.xscale('log')
plt.yscale('log')


plt.legend(fontsize=12, loc='upper left')

# Сетка
plt.grid(True, which="both", linestyle='--', alpha=0.7)
plt.grid(True, which="minor", linestyle=':', alpha=0.4)

# Подписи значений на графике - немного уменьшаем шрифт
#for i, (size, sel, q) in enumerate(zip(sizes, selection, quick)):
 #   plt.text(size, sel, f'{sel:.2f}s', fontsize=8, ha='right', va='bottom')
  #  plt.text(size, q, f'{q:.2f}s', fontsize=8, ha='right', va='top'

# Увеличиваем отступы и сохраняем график
plt.subplots_adjust(left=0.1, right=0.95, top=0.9, bottom=0.1)
plt.tight_layout()

plt.savefig("graph.png", dpi=300, bbox_inches='tight')

print("\n" + "="*60)
print("Сравнение методов сортировки:")
print("="*60)
print(f"{'Размер':<10} {'Выбором (с)':<15} {'Быстрая (с)':<15} {'Ускорение':<10}")
print("-"*60)


print("\nГрафик сохранен как graph.png")
print("="*60)

# Показываем график
plt.show()
