#include <iostream>
#include <cmath>
#include <string>
#include <ctime>
#include <fstream>
using namespace std;
#include <GL/freeglut.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <math.h>
//#define RAND_MAX 1
#define maxposvehicles 25
#define maxtype 5
#define maxcolor 11
#define maxvalues 1000
int variable = 0;
class Road;
class Vehicle;
float x[14][1000], y[14][1000];
//Globals
// Car 0 Bike 1 Bus 2 Truck 3
char symbol[maxtype] = { '=','~',':','#','>' };
string colour[maxcolor] = { "Red","Blue","Green","Yellow","Orange","Purple","Violet","Pink","Black","White","Grey" };
int timestep = 1;	//seconds
int duration = 80;
int maxvehicles;

class Vehicle {
public: bool active;	//Vehicle present or not
//One time
public: char type;	//b bike, C Car , T Truck, 
public: int length = 2;
public: int width = 2;
public: int acceleration = 1;
public: int deceleration = 1;
public: int maxspeed = 1;
public: string color = "Red";
public: int releasetime;
		//Road r; //Object of road on which it is
		//Dynamic
public: int ypos = 0;
public: int speed = 0;
public: int pos;

public: bool flag = 0;


};

Vehicle vehicles[maxposvehicles];
class Road {
public: int id;
public: int width;
public: int length;
public: int lightstate;
public: int lightpos;
public: int lanes;
public: int lanewidth;
		//Can later inlclude vehicle array here as to facilitate more roads
public: int checkIfOccupied(int veh, int vpos, int vend, int vypos, int vleft) {
	//cout<<"vpos :"<<vpos<<endl;
	//cout<<"vlan :"<<vypos<<endl;
	for (int i = 0; i < maxvehicles; i++) {	//need to check just rear half of vehicle
	//	cout<<"vehlcepos :"<<(vehicles[i].type)<<" "<<vehicles[i].pos<<endl;
	//	cout<<"vehlcelane :"<<(vehicles[i].type)<<" "<<vehicles[i].ypos<<endl;
//cout<<(vehicles[i].ypos/*+vehicles[i].width/2*/ == vypos)<<"  <---Lane pos-->"<<((vehicles[i].pos-(vehicles[i].length) < vpos)&&vehicles[i].pos>=vpos)<<endl;
		if (veh == i)continue;
		bool front((vehicles[i].pos - vehicles[i].length <= vpos) && vehicles[i].pos >= vpos);
		bool end = ((vehicles[i].pos <= vpos) && vehicles[i].pos >= vend);
		bool right((vehicles[i].ypos - vehicles[i].width <= vypos) && vehicles[i].ypos >= vypos);
		bool left = ((vehicles[i].ypos <= vypos) && vehicles[i].ypos >= vleft);
		//cout<<"Vehicel"<<vehicles[i].type<<front<<left<<right<<endl;
				//	front = vpos>vhi
		if ((front /*||end*/) && (right || left))
			return i;	//return vehicle id
	}

	return -1;
}

public: void print(int vypos, int pos) {
	bool flag = 0;
	for (int i = 0; i < maxvehicles; i++) {
		if (((vehicles[i].pos - vehicles[i].length < pos) && (vehicles[i].pos >= pos)) && ((vehicles[i].ypos - vehicles[i].width < vypos) && (vehicles[i].ypos >= vypos)) && vehicles[i].active) {
			cout << vehicles[i].type;
			flag = 1;
		}
	}
	if (!flag&&lightpos == pos)
		//cout<<vehicles[i].pos;
		cout << "|";
	else if (!flag&&vypos%lanewidth == 0)
		cout << "_";
	else if (!flag)
		cout << " ";

}

