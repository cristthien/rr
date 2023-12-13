
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct {
	int iPID;
	int iArrival, iBurst;
	int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;

#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3

void inputProcess(int n, PCB P[]) {
	for (int i = 0; i < n; i++) {
		printf("Enter information for process %d:\n", i + 1);
		P[i].iPID = i;

		printf("  Arrival time: ");
		scanf_s("%d", &P[i].iArrival);

		printf("  Burst time: ");
		scanf_s("%d", &P[i].iBurst);
	}
}
void inputProcessAuto(int n, PCB P[]) {
	for (int i = 0; i < n; i++) {
		P[i].iPID = i;

		P[i].iArrival = rand() % 21;
		P[i].iBurst = rand() % 11 + 2;

	}

}

void printProcess(int n, PCB P[]) {
	printf("\nPID\tArrival\tBurst\n");
	for (int i = 0; i < n; i++) {
		printf("%d\t%d\t%d\n",
			P[i].iPID, P[i].iArrival, P[i].iBurst);
	}
}


int swapProcess(PCB* P, PCB* Q) {
	PCB temp = *P;
	*P = *Q;
	*Q = temp;

	return 0;
}

int compareProcesses(PCB* P, PCB* Q, int iCriteria) {
	switch (iCriteria) {
	case SORT_BY_ARRIVAL:
		return P->iArrival - Q->iArrival;
	case SORT_BY_PID:
		return P->iPID - Q->iPID;
	case SORT_BY_BURST:
		return P->iBurst - Q->iBurst;
	case SORT_BY_START:
		return P->iStart - Q->iStart;
	default:
		return 0;  // Default case, should not occur
	}
}



void pushProcess(int* n, PCB P[], PCB Q);
void removeProcess(int* n, int index, PCB P[]);

int partition(PCB P[], int low, int high, int iCriteria);
void quickSort(PCB P[], int low, int high, int iCriteria);



void calculateAWT(int n, PCB P[]) {
	float totalWaitingTime = 0;

	for (int i = 0; i < n; i++)
	{
		totalWaitingTime += P[i].iWaiting;
	}

	float averageWaitingTime = totalWaitingTime / n;
	printf("Average Waiting Time: %.2f\n", averageWaitingTime);
}

void calculateATaT(int n, PCB P[]) {
	float totalTurnaroundTime = 0;

	for (int i = 0; i < n; i++) {
		totalTurnaroundTime += P[i].iTaT;
	}

	float averageTurnaroundTime = totalTurnaroundTime / n;
	printf("Average Turnaround Time: %.2f\n", averageTurnaroundTime);
}
void bubbleSort(PCB P[], int n, int iCriteria) {
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			if (compareProcesses(&P[j], &P[j + 1], iCriteria) > 0) {
				swapProcess(&P[j], &P[j + 1]);
			}
		}
	}
}
void pushProcessGant(int* n, PCB P[], PCB Q);


void printProcessFull(int n, PCB P[]) {
	printf("\nPID\tArrival\tBurst\tStart\tFinish\tWaiting\tResponse\tTurnaround\n");
	for (int i = 0; i < n; i++) {
		printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
			P[i].iPID, P[i].iArrival, P[i].iBurst,
			P[i].iStart, P[i].iFinish, P[i].iWaiting, P[i].iResponse, P[i].iTaT);
	}
}
void exportGanttChart(int n, PCB P[]) {
	// In tiêu đề của Gantt Chartprintf("P%d   |", P[i].iPID);
	printf("Gantt Chart:\n");
	if (P[0].iStart > 0) {
		printf("------");
	
	}
	for (int i = 0; i < n; i++) {
		printf("------");
	}
	printf("\n");

	printf("|");
	if (P[0].iStart > 0) {
		printf("idle  |");

	}
	for (int i = 0; i < n; i++)
	{
		
		if (P[i].iPID == -1) {
			printf("idle  |");

		}
		else {
			printf("P%d   |", P[i].iPID);
		}

	}

	printf("\n");
	if (P[0].iStart > 0) {
		printf("------");

	}
	for (int i = 0; i < n; i++) {
		printf("------");
	}
	printf("\n");

	// In thời gian tại mỗi thời điểm

	if (P[0].iStart > 0) {
		printf("%d     ", 0);

	}

	printf("%d     ", P[0].iStart);
	for (int i = 0; i < n; i++) {
		printf("%d    ", P[i].iFinish);
	}
	printf("\n");
}


