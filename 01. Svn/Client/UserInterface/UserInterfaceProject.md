# UserInterface project notes

Add these files to the UserInterface project and filters if your fork uses Visual Studio project files:

```txt
PythonPassiveAttr.cpp
PythonPassiveAttr.h
```

The current source tree already had these entries in:

```txt
Srcs/Client/vs_files/UserInterface/UserInterface.vcxproj
Srcs/Client/vs_files/UserInterface/UserInterface.vcxproj.filters
```

If your target fork does not have them, add `PythonPassiveAttr.cpp` as `ClCompile` and `PythonPassiveAttr.h` as `ClInclude`.
