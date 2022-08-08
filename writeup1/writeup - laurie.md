# laurie

```shell
laurie@BornToSecHackMe:~$ ls
bomb  README

laurie@BornToSecHackMe:~$ file bomb
bomb: ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.0.0, not stripped

laurie@BornToSecHackMe:~$ cat README
Diffuse this bomb!
When you have all the password use it as "thor" user with ssh.

HINT:
P
 2
 b

o
4

NO SPACE IN THE PASSWORD (password is case sensitive).

$ scp laurie@192.168.56.143:/home/laurie/bomb .
        ____                _______    _____
       |  _ \              |__   __|  / ____|
       | |_) | ___  _ __ _ __ | | ___| (___   ___  ___
       |  _ < / _ \| '__| '_ \| |/ _ \\___ \ / _ \/ __|
       | |_) | (_) | |  | | | | | (_) |___) |  __/ (__
       |____/ \___/|_|  |_| |_|_|\___/_____/ \___|\___|

                       Good luck & Have fun
laurie@192.168.56.143's password:
bomb                        100% 26KB   6.6MB/s   00:00
```

# Bomb Reverse Engineering
## Pseudocode of main function

```C
char *line = 0;

printf("Welcome this is my little bomb !!!! You have 6 stages with\n");
printf("only one life good luck !! Have a nice day!\n");
line = read_line();
phase_1(line);
phase_defused();
printf("Phase 1 defused. How about the next one?\n");
line = read_line();
phase_2(line);
phase_defused();
printf("That's number 2.  Keep going!\n");
line = read_line();
phase_3(line);
phase_defused();
printf("Halfway there!\n");
line = read_line();
phase_4(line);
phase_defused();
printf("So you got that one.  Try this one.\n");
line = read_line();
phase_5(line);
phase_defused();
printf("Good work!  On to the next...\n");
line = read_line();
phase_6(line);
phase_defused();
```

## Pseudocode phase_1
```C
int phase_1(char *line) {
    int result = strings_not_equal(line, "Public speaking is very easy.");
    if (result)
      explode_bomb();
    return result;
}
```
> Answer: Public speaking is very easy.

## Pseudocode phase_2
```C
int phase_2(char *line) {
  int result;
  int numbers[6];

  read_six_numbers(line, numbers); // converts the string to six numbers
  if (numbers[0] != 1)
    explode_bomb();
  for (int i = 1; i < 6; ++i) {
    result = numbers[i - 1] * (i + 1);
    if (numbers[i] != result)
      explode_bomb();
  }
  return result;
}
```
In the suggestions of the Readme the number 2 is indicated as the second number 
in the progression therefore doing a bit of tests using the function separately, 
the only correct progression is the following.
> Answer: 1 2 6 24 120 720

## Pseudocode phase_3
```C
int phase_3(char *line) {
  int result;
  int first;
  char second;
  int third;
  char n;

  if ( sscanf(line, "%d %c %d", &first, &second, &third) <= 2 )
    explode_bomb();
  result = first;
  switch ( first ) {
    case 0:
      n = 113;
      if ( third != 777 )
        explode_bomb();
      return result;
    case 1:
      n = 98;
      if ( third != 214 )
        explode_bomb();
      return result;
    case 2:
      n = 98;
      if ( third != 755 )
        explode_bomb();
      return result;
    case 3:
      n = 107;
      if ( third != 251 )
        explode_bomb();
      return result;
    case 4:
      n = 111;
      if ( third != 160 )
        explode_bomb();
      return result;
    case 5:
      n = 116;
      if ( third != 458 )
        explode_bomb();
      return result;
    case 6:
      n = 118;
      if ( third != 780 )
        explode_bomb();
      return result;
    case 7:
      n = 98;
      if ( third != 524 )
        explode_bomb();
      return result;
    default:
      explode_bomb();
  }
  if ( n != second )
    explode_bomb();
  return result;
}
```
Through the suggestions of the Readme we find the letter 'b' which is surely the 
character scanned by 'sscanf'. So since 'n' and 'second' must be the same, we 
have the following correct answers.

> Answer: 1 b 214

> Answer: 2 b 755

> Answer: 7 b 524

## Pseudocode phase_4
```C
int func4(int a)
{
  int v;

  if ( a <= 1 )
    return 1;
  v = func4(a - 1);
  return v + func4(a - 2);
}

int phase_4(char *line)
{
  int result;
  int n;

  if ( sscanf(line, "%d", &n) != 1 || n <= 0 )
    explode_bomb();
  result = func4(n);
  if ( result != 55 )
    explode_bomb();
  return result;
}
```
With the following code I applied brute force to 'func4' and found the number 
for which the function returns 55
```C
int main(void)
{
    for (int i = 1; i; ++i) {
        if (func4(i) == 55) {
            printf("Number: %d\n", i);
            break;
        }
    }
    return 0;
}
```
> Answer: 9

