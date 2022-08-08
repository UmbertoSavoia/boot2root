## VMWare interfaccia:
``` ipconfig /all ```
```shell
Scheda Ethernet VMware Network Adapter VMnet8:

   Suffisso DNS specifico per connessione:
   Descrizione . . . . . . . . . . . . . : VMware Virtual Ethernet Adapter for VMnet8
   Indirizzo IPv4. . . . . . . . . . . . : 192.168.56.1(Preferenziale)
   Subnet mask . . . . . . . . . . . . . : 255.255.255.0
```
```shell
$ nmap 192.168.56.1-255
Starting Nmap 7.80 ( https://nmap.org ) at 2022-07-04 20:21 CEST
Nmap scan report for 192.168.56.143
Host is up (0.042s latency).
Not shown: 994 closed ports
PORT    STATE SERVICE
21/tcp  open  ftp
22/tcp  open  ssh
80/tcp  open  http
143/tcp open  imap
443/tcp open  https
993/tcp open  imaps

Nmap done: 255 IP addresses (1 host up) scanned in 61.52 seconds
```
