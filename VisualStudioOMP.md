# Visual Studio 2019 - How to make OpenMP work

In the project configuration:

1. Enable multiprocessing flag /MP in General
2. Add library directory C:\Program Files\LLVM\lib
3. Add include directory C:\Program Files\LLVM\lib\clang\11.0.0\include
4. Add library input C:\Program Files\LLVM\lib\libomp.lib and C:\Program Files\LLVM\lib\libiomp5md.lib
5. Add libomp*.dll in _vs/Debug from C:\Program Files\LLVM\bin
