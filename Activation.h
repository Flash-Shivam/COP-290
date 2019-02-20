#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
using namespace std;


vector<vector<float> > Activation_relu(vector<vector<float> > v, int row, int col){
  for(int i=0;i<row;i++)
    for(int j=0;j<col;j++)
    //RELU function
      if(v[i][j]<0)
        v[i][j]=0;
        
  return v;
}

vector<vector<float> > Activation_tanh(vector<vector<float> > v, int row, int col){
  for(int i=0;i<row;i++)
    for(int j=0;j<col;j++)
      //tanh function
      v[i][j]=tanh(v[i][j]);

  return v;
}
