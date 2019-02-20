#include <iostream>
#include <cmath>
#include <vector>
using namespace std;

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


vector<float> multiplication(vector<vector<float> > a,vector<vector<float> > b,int n, int m){
	int i,j,k,p,q;
  int t = n-m+1;
	//cout<<"t="<<t<<endl;
  vector<vector<float> > c;
  vector<float> d;
if(m>=n){
  cout<<"kernel size should be smaller than matrix size";
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
  vector<float> e;
  e.resize(t*t);


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
}

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
