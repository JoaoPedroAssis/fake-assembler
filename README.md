# Fake-Assembler
This is the first assignment for the System Software discipline, which will contain an assembler for a fake assembly language. The repository can be found at https://github.com/JoaoPedroAssis/fake-assembler

## Compilation instructions

A makefile can be found on the root directory. Once in there, type `make` to initiate the compilation process. On the same directory, an executable named `montador` will be found. To execute this program, follow this instructions:

### Pre-process and assemble

To pre-process and assemble the file, type:

```
$ ./montador arquivo.asm
```
The output file will be found in the same directory, under the name `arquivo.obj`

### Pre-process only

To pre-process the file and inspect it's output, type:

```
$ ./montador -p arquivo.asm
```
The output file will be found in the same directory, under the name `arquivo.pre`

### Assemble from pre-processed file

To assemble a file that has already been pre-processed, type:

```
$ ./montador -o arquivo.pre
```
Note that the input file must have a `.pre` extension

---

Programmed by João Pedro Assis for the System Software discipline at Universidade de Brasília - 11/10/2020 😎