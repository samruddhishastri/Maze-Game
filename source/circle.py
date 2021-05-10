import math

n = 100
a = 2*math.pi/n
fact = 0.5
k = 0

ratio_y = 12
ratio_z = 15

for i in range(1, n+1):
    print("0.0f, " + str(fact*math.cos(i*a)/ratio_y) + "f, " + str(fact*math.sin(i*a)/ratio_z) + "f,")
    print("0.0f, "+ str(fact*math.cos(i*a + a)/ratio_y) + "f, " + str(fact*math.sin(i*a + a)/ratio_z) + "f,")
    print("0.0f, 0.0f, 0.0f,")
    k += 1
    print("\n")

print(k)