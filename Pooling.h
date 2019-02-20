#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
using namespace std;

//returns input matrix if pooling not possible
vector<vector<float> > Pooling_max(vector<vector<float> > v   , int in_order, int out_order){
  if(in_order%out_order!=0){
    cout<<"Desired output order is not possible";
    return v;
  }

    vector<vector<float> > out_matrix;
   out_matrix.resize(out_order);
  for(int i=0;i<out_order;i++){
     out_matrix[i].resize(out_order);
  }

  int stride = in_order/out_order;
    float max;
  //looping all out matrix positons
  for(int i=0,I=0;i<in_order;i+=stride){
    for(int j=0,J=0;j<in_order;j+=stride){

      max=v[i][j];

      //looping a stride to find max
      for(int ii=0;ii<stride;ii++)
        for(int jj=0;jj<stride;jj++) //as 00 value already present in out_matrix
          if(max < v[i+ii][j+jj])
            max=v[i+ii][j+jj];
      out_matrix[I][J]=max;
      J++;
    }
    I++;
  }
    return out_matrix;
}

vector<vector<float> > Pooling_avg(vector<vector<float>> v   , int in_order, int out_order){
  if(in_order%out_order!=0){
    cout<<"Desired output order is not possible as not a multiple\n";
    return v;
  }

  vector<vector<float> > out_matrix;
   out_matrix.resize(out_order);
  for(int i=0;i<out_order;i++){
     out_matrix[i].resize(out_order);
  }

  int stride = in_order/out_order;
  float avg=0;
  //looping all out matrix positons
  for(int i=0,I=0;i<in_order;i+=stride){
    for(int j=0,J=0;j<in_order;j+=stride){



      //looping a stride to find max
        for(int ii=0;ii<stride;ii++)
        for(int jj=0;jj<stride;jj++)
            avg+=v[i+ii][j+jj];  //summing all values

      out_matrix[I][J] = avg/(stride*stride);
      J++;
    }
    I++;
  }

  return out_matrix;
}

//main function to test above function
// int main(){
//   vector<vector<float> > v;
//   int m,n;  //size of array m*m: size of out array n*n
//   cin>>m>>n;
//   v.resize(m);
//   for(int i=0;i<m;i++){
//     v[i].resize(m);
//   }
//   //take input
//
//   //input form file
//     ifstream input;
//     input.open("filename.txt");
//
//   for(int i=0;i<m;i++)
//     for(int j=0;j<m;j++)
//       //form console
//       //cin>>v[i][j];
//       //form file
//       input>>v[i][j];
//
//   //call activation function
//   v = MaxPooling(v,m,n);
//
//   //print
//   for(int i=0;i<n;i++){
//     for(int j=0;j<n;j++)
//       cout<<v[i][j]<<"  ";
//     cout<<endl;
//   }
//}
