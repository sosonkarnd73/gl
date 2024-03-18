import random
sum = 0
n = 100000
N = 10
for i in range(1,N):
    num=0
    count=0
    for i in range(1,n):
        x = random.random()
        y = random.random()
        z = (x**2+y**2)
        
        if(z<=1.000):
            count = count+1
    pi = (4*count)/n
    print("pi : ",pi)

#    sum = sum +pi
#//pi = sum/N
