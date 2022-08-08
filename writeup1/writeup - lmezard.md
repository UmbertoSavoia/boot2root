# lmezard

```shell
$ sudo ls -lR ./home/lmezard
./home/lmezard:
total 796
-rwxr-x--- 1 1001 docker 808960 Oct  8  2015 fun
-rwxr-x--- 1 1001 docker     96 Oct 15  2015 README

$ sudo bat ./home/lmezard/README
───────┬───────────────────────────────────────────────────────────────────────
       │ File: ./home/lmezard/README
───────┼───────────────────────────────────────────────────────────────────────
   1   │ Complete this little challenge and use the result as password for user
       |   'laurie' to login in ssh
───────┴───────────────────────────────────────────────────────────────────────


$ sudo file ./home/lmezard/fun
./home/lmezard/fun: POSIX tar archive (GNU)

$ sudo tar -xf unpacked_filesystem/home/lmezard/fun

$ ls -l
total 417832
drwxr-x---  2 umberto users       28672 Sep 15  2015 ft_fun
-rw-r--r--  1 umberto umberto      2137 Jul  9 17:55 order_pcap.c

$ gcc order_pcap.c -o order_pcap

$ ./order_pcap ./ft_fun > out.c

$ gcc out.c -o out

$ ./out
MY PASSWORD IS: Iheartpwnage
Now SHA-256 it and submit

$ echo -n "Iheartpwnage" | openssl dgst -sha256
(stdin)= 330b845f32185747e4f8ca15d40ca59796035c89ea809fb5d30f4da83ecf45a4

umberto@DESKTOP-4VV8PNQ: ~/boot2root
$ ssh laurie@192.168.56.143
        ____                _______    _____
       |  _ \              |__   __|  / ____|
       | |_) | ___  _ __ _ __ | | ___| (___   ___  ___
       |  _ < / _ \| '__| '_ \| |/ _ \\___ \ / _ \/ __|
       | |_) | (_) | |  | | | | | (_) |___) |  __/ (__
       |____/ \___/|_|  |_| |_|_|\___/_____/ \___|\___|

                       Good luck & Have fun
laurie@192.168.56.143's password:
laurie@BornToSecHackMe:~$ id
uid=1003(laurie) gid=1003(laurie) groups=1003(laurie)
laurie@BornToSecHackMe:~$
```