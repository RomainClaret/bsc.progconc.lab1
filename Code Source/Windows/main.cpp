//
//  main.cpp
//  Labo1 - Programmation Concurente
//
//  Created by Roman Yakovenko on 07.10.14.
//  Copyright (c) 2014 Rocla Matrix. All rights reserved.
//

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <windows.h> // for timing

#include "pThread.h"

//If turned On, extra details are displayed
bool DEVmode = false;

//Global values initialisation
unsigned long long nombre = 101111111111111111;
int n_threads = 0;
int counter = 0;

//Prototypes
bool isPrime(unsigned long long, int);
bool isPrimeSingle(unsigned long long);
bool isPrimeMultithreads(unsigned long long, int);
void *checkPrimeThread(void *t);
void recursiveThreadTest(unsigned long long);

//Structure to share data with threads
struct thread_data{
	int thread_id;
	unsigned long long number;
	int modulo;
	bool isPrime;
};

int main(int argc, char* argv[])
{
	//We recieved only 1 argument from the terminal
	if (argc == 2) {
		nombre = strtoull(argv[1], nullptr, 10);
		std::cout << nombre << std::endl;
		std::cout << "Entrez le nombre de threads" << std::endl;
		std::cin >> n_threads;
		//We recieved both arguments, number and threads
	}
	else if (argc == 3){
		nombre = strtoull(argv[1], nullptr, 10);
		n_threads = atoi(argv[2]);
	}
	else{
		std::cout << "Entrez le nombre a tester" << std::endl;
		//18446744073709551615
		std::cout << "number <= 18'446'744'073'709'551'615" << std::endl;
		std::cin >> nombre;
		std::cout << "Entrez le nombre de threads" << std::endl;
		std::cin >> n_threads;
	}

	//If number of threads is 0, we are going to test all
	if (n_threads == 0) {
		recursiveThreadTest(nombre);
	}
	else{
		LARGE_INTEGER Frequency;			// Processor ticks per second.
		LARGE_INTEGER StartTimer, EndTimer;	// Number of ticks at a certain time.
		double ElapsedTime;
		// Get number of ticks per second.
		QueryPerformanceFrequency(&Frequency);
		// Start high-precision timer.
		QueryPerformanceCounter(&StartTimer);

		//Main function to dispatch the work depending on threads asked
		if (isPrime(nombre, n_threads)){
			std::cout << "is prime" << std::endl;
		}
		else{
			std::cout << "is not prime" << std::endl;
		}

		// Stop the high precision timer.
		QueryPerformanceCounter(&EndTimer);

		ElapsedTime = (EndTimer.QuadPart - StartTimer.QuadPart) * 1000.0 / Frequency.QuadPart;
		printf("\nElapsed time: %2.4fms.\n", ElapsedTime);

		return 0;
	}
}

//Sequential Version of isPrime
bool isPrimeSingle(unsigned long long number){
	unsigned long long squart_root = sqrt(number);

	//std::cout << squart_root <<std::endl;

	if (number < 2) return false;
	else if (number == 2){
		return true;
	}
	else if (int(number % 2) == 0){
		return false;
	}
	else if (int(number % 3) == 0){
		return false;
	}
	//Modulo for only odd numbers
	for (int i = 3; i <= squart_root; i += 2){
		if (long(number % i) == 0){
			return false;
		}
	}

	return true;
}

//Multithreads Version of isPrime
bool isPrimeMultithreads(unsigned long long number, int n_threads){
	int thread;
	int i;
	int j = 3;
	int thread_running = 0;
	pthread_t* threads = new pthread_t[n_threads];
	pthread_attr_t attr;
	void *status;
	struct thread_data* td = new struct thread_data[n_threads];

	// Initialize and set thread joinable
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	//Count the max number of threads to test
	for (int i = j; i*i <= sqrt(number); i += 2){
		counter++;
	}

	if (DEVmode) {
		std::cout << counter << std::endl;
	}

	while (counter > 0) {
		for (i = 0; i < n_threads; i++){
			counter--;

			if (counter >= 0) {
				if (DEVmode) {
					std::cout << "main() : creating thread, " << i << std::endl;
				}

				//Initialization of the threads structures to pass
				td[i].thread_id = i;
				td[i].modulo = j;
				td[i].number = number;
				td[i].isPrime = true;

				//std::cout << "thread id: " << i << " modulo: " << j << std::endl;

				j += 2;   //Modulo is only odd number
				thread_running++;

				thread = pthread_create(&threads[i], NULL, checkPrimeThread, (void *)&td[i]);
				if (thread){
					std::cout << "Error:unable to create thread," << thread << std::endl;
					exit(-1);
				}
			}
		}

		if (thread_running > 0) {
			//free attribute and wait for the other threads
			pthread_attr_destroy(&attr);
			for (i = 0; i < thread_running; i++){
				thread = pthread_join(threads[i], &status);
				if (thread){
					std::cout << "Error:unable to join," << thread << std::endl;
					exit(-1);
				}
				if (DEVmode) {
					std::cout << "Main: completed thread id :" << i;
					std::cout << "  exiting with status :" << status << std::endl;
				}
				if (td[i].isPrime == false){
					return false;
				}
			}
		}
		thread_running = 0;
	}

	if (DEVmode) {
		std::cout << "Main: program exiting." << std::endl;
	}
	delete threads;
	delete td;
	threads = nullptr;
	td = nullptr;
	return true;
}

void *checkPrimeThread(void *t)
{
	unsigned long long tid;
	tid = (unsigned long long)t;

	struct thread_data *thread_data;

	thread_data = (struct thread_data *) t;

	if (thread_data->number < 2){
		thread_data->isPrime = false;
	}
	else if (thread_data->number == 2){
		thread_data->isPrime = true;
	}
	else if (int(int(thread_data->number % 2)) == 0){
		thread_data->isPrime = false;
	}
	else if (int(int(thread_data->number % 3)) == 0){
		thread_data->isPrime = false;
	}
	if (int(thread_data->number % thread_data->modulo) == 0){
		thread_data->isPrime = false;
	}

	pthread_exit(NULL);
	return NULL;
}

bool isPrime(unsigned long long number, int n_threads){
	if (n_threads <= 1) {
		return isPrimeSingle(number);
	}
	else if (sqrt(number) < 9){
		return isPrimeSingle(number);
	}
	else{
		return isPrimeMultithreads(number, n_threads);
	}
	return false;
}

//Test with a number recursivly threads
void recursiveThreadTest(unsigned long long number){
	int threadUsed = 1;

	for (int i = 3; (i*i) <= sqrt(number); i += 2){
		LARGE_INTEGER Frequency;			// Processor ticks per second.
		LARGE_INTEGER StartTimer, EndTimer;	// Number of ticks at a certain time.
		double ElapsedTime;
		// Get number of ticks per second.
		QueryPerformanceFrequency(&Frequency);
		// Start high-precision timer.
		QueryPerformanceCounter(&StartTimer);

		if (isPrime(number, threadUsed)){
			//std::cout<<"is prime"<<std::endl;
		}
		else{
			//std::cout<<"is not prime"<<std::endl;
		}

		// Stop the high precision timer.
		QueryPerformanceCounter(&EndTimer);

		ElapsedTime = (EndTimer.QuadPart - StartTimer.QuadPart) * 1000.0 / Frequency.QuadPart;
		printf("\nElapsed time: %2.4fms.\n", ElapsedTime);;

		threadUsed++;
	}
}
//993319
//25234833583
//3203431780337
//101111111111111111