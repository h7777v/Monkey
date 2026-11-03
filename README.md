# Monkey
Probably the most useless C library to exist... 
Good for trolling. 
Literally a memory vandalism machine. 

TODO: make a C++ header so C++ ppl can use it. 

# Usage

DISCLAIMER: This library contains code that relies on UB and may cause SEGV(on linux) or Access Violations(on windows) or other crashes. 

```c
int goof(void* ptr);
```

Randomly overwrites a byte in the OS page of the received pointer. 

```c
int mgoof(void* ptr, size_t times);
```

Basically an optimised version of goof() but accepts an extra arg that specifies how many times it will run goof().  

```c
int chaos(void* ptr);
```

Scrambles EVERYTHING in the page of the given pointer. This causes ultimate destruction and most likely result in a crash. 

```c
int archeology(void* ptr);
```

Lets you go on an archeology mission which will dig up the area where the pointer receieved lives. You can explore many places(without mosquitoes and a passport) such as the stack, the heap, .text, .data, .rodata/.rdata and even .bss(theres probably nothing interesting  there). 