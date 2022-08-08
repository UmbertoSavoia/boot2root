from turtle import *
import sys
import re
import time

usage = "\n    Usage: python3 pyTurtle.py <file>\n"

patterns = {
        r"Tourne gauche de ([0-9]*) degrees":left,
        r"Tourne droite de ([0-9]*) degrees":right,
        r"Avance ([0-9]*) spaces":forward,
        r"Recule ([0-9]*) spaces":backward,
        }
op = []

if len(sys.argv) != 2:
    sys.exit(usage)
try:
    f = open(sys.argv[1], "r")
except:
    sys.exit(usage)
for line in f:
    if line == '\n':
        op.append([0,'stop'])
    else:
        for k, v in patterns.items():
            k = re.search(k, line)
            if k:
                op.append([v, k.group(1)])
f.close()

for i in op:
    if i[0] == 0 and i[1] == 'stop':
        time.sleep(1)
        reset()
    else:
        i[0](int(i[1]))
done()