		//move everything forward
public:  void movefd() {
	bool flag;

	int seed = clock();
	srand(seed);
	int random = rand();
	//cout<<random;
	if (random % 20 == 0)
		flag = 1;
	else
		flag = 0;

	for (int i = 0; i < maxvehicles; i++) {
		if (!vehicles[i].active)	//When vehicles not spawned
			continue;

		if (lightstate == 0 && vehicles[i].pos >= lightpos - 1) {
			/*cout<<i<<" stop "<<endl;
			cout<<i<<" posL "<<vehicles[i].pos<<endl;
			cout<<i<<" speedL "<<vehicles[i].speed<<endl;
			cout<<i<<" laneL "<<vehicles[i].ypos<<endl;
			*/
			vehicles[i].speed = 0;
			continue;
		}
		if (checkIfOccupied(i, vehicles[i].pos + vehicles[i].speed*timestep, vehicles[i].pos + vehicles[i].speed*timestep - vehicles[i].length, vehicles[i].ypos, vehicles[i].ypos - vehicles[i].width) > -1 || flag) {
			//	cout<<"hange lan"<<endl;
			changeLane(i, flag);
			if (vehicles[i].speed < vehicles[i].maxspeed)
				vehicles[i].speed += vehicles[i].acceleration*timestep;
			/*
			cout<<i<<" posC "<<vehicles[i].pos<<endl;
			cout<<i<<" speedC "<<vehicles[i].speed<<endl;
			cout<<i<<" laneC "<<vehicles[i].ypos<<endl;
			*/
		}
		else {
			//cout<<"move ahea"<<endl;
					//cout<<"really"<<vehicles[i].pos<<endl;
			vehicles[i].pos += vehicles[i].speed*timestep;
			if (vehicles[i].speed < vehicles[i].maxspeed)
				vehicles[i].speed += vehicles[i].acceleration*timestep;
			/*cout<<timestep<<" "<<vehicles[i].acceleration<<endl;

			cout<<i<<" pos "<<vehicles[i].pos<<endl;
			cout<<i<<" speed "<<vehicles[i].speed<<endl;
			cout<<i<<" ypos "<<vehicles[i].ypos<<endl;
			*/
		}
	}
}

private:  void changeLane(int i, bool f) {
	bool flag;

	unsigned int seed = clock();
	srand(seed);
	int random = rand();
	if (random % 2 == 0)
		flag = 1;
	else
		flag = 0;

	if (vehicles[i].flag == 0) {
		if (checkIfOccupied(i, vehicles[i].pos, vehicles[i].pos - vehicles[i].length, vehicles[i].ypos + 1, vehicles[i].ypos + 1 - vehicles[i].width/*remove 1+vehicles[i].width/2*/) == -1 && (vehicles[i].ypos + 1/*vehicles[i].width/2*/ <= width) && flag) {
			vehicles[i].ypos += 1;//vehicles[i].width/2;
		}
		else if (checkIfOccupied(i, vehicles[i].pos, vehicles[i].pos - vehicles[i].length, vehicles[i].ypos - 1, vehicles[i].ypos + 1 - vehicles[i].width/*-vehicles[i].width/2*/) == -1 && (vehicles[i].ypos - 1 - vehicles[i].width/*vehicles[i].width/2*/ > 0) && flag) {
			//	vehicles[i].flag =1;
			vehicles[i].ypos -= 1;//vehicles[i].width/2;
		}
		else {
			//				if(f =0)
			//					return;
			vehicles[i].speed = 0;
		}
	}

	if (vehicles[i].flag == 1) {
		if (checkIfOccupied(i, vehicles[i].pos, vehicles[i].pos - vehicles[i].length, vehicles[i].ypos - 1, vehicles[i].ypos + 1 - vehicles[i].width/*-vehicles[i].width/2*/) == -1 && (vehicles[i].ypos - 1 - vehicles[i].width/*vehicles[i].width/2*/ > 0) && flag) {
			vehicles[i].ypos -= 1;//vehicles[i].width/2;
		}
		else if (checkIfOccupied(i, vehicles[i].pos, vehicles[i].pos - vehicles[i].length, vehicles[i].ypos + 1, vehicles[i].ypos + 1 - vehicles[i].width/*remove 1+vehicles[i].width/2*/) == -1 && (vehicles[i].ypos + 1/*vehicles[i].width/2*/ <= width) && flag) {
			vehicles[i].ypos += 1;//vehicles[i].width/2;
			vehicles[i].flag = 0;
		}

		else {
			if (f = 0)
				return;
			vehicles[i].speed = 0;
		}
	}
}
};


Road road;


int vehicletype(string name) {
	if (name == "Car")
		return 0;
	else if (name == "Bike")
		return 1;
	else if (name == "Bus")
		return 2;
	else if (name == "Truck")
		return 3;
	else if (name == "Auto")
		return 4;
	else
		return 0;
}

