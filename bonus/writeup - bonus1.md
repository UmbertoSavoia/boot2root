# Bonus 1

Using the previously extracted `filesystem.squashfs` file, I read the 
`.bash_history` file of the user `root` and found the password of the user `zaz`

```shell
$ sudo bat ./root/.bash_history
 402   │ adduser zaz
 403   │ 646da671ca01bb5d84dbb5fb2238dc8e
```