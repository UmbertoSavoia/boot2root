# thor
```shell
thor@BornToSecHackMe:~$ ls -l
total 32
-rwxr-x--- 1 thor thor    69 Oct  8  2015 README
-rwxr-x--- 1 thor thor 31523 Oct  8  2015 turtle

thor@BornToSecHackMe:~$ file turtle
turtle: ASCII text
```
After quickly reading the `turtle` file, it is quite easy to understand that 
these are instructions for a drawing, so I googled` turtle draw` and among the 
first searches the Python library appeared, after writing a simple program that 
drawn the instructions in the file, I discovered the word `SLASH`.

```python
───────┬────────────────────────────────────────────────────────────────────────
       │ File: pyTurtle.py
───────┼────────────────────────────────────────────────────────────────────────
   1   │ from turtle import *
   2   │ import sys
   3   │ import re
   4   │ import time
   5   │
   6   │ usage = "\n    Usage: python3 pyTurtle.py <file>\n"
   7   │
   8   │ patterns = {
   9   │         r"Tourne gauche de ([0-9]*) degrees":left,
  10   │         r"Tourne droite de ([0-9]*) degrees":right,
  11   │         r"Avance ([0-9]*) spaces":forward,
  12   │         r"Recule ([0-9]*) spaces":backward,
  13   │         }
  14   │ op = []
  15   │
  16   │ if len(sys.argv) != 2:
  17   │     sys.exit(usage)
  18   │ try:
  19   │     f = open(sys.argv[1], "r")
  20   │ except:
  21   │     sys.exit(usage)
  22   │ for line in f:
  23   │     if line == '\n':
  24   │         op.append([0,'stop'])
  25   │     else:
  26   │         for k, v in patterns.items():
  27   │             k = re.search(k, line)
  28   │             if k:
  29   │                 op.append([v, k.group(1)])
  30   │ f.close()
  31   │
  32   │ for i in op:
  33   │     if i[0] == 0 and i[1] == 'stop':
  34   │         time.sleep(1)
  35   │         reset()
  36   │     else:
  37   │         i[0](int(i[1]))
  38   │ done()
───────┴────────────────────────────────────────────────────────────────────────

```

In the last line of the `turtle` file it is suggested to apply a digest 
algorithm.
I immediately tried sha256, but it wasn't right, so I tried md5 and it worked.

```shell
$ echo -n "SLASH" | openssl dgst -md5
(stdin)= 646da671ca01bb5d84dbb5fb2238dc8e
```

```shell
$ ssh zaz@192.168.56.143
        ____                _______    _____
       |  _ \              |__   __|  / ____|
       | |_) | ___  _ __ _ __ | | ___| (___   ___  ___
       |  _ < / _ \| '__| '_ \| |/ _ \\___ \ / _ \/ __|
       | |_) | (_) | |  | | | | | (_) |___) |  __/ (__
       |____/ \___/|_|  |_| |_|_|\___/_____/ \___|\___|

                       Good luck & Have fun
zaz@192.168.56.143's password:
zaz@BornToSecHackMe:~$ id
uid=1005(zaz) gid=1005(zaz) groups=1005(zaz)
zaz@BornToSecHackMe:~$
```