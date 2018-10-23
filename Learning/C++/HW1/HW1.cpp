/*******************************
* Coursera C++ HW1
* Author: Lars Folkerts
* Goals: Convert C program to C++
* Instructions:
* 	change to C++ io
* 	change to one line comments
* 	change defines of constants to const
* 	change array to vector<>
* 	inline any short function
* Program Purpose: To sum a vector
*********************************/
#include <iostream>
#include <vector>
using namespace std;

const int N = 40;// size of the array

template <class T>
void inline sum(int &v_sum, int size, const vector<T> arr);

int main()
{
	int accum = 0;
	vector<int> data(N);
	for(int i = 0; i < N; ++i)
	{
		data[i] = i;
	}
	sum(accum, N, data);
	cout << "sum is " << accum << endl;
	return 0;
}

/*********************************
* Function Sum
* Purpose: Sums an array
* Arguments: 
*	int sum (result) 
*	int size (of array)
*	const int arr (array to be summed)
* Returns: void
* Important: Inline
* TODO: Why not return the sum result instead of pass by reference?
* TODO: There is a better way to implement this in C++ using vectors, but not yet taught in class
**********************************/
template <class T>
void inline sum(int &v_sum, int size, const vector<T> arr)
{ 
	for(int i = 0; i < size; ++i)
	{
		v_sum = v_sum + arr[i];
	}
}
