#Prime Number Checker MultiThreads

Check if a number (up to unsigned long long) is prime with multi-threading using the pThread Library or in Sequential.

## Intro
It was asked to make a software that finds out if the given number is prime or not, and compare the Sequential vs Multithreading data.

## How to use
### Windows
#### Console in line: 
*exec.exe* **number_to_try** **number_of_threads**

### Console without inline:
*exec.exe*

The programme will ask you for **number_to_try** and **number_of_threads**

### Options

If **number_of_threads** is 1, it will run in sequential mode.

If **number_of_threads** is 0, it will try all threads possibilities by incrementing by 1. So it will try first in sequential, then 2 threads, then 3 threads, etc. until sqrt(**number_to_try**) threads.

##Algorithm

- First it tries if the **number_to_try** is less than 2.
- It tries if it’s 2
- It tries if it’s a modulo of 2
- It tries if it’s a modulo of 3
- Then it tries from here every odd numbers until sqrt(number_to_try)

## Cancelation Politic

If during the join a thread found out that **number_to_try** is prime, the program will be terminated. 

## Observation (results for the testing machine)

- Following the results of the graphs, it was noticed that they all follow a pattern, the best performance is between 90% et 95% of the sqrt(**number_to_try**) for the amount of threads. Meaning that we could optimize the algorithm to only check in this range to get the best performances.
- It’s interesting to use multithreading for very large numbers like 3203431780337 and above.

## Testing Machine
- MacBook Pro (Retina, 15 pouces, fin 2013)
- Version: 10.10.2 (14C81h)
- Memory: 16 Go 1600 MHz DDR3
- Processor: 2.3 GHz Intel Core i7

## License

Banana Editor Online by [Romain Claret](http://www.romainclaret.com) is licensed under a Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