int color(string name) {
	if (name == "Red")
		return 0;
	else if (name == "Blue")
		return 1;
	else if (name == "Green")
		return 2;
	else if (name == "Yellow")
		return 3;
	else if (name == "Orange")
		return 4;
	else if (name == "Purple")
		return 5;
	else if (name == "Violet")
		return 6;
	else if (name == "Pink")
		return 7;
	else if (name == "Black")
		return 8;
	else if (name == "White")
		return 9;
	else if (name == "Grey")
		return 10;
	else
		return 0;
}
/*
void configure(string name, int *arr) {
	//input form file
	ifstream input(name);
	if (!input.is_open()) {
		cout << "No such file exists: " << name << endl;
	}
	int var = 0, i = 0;
	string line;
	while (getline(input, line, '=')) {
		//cout<<" ->"<<line<<":"<<endl;
		if (line.compare("vehicle") == 0) {
			getline(input, line, ' ');
			//cout<<line;
			arr[i] = vehicletype(line);
			i++;
			//Input colour
			getline(input, line, ' ');
			//cout<<line;
			arr[i] = color(line);
			i++;
			for (int j = 1; j < 7; j++) {
				input >> var;
				arr[i] = var;
				i++;
				//	cout<<"array clfghue "<<arr[i-1]<<endl;
			}
		}
		else {
			input >> var;
			arr[i] = var;
			i++;
			//	cout<<"a"<<arr[i-1]<<endl;
		}
		getline(input, line, '\n');
	}
	input.close();
}
*/
void addVehicle(Vehicle *v, int type, int color, int length, int width, int pos, int lane, int maxspeed, int releasetime) {
	v->active = false;
	v->pos = pos;
	v->ypos = (lane - 1)*(road.lanewidth) + width;
	v->releasetime = releasetime;
	v->maxspeed = maxspeed;
	v->speed = v->maxspeed;
	v->acceleration = 1;
	v->deceleration = 1;
	v->length = length;
	v->width = width;
	v->type = symbol[type];
	v->color = colour[color];
}

void spawnVehicles(int time) {
	for (int i = 0; i < maxvehicles; i++) {
		if (vehicles[i].releasetime == time)
			vehicles[i].active = true;
	}
}

/*
	Method to Print state of the road at the given time instant
*/
void printState(int time) {
	for (int i = 1; i <= road.length; i++) {
		if (i == road.lightpos)
			cout << "|";
		else
			cout << "~";
	}
	cout << endl;
	//cout<<"bate dilo ki yaar ajawani aake tumhe samjhayen yara tu jaane na aaaa tu jaane naaaaa"<<endl<<road.lanes<<endl<<road.lanewidth<<endl;
	for (int i = 1; i <= road.width; i++) {
		//for(int k=0;k<road.lanewidth;k++){
		for (int j = 1; j <= road.length; j++) {
			road.print(i, j);
			//cout<<"hiaye"<<endl;
		}
		cout << endl;
		//}
	}
	for (int i = 1; i <= road.length; i++) {
		if (i == road.lightpos)
			cout << "|";
		else
			cout << "~";
	}
	cout << endl;
	cout << "Time = " << time << endl;
}

/*
	Specifies interval between printing each step
*/
void delay(int mseconds)
{
	clock_t endwait;
	endwait = clock() + (mseconds*1.0 / 1000) * CLOCKS_PER_SEC;
	while (clock() < endwait) {}
}


GLint b = 300;
float  counter[100];
int z = 0;
float s = -100;
void initOpenGl()
{
	glClearColor(0.5, 0.5, 0.5, 1); //Background Color
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 700, 0, 700);
	glMatrixMode(GL_MODELVIEW);
	
}

void wheel(int x, int y,int z)
{
	float th;
	glBegin(GL_POLYGON);
	if (z == 0) {
		glColor3f(1, 0, 0);
	}
	else {
		glColor3f(0, 1, 0);
	}
	
	for (int i = 0; i < 360; i++)
	{
		th = i * (3.1416 / 180);
		glVertex2f(x + 20* cos(th), y + 20 * sin(th));
	}

	glEnd();

}

void test(int x,int y,int z,int w) {
	
	glRectf(x, y, z, w);

}