## Pseudocode phase_5
```C
char array[] = "isrveawhobpnutfg";

int phase_5(char *line)
{
  int result;
  char v[8];

  if ( string_length(line) != 6 )
    explode_bomb();
  for ( int i = 0; i < 6; ++i )
    v[i] = array[line[i] & 0xF];
  v[6] = 0;
  result = strings_not_equal(v, "giants");
  if ( result )
    explode_bomb();
  return result;
}
```
Using the string `"isrveawhobpnutfg"`, the six characters we entered and the 
calculation `line[i] & 0xF`, we must form the string `"giants"`.
```
i s r v e a w h o b p  n  u  t  f  g
0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
```
We need the index: ```15, 0, 5, 11, 13, 1```.
The Readme suggests the character 'o'
```
ASCII    Decimal    Binary
  o   =    111   =  0110 1111
```
With the bitwise operator `&` and the mask `0xF` we isolate the first 4 bits.
With the following code we search for all possible combinations:
```C
int main(void)
{
    unsigned char alpha[] = "abcdefghijklmnopqrstuvwxyz";
    unsigned char v[] = {15, 0, 5, 11, 13, 1};
    
    for (int i = 0; i < sizeof(v); ++i)
        for (int j = 0; j < sizeof(alpha); ++j)
            if ((alpha[j] & 0xF) == v[i])
                printf("v = %2d - c = %c\n", v[i], alpha[j]);
    return 0;
}
```
```
v = 15 - c = o
v =  0 - c = p
v =  5 - c = e
v =  5 - c = u
v = 11 - c = k
v = 13 - c = m
v =  1 - c = a
v =  1 - c = q

opekma
  u  q
  ^  ^
  |  |
  -------- variants
```
> Answer: opekma

> Answer: opukma

> Answer: opekmq

> Answer: opukmq

## Pseudocode phase_6
```C
int node1[] = {253, 1, 134525536, 1001};
int node2[] = {725, 2, 134525524, 253};
int node3[] = {301, 3, 134525512, 725};
int node4[] = {997, 4, 134525500, 301};
int node5[] = {212, 5, 134525488, 997};
int node6[] = {432, 6, 0, 212};

int phase_6(char *s)
{
  int n1, n2, n3, n4;
  int result;
  int *ptr;
  int n5[6];
  int numbers[6];

  read_six_numbers(s, numbers); // converts the string to six numbers
  for ( int i = 0; i <= 5; ++i ) {
    if ( numbers[i] - 1 > 5 )
      explode_bomb();
    for ( int j = i + 1; j <= 5; ++j ) {
      if ( numbers[i] == numbers[j] )
        explode_bomb();
    }
  }
  for ( int i = 0; i <= 5; ++i ) {
    ptr = &node;
    for ( int j = 1; j < numbers[i]; ++j )
      ptr = ptr[2];
    n5[i] = (int)ptr;
  }
  n1 = n4 = n5[0];
  for ( int i = 1; i <= 5; ++i ) {
    n2 = n5[i];
    n1[8] = n2;
    n1 = n2;
  }
  n2[8] = 0;
  n3 = n4;
  for ( int i = 0; i <= 4; ++i ) {
    result = *(int *)n3;
    if ( *(int *)n3 < **(int **)(n3 + 8) )
      explode_bomb();
    n3 = *(int *)(n3 + 8);
  }
  return result;
}
```

```
>>> x/4d $esi-(12*0)
0x804b26c <node1>:      253     1       134525536       1001
>>> x/4d $esi-(12*1)
0x804b260 <node2>:      725     2       134525524       253
>>> x/4d $esi-(12*2)
0x804b254 <node3>:      301     3       134525512       725
>>> x/4d $esi-(12*3)
0x804b248 <node4>:      997     4       134525500       301
>>> x/4d $esi-(12*4)
0x804b23c <node5>:      212     5       134525488       997
>>> x/4d $esi-(12*5)
0x804b230 <node6>:      432     6       0       212
```
The numbers passed to phase_6 are used to check if the numbers in the first 
index of the "node{1,2,3,4,5,6}" arrays are in descending order.
> Answer: 4 2 6 3 1 5

## Solution
The combinations to be tested are as follows:
```
Publicspeakingisveryeasy.126241207201b2149opekma426315
Publicspeakingisveryeasy.126241207201b2149opukma426315
Publicspeakingisveryeasy.126241207201b2149opekmq426315
Publicspeakingisveryeasy.126241207201b2149opukmq426315
Publicspeakingisveryeasy.126241207202b7559opekma426315
Publicspeakingisveryeasy.126241207202b7559opukma426315
Publicspeakingisveryeasy.126241207202b7559opekmq426315
Publicspeakingisveryeasy.126241207202b7559opukmq426315
Publicspeakingisveryeasy.126241207207b5249opekma426315
Publicspeakingisveryeasy.126241207207b5249opukma426315
Publicspeakingisveryeasy.126241207207b5249opekmq426315
Publicspeakingisveryeasy.126241207207b5249opukmq426315
```

But none of the combinations seem to work because unfortunately there is a bug 
in the iso. [Reference](https://stackoverflow.com/c/42network/questions/664/1628)

```
Correct solution:
Publicspeakingisveryeasy.126241207201b2149opekmq426135
```
```shell
$ ssh thor@192.168.56.143
        ____                _______    _____
       |  _ \              |__   __|  / ____|
       | |_) | ___  _ __ _ __ | | ___| (___   ___  ___
       |  _ < / _ \| '__| '_ \| |/ _ \\___ \ / _ \/ __|
       | |_) | (_) | |  | | | | | (_) |___) |  __/ (__
       |____/ \___/|_|  |_| |_|_|\___/_____/ \___|\___|

                       Good luck & Have fun
thor@192.168.56.143's password:
thor@BornToSecHackMe:~$ id
uid=1004(thor) gid=1004(thor) groups=1004(thor)
thor@BornToSecHackMe:~$
```
