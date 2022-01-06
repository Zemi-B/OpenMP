import matplotlib.pyplot as plt
import japanize_matplotlib

labels = ['4近傍', '50*50', '50*50(累積和)']
x = [1, 2, 4, 8]

f = open('out.txt', 'r')
v = list(map(float, f.read().split()))
y = [[] for _ in labels]
for i in range(len(labels)):
    for j in range(len(x)):
        y[i].append(v[i*len(x)+j])

plt.figure()
for i in range(len(labels)):
    plt.plot(x, y[i], label=labels[i], marker='o')

plt.xlabel("スレッド数")
plt.ylabel("実行時間[s]")
plt.xscale("log", base=2)
plt.legend()
plt.grid()
plt.savefig('../img/fig.png')
