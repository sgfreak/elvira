# "vi" in elvira is for "vi", the unix editor.

elvira is a NOT bloated editor made when i was bored.
clearly inspired by vi, elvis, nvi, busybox vi and other editor slop
theres no makefile because theres no need to.

## Compiling (with small binary size)
```gcc -Os -s -ffunction-sections -fdata-sections -Wl,--gc-sections elvira.c -o elvira```


**You can also use the binutils strip if you want a really smol binary**
```strip -s elvira```

compiling with normal size is just gcc -o elvira elvira.c

- Churcs of emacs approved ✅✅✅