void signal()
{
	glBegin(GL_LINES);
	glVertex2f(variable, 0);
	glVertex2f(variable, 1000);
	glEnd();
}
void car( int p,int z)
{


	//Bottom Part
	/*for (int i = 0; i < road.length; i++) {
		if (i != road.lightpos)
			counter[p] = counter[p] + (vehicles[p].speed)*0.0005; 
	}*/
	

	glLoadIdentity();

	
	
	signal();
	
			/*cout<<i<<" stop "<<endl;
			cout<<i<<" posL "<<vehicles[i].pos<<endl;
			cout<<i<<" speedL "<<vehicles[i].speed<<endl;
			cout<<i<<" laneL "<<vehicles[i].ypos<<endl;
			*/
	
			/*
			cout<<i<<" posC "<<vehicles[i].pos<<endl;
			cout<<i<<" speedC "<<vehicles[i].speed<<endl;
			cout<<i<<" laneC "<<vehicles[i].ypos<<endl;
			*/
			//--------------------------
			
	
//	s = s + 0.009;
//	z = z + 0.5;
		//---------------
	wheel(variable, 700,z);

	//glTranslated(counter[p], 0, 0.0);
	//glTranslated(s, 0, 0.0);
	glEnable(GL_COLOR_MATERIAL);
	for (int i = 0; i < 14; i++) {
		
		glColor3b(i,i*10,i*30);
		test(x[i][p]- vehicles[i].length  , y[i][p],  x[i][p], vehicles[i].width + y[i][p]);
	}
	
	
	/*//glScaled(0.1,0.1,0.0);
	glBegin(GL_POLYGON);
	glVertex2f(100, 100);
	glVertex2f(100, 160);
	glVertex2f(450, 160);
	glVertex2f(450, 100);

	//Top Part
	glBegin(GL_POLYGON);
	glVertex2f(150, 160);
	glVertex2f(200, 200);
	glVertex2f(400, 200);
	glVertex2f(450, 160);

	glEnd();*/

/*	for (int k = 1; k <= road.lanes; k++) {
		for (int j = 1; j <= road.length; j++) {
			for (int i = 0; i < maxvehicles; i++) {
				if (((vehicles[i].pos - vehicles[i].length < j) && (vehicles[i].pos >= j)) && (vehicles[i].ypos == k) && vehicles[i].active) {
					test(10 * vehicles[p].pos, 20 * vehicles[p].ypos, 20 * vehicles[p].length + 10 * vehicles[p].pos, 20 * vehicles[p].ypos + 20 * vehicles[p].width);

				}
			}
		}
		
	}*/
	/*for (int i = 1; i <= road.lanes; i++) {
		for (int j = 1; j <= road.length; j++) {
			road.prin(i, j, p, road);
		}

	}*/


	/*for (int i = 0; i < maxvehicles; i++) {
		if (((vehicles[i].pos - vehicles[i].length < pos) && (vehicles[i].pos >= pos)) && (vehicles[i].ypos == vypos) && vehicles[i].active) {
			test(10 * vehicles[p].pos, 20 * vehicles[p].ypos, 20 * vehicles[p].length + 10 * vehicles[p].pos, 20 * vehicles[p].ypos + 20 * vehicles[p].width);
			
		}
	}*/

	//test(10*vehicles[p].pos, 20*vehicles[p].ypos, 20*vehicles[p].length + 10*vehicles[p].pos, 20*vehicles[p].ypos + 20*vehicles[p].width);
	
	/*glBegin(GL_POINTS);
	for (int j = 0; j < 100*vehicles[p].length; j++)
	{
		glVertex2f(vehicles[p].pos, vehicles[p].ypos +j);
	}
	glEnd();*/

}

void display(int x,int z)
{
	glColor3f(0.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);
	//Push and pop matrix for separating circle object from Background
	
	car(x,z);
//	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
	glFlush();


}



