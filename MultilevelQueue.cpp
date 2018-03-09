//	Shortest Job First
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>
#include <queue>
#include <vector>

using namespace std;

struct unit
{
	int timeleft;
	int endTime;
	int arrivalTime;
	int burstTime;
	int id;
	unit(){

	}
	unit(int t, int a, int b, int i){
		timeleft = t;
		arrivalTime = a;
		burstTime = b;
		id = i;
	}
};

string printBlank(int num){
	string str = "";
	for(int i=0;i<num;i++)
		str += " ";
	return str;
}

void sortArrival(int *arr, int totalNum){
	for(int i=0;i<totalNum;i++){
		int min = i, minNum = arr[1 * totalNum + i];
		for(int j=i;j<totalNum;j++){
			if(arr[1 * totalNum + j] < minNum){
				min = j;
				minNum = arr[1 * totalNum + j];
			}
		}
		//	get min
		swap(arr[0 * totalNum + i], arr[0 * totalNum + min]);
		swap(arr[1 * totalNum + i], arr[1 * totalNum + min]);
		swap(arr[2 * totalNum + i], arr[2 * totalNum + min]);
	}
}

void multiLevelQ(int *arr, int totalNum, int timeQ1, int timeQ2){
	stringstream ss;

	queue<unit> queueHigh, queueMid;
	vector<unit> queueLow, queueEnd;
	//sortArrival(arr, totalNum);

	unit newUnit;

	int alreadyIn[totalNum];
	for(int i=0;i<totalNum;i++) alreadyIn[i] = 0;
	int time = 0, count = 0, runningCount = 0, runningWho = -1, move = 0, minJob = -1;

	while(count < totalNum){
		for(int j=0;j<totalNum;j++){
			if(arr[1 * totalNum + j] <= time && alreadyIn[j] == 0){
				//	go in Queue
				alreadyIn[j] = 1;

				newUnit.timeleft = arr[2*totalNum + j];
				newUnit.arrivalTime = arr[1*totalNum + j];
				newUnit.burstTime = arr[2*totalNum + j];
				newUnit.id = arr[0*totalNum + j];
				queueHigh.push(newUnit);
			}
		}

		if(runningCount != 0){
			//	 someone is running
			if(runningCount == 1)
				move = 1;
			switch(runningWho){
				case 1:{
					runningCount--;
					queueHigh.front().timeleft--;
					if(queueHigh.front().timeleft == 0){
						runningCount = timeQ1;
						move = 0;
						queueHigh.front().endTime = time;
						queueEnd.push_back(queueHigh.front());
						queueHigh.pop();
						count ++;
					}
					break;
				}
				case 2:{
					runningCount--;
					queueMid.front().timeleft--;
					if(queueMid.front().timeleft == 0){
						runningCount = timeQ2;
						move = 0;
						queueMid.front().endTime = time;
						queueEnd.push_back(queueMid.front());
						queueMid.pop();
						count ++;
					}
					break;
				}
				case 3:{
					runningCount--;
					queueLow[minJob].timeleft--;
					move = 0;
					if(queueLow[minJob].timeleft == 0){
						queueLow[minJob].endTime = time;
						queueEnd.push_back(queueLow[minJob]);
						queueLow.erase(queueLow.begin() + minJob);
						count ++;
					}
					break;
				}
			}

		}
		else{
			//	no one is running
			if(move == 1){	//	have someone to move
				switch(runningWho){
					case 1:{
						move = 0;
						queueMid.push(queueHigh.front());
						queueHigh.pop();
						break;
					}
					case 2:{
						move = 0;
						queueLow.push_back(queueMid.front());
						queueMid.pop();
						break;
					}
				}
			}

			if(!queueHigh.empty()){
				//	run High Queue
				runningCount = timeQ1 - 1;
				runningWho = 1;
				queueHigh.front().timeleft--;
				if(queueHigh.front().timeleft == 0){
					runningCount = timeQ1;
					queueHigh.front().endTime = time;
					queueEnd.push_back(queueHigh.front());
					queueHigh.pop();
					count ++;
				}
			}
			else{
				if(!queueMid.empty()){
					//	run Mid Queue
					runningCount = timeQ2 - 1;
					runningWho = 2;
					queueMid.front().timeleft--;
					if(queueMid.front().timeleft == 0){
						runningCount = timeQ2;
						queueMid.front().endTime = time;
						queueEnd.push_back(queueMid.front());
						queueMid.pop();
						count ++;
					}
				}
				else{
					if(!queueLow.empty()){
						//	run Low Queue
						runningWho = 3;
						minJob = 0;
						int minBurst = queueLow[0].timeleft;
						for(int i=1;i<queueLow.size();i++){
							if(queueLow[i].timeleft < minBurst){
								minJob = i;
								minBurst = queueLow[i].timeleft;
							}
						}
						//	already get min Burst
						runningCount = queueLow[minJob].timeleft - 1;
						queueLow[minJob].timeleft--;
						if(queueLow.front().timeleft == 0){
							queueLow[minJob].endTime = time;
							queueEnd.push_back(queueLow[minJob]);
							queueLow.erase(queueLow.begin() + minJob);
							count ++;
						}
					}
					else{
						//	all queue empty, do nothing
					}
				}
			}	
		}

		if(runningCount > 0 && runningWho == 1){
			if(queueHigh.empty()){
				move = 0;
				runningCount = 0;
			}
		}
		if(runningCount > 0 && runningWho == 2){
			if(queueMid.empty()){
				move = 0;
				runningCount = 0;
			}
		}

		time++;
	}
	
	int waitingTime = 0, turnaroundTime = 0;
	string *ptrPrint = new string[totalNum];
	for(int i=0;i<totalNum;i++)
		ptrPrint[i] = "";

	for(int i=0;i<queueEnd.size();i++){
		
		string msg = "P[";
		ss << queueEnd[i].id + 1;
		msg += ss.str();
		ss.str("");
		msg += "]";
		ptrPrint[queueEnd[i].id] += msg;
		ptrPrint[queueEnd[i].id] += printBlank(15 - msg.size());
		
		//	******************************
		
		msg = "";
		waitingTime += queueEnd[i].endTime - queueEnd[i].arrivalTime + 1 - queueEnd[i].burstTime;
		ss << queueEnd[i].endTime - queueEnd[i].arrivalTime + 1 - queueEnd[i].burstTime;
		msg += ss.str();
		ss.str("");
		ptrPrint[queueEnd[i].id] += msg;
		ptrPrint[queueEnd[i].id] += printBlank(15 - msg.size());
		
		//	******************************
		
		msg = "";
		turnaroundTime += queueEnd[i].endTime - queueEnd[i].arrivalTime + 1;
		ss << queueEnd[i].endTime - queueEnd[i].arrivalTime + 1;
		msg += ss.str();
		ss.str("");
		ptrPrint[queueEnd[i].id] += msg;
		ptrPrint[queueEnd[i].id] += printBlank(15 - msg.size());
		ptrPrint[queueEnd[i].id] += "\n";

	}

	printf("Process        Waiting Time   Turnaround Time\n");	
	for(int i=0;i<totalNum;i++)
		cout << ptrPrint[i];

	printf("\nAverage waiting time: %f\n", ((double)waitingTime / totalNum));
	printf("Average turnaround time: %f\n", ((double)turnaroundTime / totalNum));

	delete [] ptrPrint;
}

int main(void)
{
	int totalNum;
	fstream file("./Q4.txt", ios::in);

	if(!file){
		cout << "File open error.....\n";
		exit(0);
	}
	else{
		file >> totalNum;	// get total number od process
		int *arr = new int[3 * totalNum];	// 0: process number, 1: arrival time, 2: burst time
		int timeQ1, timeQ2;	//	time quantent 1 & 2
		for(int i=0;i<totalNum;i++){
			arr[0 * totalNum + i] = i;
			file >> arr[1 * totalNum + i];
		}
		for(int i=0;i<totalNum;i++)
			file >> arr[2 * totalNum + i];
		file >> timeQ1 >> timeQ2;
		multiLevelQ(arr, totalNum, timeQ1, timeQ2);
		delete arr;
		file.close();
	}




	return 0;
}