import numpy as np

sizeA = 10000
sizeB = 10000
array = np.random.randint(50, size= (sizeA, sizeB))
kernel = np.random.randint(50, size=(3,3))
with open("data/lab2/data3.txt", 'w') as f:
    f.write(f'{sizeA} {sizeB}\n')
    for line in array:
        for nr in line:
            f.write(f'{nr} ')
        f.write('\n')

    f.write('3 3\n')
    for line in kernel:
        for nr in line:
            f.write(f'{nr} ')
        f.write('\n')
    

