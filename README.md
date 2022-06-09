<h1 align="center"> Linda multi-process communication system using FIFO (named pipes). </h1>

## 1. Authors:
+ Bartosz Cywiński
+ Łukasz Staniszewski
+ Mateusz Szczepanowski
+ Albert Ściseł

## 2. About:
Linda is a communication language, which realize three specific operations:

```cpp
output(tuple);
input(tuple_pattern, timeout);
read(tuple_pattern, timeout);
```

Goal of this project was to create solution which uses Linda to realize multi-process communication system. In the system, there are client processes, between which communication is taking place, and one server process, which is system's central supervisor.

Structures that are transmitted between processes are named <b>tuples</b> - tables of any length, which elements can be of type <b>string</b>, <b>integer</b>, <b>float</b>, example tuple:

```python
(int:3,str:"abc",float:2.5)
```
Client, to specify, what kind of tuple he wants, needs to specify <b>tuple pattern</b> in his request, when he uses <b>input</b> or <b>read</b> method, example tuple pattern:

```python
(int:=3,str:*,float:<2.5)
```

To make tuple's space, we had to use IPC Unix <b> Named Pipes (FIFO)</b>.

## 3. Tools:
+ Programming languages: C++11 (system implementation), Python (integration tests).
+ To build program: Makefile.
+ Linter: clang-tidy.
