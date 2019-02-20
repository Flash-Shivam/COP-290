#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath> 
#include <ctime>

using namespace std;

// maximum size of matrix 
#define MAXSIZE  500
 int MAX ;
 int maxa;
 int maxb;
// maximum number of threads 
int MAX_THREAD; 

//to pass arguments to multi functions
float matA[MAXSIZE][MAXSIZE] = {0}; 
float matB[MAXSIZE][MAXSIZE] = {0}; 
float matC[MAXSIZE][MAXSIZE] = {0}; 
int step_i = 0; 


/*------Prototypes------*/

vector<vector<float> > Activation_relu(vector<vector<float> > v, int row, int col);
vector<vector<float> > Activation_tanh(vector<vector<float> > v, int row, int col);

vector<vector<float> > convolution(vector<vector<float> > a,vector<vector<float> > b,int n,int m);
vector<float> multiplication(vector<vector<float> > a,vector<vector<float> > b,int n,int m);
vector<float> mult_pthread(vector<vector<float> > a, vector<vector<float> > b,int n, int m);
vector<vector<float> > padding(vector<vector<float> > a,int size,int p);

vector<vector<float> > Pooling_max(vector<vector<float> > v   , int in_order, int out_order);
vector<vector<float> > Pooling_avg(vector<vector<float> > v   , int in_order, int out_order);\

vector<float> Probability_sigmoid(vector<float> x,int size);
vector<float> Probability_softmax(vector<float> x,int size);

/*-----END protoypes-----*/

/*------Main Functions---------*/

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
  ans = mult_pthread(padding(mat,matrix_size,padsize),ker,matrix_size+2*padsize,kernel_size);
  //ans = multiplication(padding(mat,matrix_size,padsize),ker,matrix_size+2*padsize,kernel_size);
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

/*------------END Helper Functions---------------*/


/*
----------Main Functions-----------
*/
int main(int argc, char** argv){
    //Start Clock
	int start = clock();

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
//Stop clock
	int stop = clock();
	int duration = stop-start;
	cout<<"Runtime of Program is "<<((float)duration/CLOCKS_PER_SEC)*1000<<"ms"<<endl;
      return 0;
    }

/**********END Main**********/


/*-------------------Activation Functions-------------------*/

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

/*------------------Convolution Functions-----------------*/

vector<vector<float> > convolution(vector<vector<float> > a,vector<vector<float> > b,int n,int m)
{
  if(n<=m){
    cout<<"ERROR !!!kernel size should be less than matrix size"<<endl;
    //Return but what?
  }


  vector<vector<float> > c;
  c.resize(n+m-1);
  for(int i=0;i<n+m-1;i++)
    c[i].resize(n+m-1);

  //float c[n+m-1][n+m-1];
  int i,j,x,y;


  for(i=0;i<n-m+1;i++)
  {
    for(j=0;j<n-m+1;j++)
    {
      c[i][j]=0;
      for(x=0;x<m;x++){
        for(y=0;y<m;y++){
          c[i][j]+=a[i+x][j+y]*b[x][y];
        }
      }
    //  cout<<c[i][j]<<"  ";
    }
  //  cout<<endl;
  }




  return c;
}


/*
	Multiplication Function computes convolution of matrix wrt to kernel using multithreaded computaion
*/

void* multi(void* arg) 
{ 
 
    int core = step_i++; 
  
    // Each thread computes 1/4th of matrix multiplication 
    for (int i = core ; i < (core + 1) ; i++)  
        for (int j = 0; j < maxb; j++)  
            for (int k = 0; k < maxa; k++)  
                matC[i][j] += matA[i][k] * matB[k][j]; 
} 


