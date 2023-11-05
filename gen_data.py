import numpy as np
from argparse import ArgumentParser

parser = ArgumentParser()
parser.add_argument("-f", "--file", dest="filename",
                    help="write data to FILE", metavar="FILE")
parser.add_argument("dimensions", type=int, nargs=2)
args = parser.parse_args()

sizeA = args.dimensions[0]
sizeB = args.dimensions[1]
array = np.random.randint(50, size= (sizeA, sizeB))
kernel = np.random.randint(50, size=(3,3))
with open(args.filename, 'w') as f:
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
    

