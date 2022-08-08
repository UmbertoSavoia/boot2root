## Mount iso
```shell
$ sudo mkdir /mnt/boot2root
$ sudo mount BornToSecHackMe-v1.1.iso /mnt/boot2root
$ ls -R /mnt/boot2root
/mnt/boot2root:
casper  install  isolinux  md5sum.txt  preseed  README.diskdefines  ubuntu

/mnt/boot2root/casper:
filesystem.manifest  filesystem.manifest-desktop  filesystem.size  filesystem.squashfs  initrd.gz  README.diskdefines  vmlinuz

/mnt/boot2root/install:
memtest

/mnt/boot2root/isolinux:
boot.cat  isolinux.bin  isolinux.cfg  splash.png  vesamenu.c32

/mnt/boot2root/preseed:
custom.seed

$ sudo unsquashfs -f -d ./unpacked_filesystem /mnt/boot2root/casper/filesystem.squashfs
Parallel unsquashfs: Using 16 processors
61188 inodes (56421 blocks) to write

[==========================================================/ ] 56393/56421  99%

created 54391 files
created 8445 directories
created 6743 symlinks
created 2 devices
created 2 fifos

$ ls unpacked_filesystem
bin  boot  dev  etc  home  initrd.img  lib  media  mnt  opt  proc  root  run  sbin  selinux  srv  sys  tmp  usr  var  vmlinuz

```

## System inspection
```shell
$ bat ./etc/passwd
───────┬────────────────────────────────────────────────────────────────────────
       │ File: ./etc/passwd
───────┼────────────────────────────────────────────────────────────────────────
   1   │ root:x:0:0:root:/root:/bin/bash
   2   │ daemon:x:1:1:daemon:/usr/sbin:/bin/sh
   3   │ bin:x:2:2:bin:/bin:/bin/sh
   4   │ sys:x:3:3:sys:/dev:/bin/sh
   5   │ sync:x:4:65534:sync:/bin:/bin/sync
   6   │ games:x:5:60:games:/usr/games:/bin/sh
   7   │ man:x:6:12:man:/var/cache/man:/bin/sh
   8   │ lp:x:7:7:lp:/var/spool/lpd:/bin/sh
   9   │ mail:x:8:8:mail:/var/mail:/bin/sh
  10   │ news:x:9:9:news:/var/spool/news:/bin/sh
  11   │ uucp:x:10:10:uucp:/var/spool/uucp:/bin/sh
  12   │ proxy:x:13:13:proxy:/bin:/bin/sh
  13   │ www-data:x:33:33:www-data:/var/www:/bin/sh
  14   │ backup:x:34:34:backup:/var/backups:/bin/sh
  15   │ list:x:38:38:Mailing List Manager:/var/list:/bin/sh
  16   │ irc:x:39:39:ircd:/var/run/ircd:/bin/sh
  17   │ gnats:x:41:41:Gnats Bug-Reporting System (admin):/var/lib/gnats:/bin/sh
  18   │ nobody:x:65534:65534:nobody:/nonexistent:/bin/sh
  19   │ libuuid:x:100:101::/var/lib/libuuid:/bin/sh
  20   │ syslog:x:101:103::/home/syslog:/bin/false
  21   │ messagebus:x:102:106::/var/run/dbus:/bin/false
  22   │ whoopsie:x:103:107::/nonexistent:/bin/false
  23   │ landscape:x:104:110::/var/lib/landscape:/bin/false
  24   │ sshd:x:105:65534::/var/run/sshd:/usr/sbin/nologin
  25   │ ft_root:x:1000:1000:ft_root,,,:/home/ft_root:/bin/bash
  26   │ mysql:x:106:115:MySQL Server,,,:/nonexistent:/bin/false
  27   │ ftp:x:107:116:ftp daemon,,,:/srv/ftp:/bin/false
  28   │ lmezard:x:1001:1001:laurie,,,:/home/lmezard:/bin/bash
  29   │ laurie@borntosec.net:x:1002:1002:Laurie,,,:/home/laurie@borntosec.net:/bin/bash
  30   │ laurie:x:1003:1003:,,,:/home/laurie:/bin/bash
  31   │ thor:x:1004:1004:,,,:/home/thor:/bin/bash
  32   │ zaz:x:1005:1005:,,,:/home/zaz:/bin/bash
  33   │ dovecot:x:108:117:Dovecot mail server,,,:/usr/lib/dovecot:/bin/false
  34   │ dovenull:x:109:65534:Dovecot login user,,,:/nonexistent:/bin/false
  35   │ postfix:x:110:118::/var/spool/postfix:/bin/false

$ sudo ls -R ./home
./home:
ft_root  laurie  laurie@borntosec.net  lmezard  LOOKATME  thor  zaz

./home/ft_root:
Desktop  mail

./home/ft_root/Desktop:

./home/ft_root/mail:
INBOX.Drafts  INBOX.Sent  INBOX.Trash

./home/laurie:
bomb  README

./home/laurie@borntosec.net:
mail

./home/laurie@borntosec.net/mail:
INBOX.Drafts  INBOX.Sent  INBOX.Trash

./home/lmezard:
fun  README

./home/LOOKATME:
password

./home/thor:
README  turtle

./home/zaz:
exploit_me  mail

./home/zaz/mail:
INBOX.Drafts  INBOX.Sent  INBOX.Trash
```

```
$ sudo ls -l LOOKATME
total 4
-rwxr-x--- 1 www-data www-data 25 Oct  8  2015 password

$ sudo cat LOOKATME/password
lmezard:G!@M6f4Eatau{sF"
```