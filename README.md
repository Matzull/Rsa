# RSA Cracker

This is a simple program designed to factorize large numbers \( N \) using Fermat's factorization method. This technique can be quite effective for certain types of numbers but has its limitations.

## Overview

RSA Cracker utilizes Fermat's factorization algorithm to break down large numbers into their prime factors. It’s particularly useful for understanding the basics of number theory and cryptographic vulnerabilities. This method can efficiently handle numbers up to approximately 30 digits (around 100 bits) or numbers with unsecure prime factors (very similar p and q primes).

## Fermat's Factorization Method

Fermat's factorization algorithm works well when the number \( N \) can be expressed as the product of two close primes. The basic idea is to find such a pair of numbers where their difference is minimized. However, this method becomes less effective as the number of digits increases or if the prime factors are not close to each other.

## Limitations

- **Effective for Smaller \( N \)**: This implementation is most effective for numbers up to around 28 digits (~93 bits). For larger numbers or numbers with secure prime factors, Fermat's factorization may not be practical.
- **Unsecure Prime Factors**: The method performs better when \( N \) has relatively small and close prime factors. For numbers with larger or more secure prime factors, more sophisticated algorithms are required.

## Usage

To run the RSA Cracker, follow these steps:

1. **Clone the Repository:**

   ```bash
    git clone https://github.com/yourusername/rsa-cracker.git
    cd rsa-cracker
   ```

2. **Install Dependencies:**

   Make sure to install gmp (GNU BigNum library) before running the program. You can install it using the following command:

   ```bash
    sudo apt-get install libgmp-dev
   ```

3. **Run the Program:**

   Change the values NUM_THREADS to the number of threads you want to use and NUMBER_TO_FACTORIZE to the number you want to factorize:

    ```bash
     make run
    ```

## Reference

This program is based on the method described in as shown in computerphile's video [Breaking RSA encyption](https://www.youtube.com/watch?v=-ShwJqAalOk).


## Acknowledgements

Special thanks to the Computerphile team for their insightful videos that help explain complex topics in an accessible manner.

Feel free to reach out if you have any questions or need further assistance