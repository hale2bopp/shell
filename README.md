# shell
A simple shell. Running in C++. 
- Displays prompt and takes inputs 
- Tokenises based on spaces in inputs 
- Executes a limited set of commands
- Allows backspace and command history (up arrow, down arrow)

## Build 
### Generate Makefile 
```
cd build && cmake -G "Unix Makefiles" ..
```

### Build Tests 
```
cd build/test && make
```

### Build Src
```
cd build/src && make
```

### Build All
```
cd build && make
```

## Run
### Test
```
./build/test/shell_tst
```

### Src 
```
./build/src/shell_run
```



  

