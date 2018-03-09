//	Shortest Job First
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>

using namespace std;

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

void SJF(int *arr, int totalNum){
	stringstream ss;

	int count = 0;
	int size = 15;
	int time = 0;
	int waitingTime = 0, turnaroundTime = 0;

	string *ptrPrint = new string[totalNum];
	for(int i=0;i<totalNum;i++)
		ptrPrint[i] = "";

	//sortArrival(arr, totalNum);

	int *originalBurst = new int[totalNum];
	for(int i=0;i<totalNum;i++)
		originalBurst[i] = arr[2 * totalNum + i];

	printf("Process        Waiting Time   Turnaround Time\n");	

	int running = 0;
	int minBurst = 9999, min = -1;
	while(count<totalNum){
		//	select who to run
		if(running == 0){
			minBurst = 9999;
			min = -1;
			for(int j=0;j<totalNum;j++){
				if(arr[1 * totalNum + j] <= time && arr[2 * totalNum + j] < minBurst && arr[2 * totalNum + j] != 0){
					//	change minBurst
					min = j;
					minBurst = arr[2 * totalNum + j];
					running = 1;
				}
			}	
		}
		
		//	run
		arr[2 * totalNum + min]--;

		//	check if done
		if(arr[2 * totalNum + min] == 0){
			running = 0;
			count++;

			// done
			string msg = "P[";
			ss << arr[0 * totalNum + min] + 1;
			msg += ss.str();
			ss.str("");
			msg += "]";
			ptrPrint[arr[0 * totalNum + min]] += msg;
			ptrPrint[arr[0 * totalNum + min]] += printBlank(15 - msg.size());

			//	******************************

			msg = "";
			waitingTime += time + 1 - arr[1 * totalNum + min] - originalBurst[min];
			ss << time + 1 - arr[1 * totalNum + min] - originalBurst[min];
			msg += ss.str();
			ss.str("");
			ptrPrint[arr[0 * totalNum] + min] += msg;
			ptrPrint[arr[0 * totalNum] + min] += printBlank(15 - msg.size());

			//	******************************

			msg = "";
			turnaroundTime += time + 1 - arr[1 * totalNum + min];
			ss << time + 1 - arr[1 * totalNum + min];
			msg += ss.str();
			ss.str("");
			ptrPrint[arr[0 * totalNum] + min] += msg;
			ptrPrint[arr[0 * totalNum] + min] += printBlank(15 - msg.size());
			ptrPrint[arr[0 * totalNum] + min] += "\n";
		}

		time++;
	}

	for(int i=0;i<totalNum;i++)
		cout << ptrPrint[i];

	printf("\nAverage waiting time: %f\n", ((double)waitingTime / totalNum));
	printf("Average turnaround time: %f\n", ((double)turnaroundTime / totalNum));

	delete [] ptrPrint;
	delete [] originalBurst;
}


int main(void)
{
	int totalNum;
	fstream file("./Q2.txt", ios::in);

	if(!file){
		cout << "File open error.....\n";
		exit(0);
	}
	else{
		file >> totalNum;	// get total number od process
		int *arr = new int[3 * totalNum];	// 0: process number, 1: arrival time, 2: burst time
		for(int i=0;i<totalNum;i++){
			arr[0 * totalNum + i] = i;
			file >> arr[1 * totalNum + i];
		}
		for(int i=0;i<totalNum;i++)
			file >> arr[2 * totalNum + i];

		SJF(arr, totalNum);
		delete arr;
		file.close();
	}




	return 0;
}