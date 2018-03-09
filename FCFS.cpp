//	First Come First Served
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>

using namespace std;

void printBlank(int num){
	for(int i=0;i<num;i++)
		cout << " ";
}

void FCFS(int *burstTime, int totalNum){
	stringstream ss;

	int size = 15;
	int time = 0;
	int waitingTime = 0, turnaroundTime = 0;

	printf("Process        Waiting Time   Turnaround Time\n");	
	for(int i=0;i<totalNum;i++){
		string msg = "P[";
		ss << i+1;
		msg += ss.str();
		ss.str("");
		msg += "]";
		cout << msg;
		printBlank(15 - msg.size());

		//	******************************

		msg = "";
		ss << time;
		msg += ss.str();
		ss.str("");
		cout << msg;
		printBlank(15 - msg.size());
		waitingTime += time;
		time += burstTime[i];

		//	******************************

		msg = "";
		ss << time;
		msg += ss.str();
		ss.str("");
		cout << msg;
		printBlank(15 - msg.size());
		cout << endl;
		turnaroundTime += time;
	}

	printf("\nAverage waiting time: %f\n", ((double)waitingTime / totalNum));
	printf("Average turnaround time: %f\n", ((double)turnaroundTime / totalNum));
}


int main(void)
{
	int totalNum;
	fstream file("./Q1.txt", ios::in);

	if(!file){
		cout << "File open error.....\n";
		exit(0);
	}
	else{
		file >> totalNum;	// get total number od process
		int *burstTime = new int[totalNum];
		for(int i=0;i<totalNum;i++)
			file >> burstTime[i];

		FCFS(burstTime, totalNum);
		delete [] burstTime;
		file.close();
	}

	return 0;
}