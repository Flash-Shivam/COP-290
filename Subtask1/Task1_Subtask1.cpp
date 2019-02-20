#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Convolution.h"
#include "Activation.h"
#include "Pooling.h"
#include "Probability.h"


vector<float> parseVector(string name,int size){
  vector<float> v;
  string line;
  v.resize(size);
  //input form file
    ifstream input;
    input.open(name);

  for(int i=0;i<size;i++){
    input>>v[i];
  }

  input.close();
  return v;
}

vector<vector<float> > parseMatrix(string name, int size){
  vector<vector<float> > v;
  v.resize(size);
  for(int i=0;i<size;i++){
    v[i].resize(size);
  }
  //input form file
    ifstream input(name);
    if(!input.is_open()){
      cout<<"No such file exists: "<<name<<endl;
    }
    float var=0.0;
    for(int i=0;i<size;i++){
      for(int j=0;j<size;j++){
        input>> var ;
        v[j][i]=var ;
      }
    }
    input.close();
  return v;
}

vector<vector<float> > parseMatrix(string name, int row,int col){
  vector<vector<float> > v;
  v.resize(row);
  for(int i=0;i<row;i++){
    v[i].resize(col);
  }
  //input form file
    ifstream input(name);
    if(!input.is_open()){
      cout<<"No such file exists: "<<name<<endl;
    }
    float var=0.0;
    for(int i=0;i<col;i++){
      for(int j=0;j<row;j++){
        input>> var ;
        v[j][i]=var ;
      }
    }
    input.close();
  return v;
}

//to print matrix
void printMatrix(vector<vector<float> > v , int row,int col){
  for(int i=0;i<row;i++){
    for(int j=0;j<col;j++)
      cout<<v[i][j]<<"  ";
    cout<<endl;
  }
}
void printMatrix(vector<vector<float> > v , int size){
  for(int i=0;i<size;i++){
    for(int j=0;j<size;j++)
      cout<<v[i][j]<<"  ";
    cout<<endl;
  }
}
// float *parseMatrix(char *name, int size){
//   float mat[size][size];
//   //input form file
//     ifstream input;
//     input.open(name);
//
//   for(int i=0;i<size;i++)
//     for(int j=0;j<size;j++)
//       input>>mat[j][i]; //reverse i j to get column major form
// }

void conv(string matrix , string kernel, int matrix_size, int kernel_size,int padsize){
  if(kernel_size>=matrix_size){
    cout<<"kernel size should be less than matrix size"<<endl;
    return;
  }
  int outsize= matrix_size+2*padsize-kernel_size+1;
  vector<vector<float> > mat;
  vector<vector<float> > ker;
  vector<vector<float> > ans; // confirm size ask to do vector
  mat.resize(matrix_size);
  ans.resize(outsize);//##check output size
  ker.resize(kernel_size);

  for(int i=0;i<outsize;i++){
    ans[i].resize(outsize);
  }
  for(int i=0;i<matrix_size;i++){
    mat[i].resize(matrix_size);
  }
  for(int i=0;i<kernel_size;i++)
    ker[i].resize(kernel_size);

  mat = parseMatrix(matrix,matrix_size);
  ker = parseMatrix(kernel,kernel_size);

  ans = convolution(padding(mat,matrix_size,padsize),ker,matrix_size+2*padsize,kernel_size);

  //printing matrix
  printMatrix(ans,outsize);
}

void mult(string matrix , string kernel, int matrix_size, int kernel_size,int padsize){
  if(matrix_size<=kernel_size){
    cout<<"Kernel size should be smaller than matrix size";
    return;
  }
  int outsize= matrix_size+2*padsize-kernel_size+1;
  vector<vector<float> > mat;
  vector<vector<float> > ker;
  vector<float> ans; // confirm size ask to do vector
	
  mat.resize(matrix_size);
  ans.resize(outsize*outsize);
  ker.resize(kernel_size);
  for(int i=0;i<matrix_size;i++){
    mat[i].resize(matrix_size);
  }
  for(int i=0;i<kernel_size;i++)
    ker[i].resize(kernel_size);
    mat = parseMatrix(matrix,matrix_size);

   
  ker = parseMatrix(kernel,kernel_size);

	
	
  ans = multiplication(padding(mat,matrix_size,padsize),ker,matrix_size+2*padsize,kernel_size);
  //printing vector in matrix form
  for(int i=0;i<outsize;i++){
    for(int j=0;j<outsize;j++)
      cout<<ans[outsize*i+j]<<" ";
    cout<<endl;
  }


}
//args matix name matrix size type of activation function
void acti(string matrix, int row,int col, string func){
  vector<vector<float> > mat ;

  mat.resize(row); //assiming all matrices to be square
  for(int i=0;i<row;i++)
    mat[i].resize(col);

  mat = parseMatrix(matrix,row,col);

  if(func.compare("rel")==0)
    mat = Activation_relu(mat,row,col);
  else if(func.compare ("tan")==0)
    mat = Activation_tanh(mat,row,col);
  else{
    cout<<"invalid activation function :"<<func<<"   use rel or tan"<<endl;
    return;
  }
  //printing matrix
  printMatrix(mat,row,col);

}
//args matix name matrix size type of activation function
void pool(string matrix, int in_size,int out_size, string func){

  vector<vector<float> > mat ;
  mat.resize(in_size); //assiming all matrices to be square
  for(int i=0;i<in_size;i++)
    mat[i].resize(in_size);


  mat = parseMatrix(matrix,in_size);

  if(func.compare("max")==0)
    mat = Pooling_max(mat,in_size,out_size);
  else if(func.compare("avg")==0)
    mat = Pooling_avg(mat,in_size,out_size);
  else{
      cout<<"invalid pooling function :"<<func<<"   use max or avg"<<endl;
      return;
    }
    //printing matrix
    printMatrix(mat,out_size);

}

