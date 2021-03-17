/**
 * Tonya Shulkey
 * CECS 326 sec 08
 * Project 2
 * Due Date (3/14/21)
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Global variable array
int list[] = {7, 12, 19, 3, 18, 4, 6, 15, 8};
int sList[9];


void *sorter(void *params);	/* thread that performs basic sorting algorithm */
void *merger(void *params);	/* thread that performs merging of results */



int main (int argc, const char * argv[]) 
{
	
	//get size of list
	int size = sizeof(list)/sizeof(list[0]);
	
	//Print the unsorted array
	printf("Unsorted Array: \n");
	for(int i = 0; i < size; i ++){
		printf("%d ", list[i]);
	}
	printf("\n\n");
	
	
	pthread_t tid;
	
	//Create the threads	
	pthread_create(&tid, NULL, merger, sList);
	
	//wait on thread to finish
	pthread_join(tid, NULL);
	
	
	return 0;
}

/**
 * Sorting thread will sort the list given in the param.
 */
void *sorter(void *params)
{
	//Convert back to an integer arrray
	int *arr = (int*)params;
	
	//Get the size that was assigned to the first index
	int size = arr[0];

	
	//Used bubble sort to sort the list.
	for(int i = 1; i < size; i++){
		for(int j = 1; j < size - i; j++){
			if(arr[j] > arr[j + 1]){
				int temp = arr[j + 1];
				arr[j + 1] = arr[j];
				arr[j] = temp;
			}
		}
	}
	
	pthread_exit(NULL);
}

/**
 * Merge thread will merge the two sorted halves.
 */
void *merger(void *params)
{
	//Create the two threads ids for sorting
	pthread_t tidArr[2];
	
	
	//size of list
	int s = sizeof(list) / sizeof(list[0]);
	
	//Find the size for each half the threads will sort
	int size1 = (int)((sizeof(list) / sizeof(list[0])) / 2) + 1;
	int size2 = (sizeof(list) / sizeof(list[0])) - size1 + 2;
	
	
	
	//Create arrays for the two halves that will be sorted
	int larr[size1];
	int rarr[size2];
	
	//Assign the first index in the list to the size so that the sorter function can deal with the size easier.
	larr[0] = size1;
	rarr[0] = size2;
	
	
	//Get the values for both halves
	//Subtract 1 here because the list starts at 0 but the left thread starts at 1
	for(int i = 1; i < size1; i++){
		larr[i] = list[i - 1];
	}
	
	//Subtract sub from list index because size two is at a different index than where the index starts for the thread.
	//sub is based on whether it is an even list or not.
	int sub;
	if(s % 2 == 0){
		sub = 2;
	}
	else{
		sub = 3;
	}
	for(int i = 1; i < size2; i++){
		rarr[i] = list[size2 + i - sub];
	}
	
	
	//Create number of threads and sort them
	
	pthread_create(&tidArr[0], NULL, sorter, (void*)larr);
	pthread_create(&tidArr[1], NULL, sorter, (void*)rarr);

	
	//wait on threads to finish
	for(int i = 0; i < 2; i++){
		pthread_join(tidArr[i], NULL);
	}	
	
	
	//indexes
	int num = 0;
	int l = 1;
	int r = 1;
	//create the sorted list by taking the left and right thread's sorted list and finding the least
	while(l != size1 && r != size2){
		if(larr[l] < rarr[r]){
			sList[num] = larr[l];
			num++;
			l++;
		}
		else{
			sList[num] = rarr[r];
			num++;
			r++;
		}
	}
	
	
	//When one side left or right reaches its max index the left over will be inserted into the sorted list.
	
	while(l == size1 && r != size2){
		sList[num] = rarr[r];
		num++;
		r++;
	}
	while(l != size1 && r == size2){
		sList[num] = larr[l];
		num++;
		l++;
	}
	
	
	
	//Print the sorted array
	printf("Sorted Array: \n");
	for(int i = 0; i < s; i++){
		printf("%d ", sList[i]);
	}
	printf("\n");
}

