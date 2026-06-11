# Game build notes

Add this source file to the game build list:

```txt
passive_attr.cpp
```

The current source tree already has it in `Srcs/Server/game/src/Makefile`.

If your target fork uses Visual Studio, also add `passive_attr.cpp` and `passive_attr.h` to the game project.