int main(int argc, char** argv) {
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
	int stoptime, gotime;
	float readData[maxvalues] = { 0 };	//to store data read from config file
	readData[0] = 1;
	readData[1] = 500;
	readData[2] = 75;
	readData[3] = 210;
	variable = readData[3];
	readData[4] = 15;
	readData[5] = 14;
	readData[6] = 1;
	readData[7] = 80;
	readData[8] = 0;
	readData[9] = 35;
	readData[10] = 0;
	readData[11] = 1;
	readData[12] = 20;
	readData[13] = 20;
	readData[14] = 0;
	readData[15] = 1;
	readData[16] = 20;
	readData[17] = 0;
	readData[18] = 2;
	readData[19] = 1;
	readData[20] = 30;
	readData[21] = 20;
	readData[22] = 0;
	readData[23] = 1;
	readData[24] = 10;
	readData[25] = 4;
	readData[26] = 1;
	readData[27] = 0;
	readData[28] = 20;
	readData[29] = 10;
	readData[30] = 15;
	readData[31] = 1;
	readData[32] = 20;
	readData[33] = 4;
	readData[10] = 0;
	readData[11] = 1;
	readData[12] = 20;
	readData[13] = 20;
	readData[14] = 0;
	readData[15] = 1;
	readData[16] = 20;
	readData[17] = 0;

	readData[18] = 0;
	readData[19] = 1;
	readData[20] = 20;
	readData[21] = 20;
	readData[22] = 0;
	readData[23] = 1;
	readData[24] = 20;
	readData[25] = 0;
	readData[26] = 0;
	readData[27] = 1;
	readData[28] = 20;
	readData[29] = 20;
	readData[30] = 0;
	readData[31] = 1;
	readData[32] = 20;
	readData[33] = 0;

	readData[34] = 0;
	readData[35] = 1;
	readData[36] = 20;
	readData[37] = 20;
	readData[38] = 0;
	readData[39] = 7;
	readData[40] = 20;
	readData[41] = 0;

	readData[42] = 0;
	readData[43] = 1;
	readData[44] = 20;
	readData[45] = 20;
	readData[46] = 0;
	readData[47] = 3;
	readData[48] = 20;
	readData[49] = 0;

	readData[50] = 0;
	readData[51] = 1;
	readData[52] = 20;
	readData[53] = 20;
	readData[54] = 0;
	readData[55] = 2;
	readData[56] = 20;
	readData[57] = 0;

	readData[58] = 0;
	readData[59] = 1;
	readData[60] = 20;
	readData[61] = 20;
	readData[62] = 0;
	readData[63] = 4;
	readData[64] = 20;
	readData[65] = 0;

	readData[66] = 0;
	readData[67] = 1;
	readData[68] = 20;
	readData[69] = 20;
	readData[70] = 0;
	readData[71] = 5;
	readData[72] = 20;
	readData[73] = 0;

	readData[74] = 0;
	readData[75] = 1;
	readData[76] = 20;
	readData[77] = 20;
	readData[78] = 0;
	readData[79] = 14;
	readData[80] = 20;
	readData[81] = 0;

	readData[82] = 0;
	readData[83] = 1;
	readData[84] = 20;
	readData[85] = 20;
	readData[86] = 0;
	readData[87] = 11;
	readData[88] = 20;
	readData[89] = 0;

	readData[90] = 0;
	readData[91] = 1;
	readData[92] = 20;
	readData[93] = 20;
	readData[94] = 0;
	readData[95] = 10;
	readData[96] = 20;
	readData[97] = 0;

	readData[98] = 0;
	readData[99] = 1;
	readData[100]= 20;
	readData[101] = 20;
	readData[102] = 0;
	readData[103] = 10;
	readData[104] = 20;
	readData[105] = 0;

	readData[106] = 0;
	readData[107] = 1;
	readData[108] = 20;
	readData[109] = 20;
	readData[110] = 0;
	readData[111] = 13;
	readData[112] = 20;
	readData[113] = 0;

	readData[114] = 0;
	readData[115] = 1;
	readData[116] = 20;
	readData[117] = 20;
	readData[118] = 0;
	readData[119] = 15;
	readData[120] = 20;
	readData[121] = 0;

	readData[122] = 0;
	readData[123] = 1;
	readData[124] = 20;
	readData[125] = 20;
	readData[126] = 0;
	readData[127] = 9;
	readData[128] = 100;
	readData[129] = 0;



	// Road start values
		road.id = readData[0];
	road.length = readData[1];
	road.width = readData[2];
	road.lightpos = readData[3];
	road.lanes = readData[4];
	road.lanewidth = road.width / road.lanes;
	maxvehicles = readData[5];
	timestep = readData[6];
	duration = readData[7];
	stoptime = readData[8];
	gotime = readData[9];
	//can store all these values in respective arryas then in a loop initialize all
	for (int i = 0; i < maxvehicles; i++)	addVehicle(&vehicles[i], readData[10 + i * 8], readData[11 + i * 8], readData[12 + i * 8], readData[13 + i * 8], readData[14 + i * 8], readData[15 + i * 8], readData[16 + i * 8], readData[17 + i * 8]);	//Car

	for (int i = 0; i < 100; i++)
	{
		counter[i] = -100.0;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1000, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Car Moving");
	initOpenGl();
	//display(0);

//Start simulation
	for (int time = 0; time < duration; time += timestep) {

		spawnVehicles(time);
		road.movefd();	//move vehicles forward
		if (time >= stoptime && time <= gotime)
		{
			road.lightstate = 0;
		}
		else {
			road.lightstate = 1;
			
		}
			
		//sortPos();	// to sort on basis of position
		delay(0);
		printState(time);
		for (int p = 0; p < 14; p++) {
			x[p][time] = vehicles[p].pos;
			y[p][time] = vehicles[p].ypos ;
			
		}
		
		display(time,road.lightstate);

			//glutLeaveMainLoop();
	}

	
	
	//glutMainLoop();

	return 0;
}