vector<float> mult_pthread(vector<vector<float> > a,vector<vector<float> > b,int n, int m){
	int i,j,k,p,q;
  	int t = n-m+1;
		

  	vector<vector<float> > c;
  	vector<float> d;

	if(m>=n){
		cout<<"ERROR!!!!"<<endl;
  		cout<<"kernel size should be smaller than matrix size";
		return d;
	}

  d.resize(m*m);
  c.resize(t*t);
  for(int i=0;i<t*t;i++)
    {
		c[i].resize(m*m);
    }

		for(j=0,i=0;j<m;j++)
		{
			for(k=0;k<m;k++)
			{
      				  d[i]=b[j][k];
     				   i++;
			}

	}

	for(i=0;i<t*t;i++)
	{
		for(j=0;j<m*m;j++)
		{
			p = j/m;
			q = i/t;
			c[i][j]=a[p+q][i%t+j-p*m];
		//	cout<<c[i][j]<<" ";

    		}
		//	cout<<endl;
	}

//vector<float> e;
  //e.resize(t*t);

//multiplication
	void* ptr;
	MAX = t*t;
	maxa = m*m;
	maxb = 1;
	MAX_THREAD = MAX;

	//Specfying matrix arguments
	for(int i=0;i<t*t;i++)
		for(int j=0;j<m*m;j++)
			matA[i][j]=c[i][j];	//t*t x m*m matrix

	for(int i=0;i<m*m;i++)
		matB[i][0]=d[i];		//m*m x 1 matrix


    // declaring t*t threads 

  	  pthread_t threads[MAX_THREAD]; 
  
    // Creating t*t threads, each evaluating its own part 
	for (int i = 0; i < MAX_THREAD; i++) {        
		pthread_create(&threads[i], NULL, multi, (void*)(ptr)); 
	} 
  
    // joining and waiting for all threads to complete 
	for (int i = 0; i < MAX_THREAD; i++)  
	        pthread_join(threads[i], NULL);     

 /* 
    // Displaying the result matrix 
    cout << endl 
         << "Multiplication of A and B" << endl; 
    for (int i = 0; i < MAX; i++) { 
        for (int j = 0; j < MAX; j++)  
            cout << mat[i][j] << " ";         
        cout << endl; 
    }
*/ 
	vector<float> ans;
	ans.resize(t*t);

	for(int i=0;i<t*t;i++)
		ans[i]=matC[i][0];

	return ans;


/*
  float s;

	for(i=0;i<t*t;i++)
	{ s=0;
		for(j=0;j<m*m;j++)
		{
			s+=c[i][j]*d[j];

		}
		e[i]=s;
	}

	return e;
*/
}


//Padding Function
vector<vector<float> > padding(vector<vector<float> > a,int size,int p)
{
  if(p==0){
    //Do not pad
    return a;
  }
	// p is the amount to be padded
  vector<vector<float> > b;
  b.resize(size+2*p);
  for(int i=0;i<size+2*p;i++)
    b[i].resize(size+2*p);

	//float b[size+2*p][size+2*p];
	int i,j;
  //initializing to zero

	for(i=0;i<size+2*p;i++)
		for(j=0;j<size+2*p;j++)
			b[i][j]=0;

	for(i=p;i<p+size;i++)
	{
		for(j=p;j<size+p;j++)
		{
			b[i][j]=a[i-p][j-p];

		}

	}
/*//to check padded matrix
  for(int i=0;i<size+2*p;i++){
    for(int j=0;j<size+2*p;j++)
      cout<<b[i][j]<<" ";
    cout<<endl;
  }
*/
	return b;
}

/*----------Pooling Functions---------*/


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

/*----------Probability Functions-------------*/


vector<float> Probability_sigmoid(vector<float> x,int size)
{

	int i;
	for(i=0;i<size;i++)
	{
		x[i]=1/(1+pow(2.718,-x[i]));
	}
	return x;
}

vector<float> Probability_softmax(vector<float> x,int size)
{

	int i;
	float y=0;
	for(i=0;i<size;i++)
	{
		y=y+pow(2.718,x[i]);
	}
	for(i=0;i<size;i++)
	{
		x[i]=pow(2.718,x[i])/y;
	}

	return x;
}
