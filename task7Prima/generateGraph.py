from random import choice

def generateGraph(filename, nVertices):
    graph = [[0 for _ in range(nVertices)] for _ in range(nVertices)]
    values = ["inf"] + [i for i in range(-10, 11)]
    for i in range(nVertices):
        for j in range(i, nVertices):
            if i != j:
                graph[i][j] = choice(values)
                graph[j][i] = graph[i][j]

    with open(filename, "w") as file:
        for row in graph:
            for edge in row:
                file.write(str(edge) + "\t")
            file.write("\n")


generateGraph(input("filename: "), int(input("nVertices: ")))