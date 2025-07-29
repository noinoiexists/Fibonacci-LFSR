# Fibonacci LFSR
This repository contains an implementation of the Fibonaci Linear Feedback Shift Register (LFSR) in C, along with a known-plaintext attack. This was created for educational cryptographic experimentation. 

## What is LFSR 
An LFSR is one of the many ways to create a pseudo-random number generator. It generates a stream of bits, from a linear function (here, XOR) of the previous state of bits.  
It consists of two core components:  
- **State**: a binary register of $`m`$ bits
- **Feedback Coefficients**: a binary mask of $`m`$ bits that determines which bits are selected for XOR.

## How an LFSR Works
An initial state (seed) of $`m`$ bits is set with values  $`s_{m-1},..., s_1, s_0`$  
A constant set of feedback coefficients is also set with values  $`p_{m-1},..., p_1, p_0`$  

The output bit of the LFSR $`s_m`$, which is also the input to the leftmost position on the register, can be computed by the XOR-sum of the products of the state bit and corresponding feedback coefficient:
```math
s_m \equiv s_{m-1}p_{m-1} +...+ s_{1}p_{1} + s_{0}p_{0}\;mod 2;\\e
```
In general:  
```math
s_{i+m} \equiv \sum_{j=0}^{m-1} p_j s_{i+j}\;mod 2;\;\;s_i, p_j \in {0,1};\;i=0,1,...
```
At each iteration, the register is shifted to the right and the previous output bit is made the leftmost bit.  
The least significant bit at each stage is the output to the bit stream.  

### Encrypting using LFSR
The LFSR can be used in the form of a stream cipher. The bit stream generated from an LFSR can be used to encrypt data as done in this implementation. Every 8 bits from the stream is grouped together and XORed with the corresponding byte from the input plaintext. The resultant is printed as hex to ensure portability.

## Known-Plaintext Attack
The fact that an LFSR uses a simple linear relation between the state bits and feedback coefficients can be exploited. If the register length $`m`$ is assumed to be known, and also the first $`2m`$ bits of the plaintext are known, this powerful attack can be used. In fact, even if $`m`$ isn't exactly known, using a modern computer to loop through multiple values to find $`m`$ is quick.  

Let the known plaintext bits be given by $`x_0, x_1,...,x_{2m-1}`$  
The corresonding ciphertext bits by $`y_0, y_1,...,y_{2m-1}`$  
The first $`2m`$ stream bits can be reconstructed by:  
```math
s_i \equiv x_i + y_i \;mod2;\;\;  i = 0, 1,...,2m-1
```
We also know that,
```math
s_m \equiv s_{m-1}p_{m-1} +...+ s_{1}p_{1} + s_{0}p_{0}\;mod 2;\\e
```
Similarly, the equations for $`s_{m+1}`$ to $`s_{2m-1}`$ can be written. This gives $`m`$ linear equations in $`m`$ variables $`p_0, p_1,...,p_{m-1}`$.  
These linear equations can be easily solved using a [Gaussian elimination](https://en.wikipedia.org/wiki/Gaussian_elimination) algorithm for [Galois Field of 2](https://en.wikipedia.org/wiki/GF(2)).  
The feedback coefficients are now known. The initial state(seed) is also known, which is just the first $`m`$ reconstructed bits. The LFSR can now be clocked with these values, which gives the exact same bit stream used for encryption.  
Since XOR is symmetric, the ciphertext bytes can be XORed with this stream and that results in the complete plaintext. 

## Usage
You can try out this cipher as well as the attack using this repository. Follow these steps:  
- Clone the repository and move in:
  ```sh
  git clone https://github.com/noinoiexists/Fibonacci-LFSR.git && cd Fibonacci-LFSR
  ```
- Compile the code:
  ```sh
  gcc src/lfsr.c src/cipher.c -o cipher -Wall -Wextra ; gcc src/lfsr.c src/gf2-gaussian-elimination.c attacks/known-plaintext-attack.c -o attack -Wall -Wextra
  ```
- Use the tools with `./cipher` and `./attack`

#### Example
**Encryption**
```
$ ./cipher
Seed:
101110
Feedback coefficients:
111101
Plaintext:
Go placidly amid the noise and the haste, and remember what peace there may be in silence.
Ciphertext (hex):
31ec3289b66d288e0c5d56bdc1a9d712a36691bf2c258801424abdc1aada56f77a9cfa642a941c5403bdc1aada56f17794bf6129821a1158f5c1b09e06e6739abf2c3f8f0d434abdcda5c756e177d9b3626b94015d4af3c3a190
```

**Attack** 
```
$ ./attack
Ciphertext (hex):
31ec3289b66d288e0c5d56bdc1a9d712a36691bf2c258801424abdc1aada56f77a9cfa642a941c5403bdc1aada56f17794bf6129821a1158f5c1b09e06e6739abf2c3f8f0d434abdcda5c756e177d9b3626b94015d4af3c3a190
Known Plaintext:
Go
Found Seed length:
6
Found feedback coefficients:
111101
Found plaintext:
Go placidly amid the noise and the haste, and remember what peace there may be in silence.
```
As demonstrated above, the full plaintext could be cracked from the encrypted text just by knowing the first word "Go".

## Licence
This implementation was created by me, [noinoiexists](https://github.com/noinoiexists) and is protected under the MIT Licence. You can read more about it in the [LICENCE](https://github.com/noinoiexists/Fibonacci-LFSR-in-C/blob/main/LICENSE) file.
