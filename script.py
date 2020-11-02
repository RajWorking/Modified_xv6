import matplotlib
import matplotlib.pyplot as plt

LOGS = ""

with open("inp.txt") as f:
    LOGS = f.read()

# print(LOGS)
inp = LOGS.strip().split('\n')
N = 0

graph_data = {}

for line in inp:
    words = line.split(' ')
    if words[0] == 'graph' and len(words) == 4:
        N = N + 1

        time = int(words[1])
        pid = int(words[2])
        queue = int(words[3])

        if pid not in graph_data:
            graph_data[pid] = {"time": [time], "queue": [queue]}
        else:
            graph_data[pid]["time"].append(time)
            graph_data[pid]["queue"].append(queue)

for pid in graph_data:
    print(pid)
    plt.plot(graph_data[pid]["time"],graph_data[pid]["queue"], label=pid)

plt.legend()
plt.title('MLFQ scheduling (4 CPU)')
plt.xlabel('number of ticks')
plt.ylabel('queue')
plt.show()
# print(N)