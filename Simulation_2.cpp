 /*
	See how to get rid of going through sideways problem consider cases form both vehicles Truck and Bus
	need to give range for both
	give road a width of 3 bike will get width 1 and others 2
*/
#include <iostream>
#include <cmath>
#include <string>
#include <ctime>
#include <fstream>
using namespace std;
//#define RAND_MAX 1
#define maxposvehicles 25
#define maxtype 5
#define maxcolor 11
#define maxvalues 1000
class Road;
class Vehicle;
//Globals
// Car 0 Bike 1 Bus 2 Truck 3
char symbol[maxtype] = {'=','~',':','#','>'};
string colour[maxcolor] = {"Red","Blue","Green","Yellow","Orange","Purple","Violet","Pink","Black","White","Grey"};
int timestep = 1;	//seconds
int duration = 80;
int maxvehicles;

class Vehicle{
	public: bool active;	//Vehicle present or not
	//One time
	public: char type;	//b bike, C Car , T Truck, 
	public: int length =2;
	public: int width =2;
	public: int acceleration=1;
	public: int deceleration=1;
	public: int maxspeed=1;
	public: string color = "Red";
	public: int releasetime;
	//Road r; //Object of road on which it is
	//Dynamic
	public: int ypos=0;
	public: int speed=0;
	public: int pos;

	public: bool flag=0;


};

Vehicle vehicles[maxposvehicles];
class Road{
	public: int id;
	public: int width;
	public: int length;
	public: int lightstate;
	public: int lightpos;
	public: int lanes;
	public: int lanewidth;
		//Can later inlclude vehicle array here as to facilitate more roads
	public: int checkIfOccupied(int veh,int vpos,int vend, int vypos, int vleft){
		//cout<<"vpos :"<<vpos<<endl;
		//cout<<"vlan :"<<vypos<<endl;
		for(int i=0;i<maxvehicles;i++){	//need to check just rear half of vehicle
		//	cout<<"vehlcepos :"<<(vehicles[i].type)<<" "<<vehicles[i].pos<<endl;
		//	cout<<"vehlcelane :"<<(vehicles[i].type)<<" "<<vehicles[i].ypos<<endl;
	//cout<<(vehicles[i].ypos/*+vehicles[i].width/2*/ == vypos)<<"  <---Lane pos-->"<<((vehicles[i].pos-(vehicles[i].length) < vpos)&&vehicles[i].pos>=vpos)<<endl;
			if(veh==i)continue;
			bool front ((vehicles[i].pos-vehicles[i].length <= vpos)&&vehicles[i].pos>=vpos);
			bool end = ((vehicles[i].pos <=vpos)&&vehicles[i].pos>=vend);
			bool right ((vehicles[i].ypos-vehicles[i].width <= vypos)&&vehicles[i].ypos>=vypos);
			bool left = ((vehicles[i].ypos <=vypos)&&vehicles[i].ypos>=vleft);
	//cout<<"Vehicel"<<vehicles[i].type<<front<<left<<right<<endl;
			//	front = vpos>vhi
			if((front /*||end*/)&&(right||left))
				return i;	//return vehicle id
		}
		
			return -1;
	}

	public: void print(int vypos, int pos){
		bool flag=0;
		for(int i=0;i<maxvehicles;i++){
			if(((vehicles[i].pos-vehicles[i].length < pos)&&(vehicles[i].pos >= pos)) &&((vehicles[i].ypos-vehicles[i].width < vypos)&&(vehicles[i].ypos >= vypos))&&vehicles[i].active){
				cout<<vehicles[i].type;
				flag =1;
			}
		}
		if(!flag&&lightpos==pos)
			//cout<<vehicles[i].pos;
			cout<<"|";
		else if(!flag&&vypos%lanewidth==0)
			cout<<"_";
		else if(!flag)
			cout<<" ";
		
	}