void prob(string vec, int size, string func){
  vector<float> v = parseVector(vec,size);

  if(func.compare("sof")==0)
    v = Probability_softmax(v,size);
  else if(func.compare("sig")==0)
    v = Probability_sigmoid(v,size);
  else{
    cout<<"Invalid Probability function :"<<func<<" use sof or sig"<<endl;
    return;
  }

  //printing vector
  for(int i=0;i<size;i++)
    cout<<v[i]<<endl;
}


int main(int argc, char** argv){

    //Defining Formats
    string formats[5];
    formats[0]="FORMAT:: co padsize matrix1.txt size kernel.txt size\n";
    formats[1]="FORMAT:: mu padsize matrix1.txt size kernel.txt size\n";
    formats[2]="FORMAT:: ac activation_function(rel/tan) matrix.txt numRows numColumns\n";
    formats[3]="FORMAT:: po  pooling_function(max/avg) matrix.txt in_size out_size\n";
    formats[4]="FORMAT:: pr function(sof/sig) matrix.txt size\n";

    //In case no function specified
    if(argc < 2){
      cout<<"NO Function Specified"<<endl;
      cout<<"Available functions :-"<<endl<<endl;
      cout<<" \t 1. co: convolutes given matrix(give matrix name and size) without( give padsize = 0) or with padding(specify integer pad size) using given kernel(give matrix name and size).\n\t\t";
      cout<<formats[0];
      cout<<" \t 2. mu: convolutes matrix with kernel(name and size) with (give pad size) or without (pad size = 0) padding using multiplication method.\n\t\t";
      cout<<formats[1];
      cout<<" \t 3. ac: Computes non linear activation of each element of given matrix (name,numRows,numCols) using tanh(tan) or RElu(rel) as activation function.\n\t\t";
      cout<<formats[2];
      cout<<" \t 4. po: Subsamples given matrix(name size) using MaxPooling(max) or AvgPooling(avg) functions to a given output matrix size.\n\t\t";
      cout<<formats[3];
      cout<<" \t 5. pr: Computes probabilities from vector of random floats using Softmax(sof) or Sigmoid(sig) functions.\n\t\t";
      cout<<formats[4];
      return 0;
    }

    int padsize=0;
    int size1,size2;
    string func = argv[1];
    string name1,name2;

    //Function Specified
      if(func.compare("co")==0){
                    if(argc < 7){
                    cout<<"Missing arguments, see format"<<endl;
                    cout<<formats[0];
                    return 0;
                  }

                  padsize = stoi(argv[2]);
                  size1 = stoi(argv[4]);
                  size2 = stoi(argv[6]);
                  name1 = argv[3];
                  name2 = argv[5];

                  conv(name1, name2,size1,size2,padsize);
      }
      else if(func.compare("mu")==0){
                  if(argc < 7){
                    cout<<"Missing arguments, see format"<<endl;
                    cout<<formats[1];
                    return 0;
                  }
                  padsize = stoi(argv[2]);
                  size1 = stoi(argv[4]);
                  size2 = stoi(argv[6]);
                  name1 = argv[3];
                  name2 = argv[5];

                  mult(name1,name2,size1,size2,padsize);
      }
      else if(func.compare("ac")==0){
                  if(argc < 6){
                    cout<<"Missing arguments, see format"<<endl;
                    cout<<formats[2];
                    return 0;
                  }

                  size1 = stoi(argv[4]);
                  size2 = stoi (argv[5]);
                  name1 = argv[3];
                  name2 = argv[2];
                  acti(name1, size1,size2, name2);
      }
      else if(func.compare("po")==0){
                  if(argc < 6){
                    cout<<"Missing arguments, see format"<<endl;
                    cout<<formats[3];
                    return 0;
                  }

                  size1 = stoi(argv[4]);
                  size2 = stoi(argv[5]);
                  name1 = argv[3];
                  name2 = argv[2];
                  pool(name1, size1,size2, name2);
      }
      else if(func.compare("pr")==0){
                  if(argc < 5){
                    cout<<"Missing arguments, see format"<<endl;
                    cout<<formats[4];
                    return 0;
                  }

                  size1 = stoi(argv[4]);
                  name1 = argv[3];
                  name2 = argv[2];
                  prob(name1,size1,name2);
      }
      else{
                cout<<"Invalid command given, see available commands and formats"<<endl;
                cout<<"Available functions :-"<<endl<<endl;
                cout<<" \t 1. co: convolutes given matrix(give matrix name and size) without( give padsize = 0) or with padding(specify integer pad size) using given kernel(give matrix name and size).\n\t\t";
                cout<<formats[0];
                cout<<" \t 2. mu: Multiplies two given matrx(name and size) with (give pad size) or without (pad size = 0) padding.\n\t\t";
                cout<<formats[1];
                cout<<" \t 3. ac: Computes non linear activation of each element of given matrix (name and size) using tanh(tan) or RElu(rel) as activation function.\n\t\t";
                cout<<formats[2];
                cout<<" \t 4. po: Subsamples given matrix(name size) using MaxPooling(max) or AvgPooling(avg) functions to a given output matrix size.\n\t\t";
                cout<<formats[3];
                cout<<" \t 5. pr: Computes probabilities from vector of random floats using Softmax(sof) or Sigmoid(sig) functions.\n\t\t";
                cout<<formats[4];
      }

      return 0;
    }
