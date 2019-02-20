#include <iostream>
#include <cmath>
#include <vector>
#define e 2.718
using namespace std;

vector<float> Probability_sigmoid(vector<float> x,int size)
{

	int i;
	for(i=0;i<size;i++)
	{
		x[i]=1/(1+pow(e,-x[i]));
	}
	return x;
}

vector<float> Probability_softmax(vector<float> x,int size)
{

	int i;
	float y=0;
	for(i=0;i<size;i++)
	{
		y=y+pow(e,x[i]);
	}
	for(i=0;i<size;i++)
	{
		x[i]=pow(e,x[i])/y;
	}

	return x;
}
