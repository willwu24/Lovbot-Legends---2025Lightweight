
#define maxValue 100
class jumpRemover{
private: 
	int arrSize;
	int arr[maxValue];
	int medianValue;
	int arr1[maxValue];

public:
	jumpRemover(int size);
	void add(int value);
	int getMedian();
	int getValue(int index);
	int getValueSorted(int index);
};