int main()
{
	srand(time(NULL));

	PCB Input[10];
	PCB ReadyQueue[10];
	PCB TerminatedArray[10];
	PCB GanttArray[100];

	int iNumberOfProcess;
	printf("Please input number of Process: ");
	scanf_s("%d", &iNumberOfProcess);

	int iQuantumTime;
	printf("Please input quantum time: ");
	scanf_s("%d", &iQuantumTime);

	int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0;
	int iGantt = 0;
	inputProcessAuto(iNumberOfProcess, Input);
	bubbleSort(Input, iRemain, SORT_BY_ARRIVAL);
	printf("\nInput Queue: ");
	printProcess(iRemain, Input);


	pushProcess(&iReady, ReadyQueue, Input[0]);
	removeProcess(&iRemain, 0, Input);

	ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
	ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
	ReadyQueue[0].iTaT = ReadyQueue[0].iBurst;

	if (ReadyQueue[0].iBurst <= iQuantumTime) {
		ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
	}
	else {
		ReadyQueue[0].iFinish = ReadyQueue[0].iStart + iQuantumTime;
	}

	printf("\nReady Queue: ");
	printProcess(1, ReadyQueue);
	printf("\n------- ");




	while (iTerminated < iNumberOfProcess) {
		while (iRemain > 0)
		{
			if (Input[0].iArrival <= ReadyQueue[0].iFinish )
			{
				pushProcess(&iReady, ReadyQueue, Input[0]);
				removeProcess(&iRemain, 0, Input);
				continue;
			}
			else
				break;
		}
		// Hienj tuong idle 
		if (iReady == 0 && iRemain > 0 ) {

			pushProcess(&iReady, ReadyQueue, Input[0]);
			removeProcess(&iRemain, 0, Input);

			ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
			ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
			ReadyQueue[0].iTaT = ReadyQueue[0].iBurst;

			if (ReadyQueue[0].iBurst <= iQuantumTime) {
				ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
			}
			else {
				ReadyQueue[0].iFinish = ReadyQueue[0].iStart + iQuantumTime;
			}
			while (iRemain > 0)
			{
				if (Input[0].iArrival <= ReadyQueue[0].iFinish)
				{
					pushProcess(&iReady, ReadyQueue, Input[0]);
					removeProcess(&iRemain, 0, Input);
					continue;
				}
				else
					break;
			}
			PCB *temp = new PCB();
			temp->iPID = -1;
			temp->iFinish = ReadyQueue[0].iStart;
			pushProcessGant(&iGantt, GanttArray,*temp);
			
		
		}





		if (iReady > 1) {

			if (ReadyQueue[1].iTaT < 0) {
				ReadyQueue[1].iStart = ReadyQueue[0].iFinish;
				ReadyQueue[1].iResponse = ReadyQueue[1].iStart - ReadyQueue[1].iArrival;
				ReadyQueue[1].iTaT = ReadyQueue[1].iBurst;
				ReadyQueue[1].iFinish = ReadyQueue[1].iStart;
			}

			if (ReadyQueue[1].iBurst <= iQuantumTime) {
				ReadyQueue[1].iFinish = ReadyQueue[0].iFinish + ReadyQueue[1].iBurst;

			}
			else {
				ReadyQueue[1].iFinish = ReadyQueue[0].iFinish + iQuantumTime;
			}


		}
		if (iReady > 0) {
			
			if (ReadyQueue[0].iBurst <= iQuantumTime) { // thời gian nhỏ hơn có nghĩa là nó hết  
				ReadyQueue[0].iBurst = ReadyQueue[0].iTaT;
				ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
				ReadyQueue[0].iWaiting = ReadyQueue[0].iTaT - ReadyQueue[0].iBurst;

				pushProcessGant(&iGantt, GanttArray, ReadyQueue[0]);
				pushProcess(&iTerminated, TerminatedArray,ReadyQueue[0]);
				removeProcess(&iReady, 0, ReadyQueue);

			}
			else {
				ReadyQueue[0].iBurst -= iQuantumTime;
				pushProcessGant(&iGantt, GanttArray, ReadyQueue[0]);

				if (iReady == 1) {
					if (ReadyQueue[0].iBurst <=iQuantumTime) {
						ReadyQueue[0].iFinish = ReadyQueue[0].iFinish + ReadyQueue[0].iBurst;
					}
					else {
						ReadyQueue[0].iFinish = ReadyQueue[0].iFinish + iQuantumTime;
					}


				} 

				pushProcess(&iReady, ReadyQueue, ReadyQueue[0]);
				removeProcess(&iReady, 0, ReadyQueue);

			}
		
		}

	}
	printf("\n===== Round Robin Scheduling =====\n");

	exportGanttChart(iGantt, GanttArray);
	quickSort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID);
	printf("\nTerminated Queue: ");
	printProcessFull(iTerminated, TerminatedArray);

	calculateAWT(iTerminated, TerminatedArray);
	calculateATaT(iTerminated, TerminatedArray);

}

void pushProcess(int* n, PCB P[], PCB Q) {
	if (*n < 10) {
		P[*n] = Q;
		(*n)++;
	}
	else {
		printf("Error: Queue is full\n");
	}
}
void pushProcessGant(int* n, PCB P[], PCB Q) {
	if (*n < 100) {
		P[*n] = Q;
		(*n)++;
	}
	else {
		printf("Error: Queue is full\n");
	}
}

void removeProcess(int* n, int index, PCB P[]) {
	if (*n > 0 && index >= 0 && index < *n) {
		for (int i = index; i < *n - 1; i++) {
			P[i] = P[i + 1];
		}
		(*n)--;
	}
	else {
		printf("Error: Invalid index or empty queue\n");
	}
}
int partition(PCB P[], int low, int high, int iCriteria)
{
	int pivot = low;
	int i = low + 1;
	int j = high;

	while (i <= j)
	{
		if (compareProcesses(&P[i], &P[pivot], iCriteria) <= 0)
		{
			i++;
		}
		else if (compareProcesses(&P[j], &P[pivot], iCriteria) > 0)
		{
			j--;
		}
		else
		{
			swapProcess(&P[i], &P[j]);
			i++;
			j--;
		}
	}

	swapProcess(&P[pivot], &P[j]);

	return j;
}

void quickSort(PCB P[], int low, int high, int iCriteria)
{
	if (low < high)
	{
		int pivot = partition(P, low, high, iCriteria);

		quickSort(P, low, pivot - 1, iCriteria);
		quickSort(P, pivot + 1, high, iCriteria);
	}
}