	//move everything forward
	public:  void movefd(){
		bool flag ;

		int seed=clock();
		srand(seed);
		int random = rand();
		//cout<<random;
		if(random%20==0)
			flag =1;
		else
			flag =0;

		for(int i=0;i<maxvehicles;i++){
			if(!vehicles[i].active)	//When vehicles not spawned
				continue;

			if(lightstate == 0 && vehicles[i].pos >= lightpos-1){
				/*cout<<i<<" stop "<<endl;
				cout<<i<<" posL "<<vehicles[i].pos<<endl;
				cout<<i<<" speedL "<<vehicles[i].speed<<endl;
				cout<<i<<" laneL "<<vehicles[i].ypos<<endl;
				*/
				vehicles[i].speed =0;
				continue;
			}
			if(checkIfOccupied(i,vehicles[i].pos+vehicles[i].speed*timestep,vehicles[i].pos+vehicles[i].speed*timestep-vehicles[i].length,vehicles[i].ypos,vehicles[i].ypos-vehicles[i].width)>-1||flag){
		//	cout<<"hange lan"<<endl;
				changeLane(i,flag);
				if(vehicles[i].speed<vehicles[i].maxspeed)
					vehicles[i].speed+=vehicles[i].acceleration*timestep;
				/*
				cout<<i<<" posC "<<vehicles[i].pos<<endl;
				cout<<i<<" speedC "<<vehicles[i].speed<<endl;
				cout<<i<<" laneC "<<vehicles[i].ypos<<endl;
				*/
			}
			else {
		//cout<<"move ahea"<<endl;
				//cout<<"really"<<vehicles[i].pos<<endl;
				vehicles[i].pos+=vehicles[i].speed*timestep;
				if(vehicles[i].speed<vehicles[i].maxspeed)
					vehicles[i].speed+=vehicles[i].acceleration*timestep;
				/*cout<<timestep<<" "<<vehicles[i].acceleration<<endl;

				cout<<i<<" pos "<<vehicles[i].pos<<endl;
				cout<<i<<" speed "<<vehicles[i].speed<<endl;
				cout<<i<<" ypos "<<vehicles[i].ypos<<endl;
				*/
			}
		}
	}

	private:  void changeLane(int i,bool f){
		bool flag ;

		unsigned int seed = clock();
		srand(seed);
		int random = rand();
		if(random%2==0)
			flag =1;
		else
			flag =0;
		
		if(vehicles[i].flag ==0){
			if(checkIfOccupied(i,vehicles[i].pos,vehicles[i].pos-vehicles[i].length,vehicles[i].ypos+1,vehicles[i].ypos+1-vehicles[i].width/*remove 1+vehicles[i].width/2*/)==-1&&(vehicles[i].ypos+1/*vehicles[i].width/2*/<=width)&&flag){
				vehicles[i].ypos+=1;//vehicles[i].width/2;
			}
			else if(checkIfOccupied(i,vehicles[i].pos,vehicles[i].pos-vehicles[i].length,vehicles[i].ypos-1,vehicles[i].ypos+1-vehicles[i].width/*-vehicles[i].width/2*/)==-1&&(vehicles[i].ypos-1-vehicles[i].width/*vehicles[i].width/2*/ >0)&&flag){
			//	vehicles[i].flag =1;
				vehicles[i].ypos-=1;//vehicles[i].width/2;
			}
			else{
//				if(f =0)
//					return;
				vehicles[i].speed =0;
			}
		}

		if(vehicles[i].flag ==1){
			if(checkIfOccupied(i,vehicles[i].pos,vehicles[i].pos-vehicles[i].length,vehicles[i].ypos-1,vehicles[i].ypos+1-vehicles[i].width/*-vehicles[i].width/2*/)==-1&&(vehicles[i].ypos-1-vehicles[i].width/*vehicles[i].width/2*/ >0)&&flag){
				vehicles[i].ypos-=1;//vehicles[i].width/2;
			}
			else if(checkIfOccupied(i,vehicles[i].pos,vehicles[i].pos-vehicles[i].length,vehicles[i].ypos+1,vehicles[i].ypos+1-vehicles[i].width/*remove 1+vehicles[i].width/2*/)==-1&&(vehicles[i].ypos+1/*vehicles[i].width/2*/<=width)&&flag){
				vehicles[i].ypos+=1;//vehicles[i].width/2;
				vehicles[i].flag=0;
			}

			else{
				if(f =0)
					return;
				vehicles[i].speed =0;
			}
		}
	}
};
	

Road road;


int vehicletype (string name){
	if(name=="Car")
		return 0;
	else if(name =="Bike")
		return 1;
	else if(name =="Bus")
		return 2;
	else if(name =="Truck")
		return 3;
	else if(name =="Auto")
		return 4;
	else
		return 0;
}

int color(string name){
	if(name=="Red")
		return 0;
	else if(name =="Blue")
		return 1;
	else if(name =="Green")
		return 2;
	else if(name =="Yellow")
		return 3;
	else if(name =="Orange")
		return 4;
	else if(name =="Purple")
		return 5;
	else if(name =="Violet")
		return 6;
	else if(name =="Pink")
		return 7;
	else if(name =="Black")
		return 8;
	else if(name =="White")
		return 9;
	else if(name == "Grey")
		return 10;
	else
		return 0;
}

