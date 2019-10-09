import random as rand

def generateMatrix(filename, nVerteces):
    values = list(range(1, 11))
    values.append("inf")

    with open(filename + ".txt", "w") as file:
        for i in range(nVerteces):
            for j in range(nVerteces):
                if i != j:
                    file.write(str(rand.choice(values)))
                else:
                    file.write(str(0))
                file.write("\t")
            file.write("\n")

generateMatrix(input("filename: "), int(input("nVerteces: ")))