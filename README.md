# Fibonacci LFSR
This repository contains an implementation of the Fibonaci Linear Feedback Shift Register (LFSR) in C, designed for educational cryptographic experimentation.  

## What is LFSR  
An LFSR is one of the many ways to create a pseudo-random number generator. It generates a stream of bits, from a linear function (here, XOR) of the previous state of bits.  
It consists of two core components:  
- **State**: a binary register of $`m`$ bits
- **Feedback Coefficients**: a binary mask of $`m`$ bits that determines which bits are selected for XOR.

## How it Works
An initial state (seed) of $`m`$ bits is set with values  $`s_{m-1},..., s_1, s_0`$  
A constant set of feedback coefficients is also set with values  $`p_{m-1},..., p_1, p_0`$  

The output bit of the LFSR $`s_m`$, which is also the input to the leftmost position on the register, can be computed by the XOR-sum of the products of the state bit and corresponding feedback coefficient:
```math
s_m \equiv s_{m-1}p_{m-1} +...+ s_{1}p_{1} + s_{0}p_{0}\;mod 2;
```
In general:  
```math
s_{i+m} \equiv \sum_{j=0}^{m-1} p_j s_{i+j}\;mod 2;\;\;s_i, p_j \in {0,1};\;i=0,1,...
```
At each iteration, the register is shifted to the right and the previous output bit is made the leftmost bit.  
The least significant bit at each stage is the output of the bit stream.  

## Usage
The `lfsr.c` file contains the core funcitoning of the LFSR and the `main.c` file utilises it in an example application by taking user input.  
- Clone this repository:  
  ```sh
  git clone https://github.com/noinoiexists/Fibonacci-LFSR-in-C.git && cd Fibonacci-LFSR-in-C/src/
  ```
- Compile the code:
  ```sh
  gcc lfsr.c main.c -o lfsr
  ```
- Run it using `./lfsr`

#### Example
```sh
Seed:
10110
Feedback coefficients:
11100
Length of bitstream required:
9
```
**Output**:  
`011010101`