void configure(string name,int *arr){
  //input form file
	ifstream input(name);
    if(!input.is_open()){
      cout<<"No such file exists: "<<name<<endl;
    }
    int var=0,i=0;
    string line;
    while(getline(input,line,'=')){
	//cout<<" ->"<<line<<":"<<endl;
	if(line.compare("vehicle")==0){
		getline(input,line,' ');
	//cout<<line;
		arr[i]=vehicletype(line);
		i++;
		//Input colour
		getline(input,line,' ');
	//cout<<line;
		arr[i]=color(line);
		i++;
		for(int j=1;j<7;j++){
		input>> var ;
		arr[i]=var ;
		i++;
	//	cout<<"array clfghue "<<arr[i-1]<<endl;
		}
	}
	else{
       		input>> var ;
		arr[i]=var ;
		i++;
	//	cout<<"a"<<arr[i-1]<<endl;
	}
	getline(input,line,'\n');
    }
    input.close();
}

void addVehicle(Vehicle *v,int type,int color,int length , int width,int pos,int lane,int maxspeed,int releasetime){
	v->active = false;
	v->pos =pos;
	v->ypos = (lane-1)*(road.lanewidth)+width;
	v->releasetime =releasetime;
	v->maxspeed = maxspeed;
	v->speed = v->maxspeed;
	v->acceleration = 1;
	v->deceleration = 1;
	v->length = length;
	v->width = width;
	v->type = symbol[type];
	v->color= colour[color];
}

void spawnVehicles(int time){
	for(int i=0;i<maxvehicles;i++){
		if(vehicles[i].releasetime == time)
			vehicles[i].active =true;
	}
}

/*
	Method to Print state of the road at the given time instant
*/
void printState(int time){
	for(int i=1;i<=road.length;i++){
		if(i==road.lightpos)
			cout<<"|";
		else
		cout<<"~";
	}
	cout<<endl;
	//cout<<"bate dilo ki yaar ajawani aake tumhe samjhayen yara tu jaane na aaaa tu jaane naaaaa"<<endl<<road.lanes<<endl<<road.lanewidth<<endl;
	for(int i=1;i<=road.width;i++){
		//for(int k=0;k<road.lanewidth;k++){
		for(int j=1;j<=road.length;j++){
			road.print(i,j);
			//cout<<"hiaye"<<endl;
		}
		cout<<endl;
		//}
	}
	for(int i=1;i<=road.length;i++){
		if(i==road.lightpos)
			cout<<"|";
		else
		cout<<"~";
	}
	cout<<endl;
	cout<<"Time = " <<time<<endl;
}

/*
	Specifies interval between printing each step
*/
void delay ( int mseconds ) 
{ 
  clock_t endwait; 
  endwait = clock () + (mseconds*1.0/1000) * CLOCKS_PER_SEC ; 
  while (clock() < endwait) {} 
} 
int main(int argc, char** argv){
/*		string name;
		cin>>name;
		unsigned int seed = clock();
		srand(seed);
		int random = rand();
		while(name!="exit"){
		unsigned int seed = clock();
		srand(seed);
		int random = rand();
		if(name =="Rohan"||name =="Rohan Dahiya"||name =="Dahiya")
			cout<<"Padhle Thoda"<<endl;
		else
			cout<<name<<" marks "<<random%60<<endl;
		cin>>name;
		}
		return 0;*/
	int stoptime,gotime;
	int readData[maxvalues]={0};	//to store data read from config file
	if(argc<1){
		cout<<"Configuration file missing";
		return 0;
	}
	string filename = argv[1];
	//Read the configuration file and set all the values
		configure(filename,readData);

	 //Road start values
		road.id=readData[0];
		road.length = readData[1];
		road.width =readData[2];
		road.lightpos = readData[3];
		road.lanes = readData[4];
		road.lanewidth=road.width/road.lanes;
		maxvehicles = readData[5];
		timestep = readData[6];
		duration = readData[7];
		stoptime = readData[8];
		gotime = readData[9];
//can store all these values in respective arryas then in a loop initialize all
		for(int i =0;i<maxvehicles;i++)	addVehicle(&vehicles[i],readData[10+i*8],readData[11+i*8],readData[12+i*8],readData[13+i*8],readData[14+i*8],readData[15+i*8],readData[16+i*8],readData[17+i*8]);	//Car


	//Start simulation
	for(int time =0;time<duration;time+=timestep){

		spawnVehicles(time);
		road.movefd();	//move vehicles forward
		if(time >= stoptime&& time <= gotime)
			road.lightstate =0;
		else
			road.lightstate =1;
		//sortPos();	// to sort on basis of position
		 delay(200);
		printState(time);
	}

	return 0;
}
