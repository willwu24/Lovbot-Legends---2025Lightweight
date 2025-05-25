#include "jumpRemover.h"

jumpRemover::jumpRemover(int size){
	arrSize=size;
	for(int i=0; i<arrSize; i++){
		arr[i]=0;
	}
}
void jumpRemover::add(int value){
	for(int i=arrSize-2; i>=0; i--){
		arr[i+1]=arr[i];
	}
	arr[0]=value;
}
int jumpRemover::getMedian(){
	for(int i=0; i<arrSize; i++){
		arr1[i]=arr[i];
	}

	for(int i=0; i<arrSize-1; i++){
		int min =i;
		for(int j = i+1; j< arrSize; j++){
			if(arr1[j]<arr1[min]){
				min=j;
			}
		}
		int temp = arr1[i];
		arr1[i]=arr1[min];
		arr1[min]=temp;
	}


	medianValue=arr1[arrSize/2];
	return medianValue;
}
int jumpRemover::getValue(int index){
	return arr[index];
}
int jumpRemover::getValueSorted(int index){
	return arr1[index];
}

