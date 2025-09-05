#include "scheduler.h"

int	sType = 0;
char sAcronym[50];
int	quantum = 0;
char testFilePath[500];
int	numProcesses = 0;
int	origNumProcesses = 0;

struct Process {
	int	id;
	int	burst;
	int	priority;
	int	turnAroundTime;
	int	startTime;
	int	waitTime;
	int	timesRan;
};

struct Process *p;
int	pSize = 0;

float averageWaitTime = 0.00;
float averageTurnAroundTime = 0.00;


void printInitialSection(){
	printf("\nScheduler :  %d %s\n", sType, sAcronym);
	printf("Quantum   :  %d\n", quantum);
	printf("Sch. File : %s\n", testFilePath);
	printf("-------------------------------\n");

	printf("Arrival Order: ");
	for (int i = 0; i < pSize; i++) {
		printf("%d", p[i].id);
		if (i != pSize - 1) {
			printf(", ");
		}
	}
	printf("\nProcess Information:\n");
	for (int i = 0; i < pSize; i++) {
		printf("%d%9d%9d\n", p[i].id, p[i].burst, p[i].priority);
	}
	//Something
		printf("-------------------------------\n");
	printf("Running...\n");
	printf("##################################################\n");
	printf("#  #    CPU    Pri       W       T\n");
}


void printFinalSection(){
	printf("##################################################\n");
	printf("# Avg. Waiting Time : %.2f\n", averageWaitTime);
	printf("# Avg. Turnaround Time: %.2f\n", averageTurnAroundTime);
	printf("##################################################\n\n");
}


void readArguments(int argc, char *argv[]){
	char nonQuantum[] = "-s";
	char Quantum[] = "-q";
	const char *acronyms[] = {"FCFS", "SJF", "Priority", "RR"};

	if (strcmp(argv[1], nonQuantum) == 0) {
		sType = atoi(argv[2]);
		switch (sType) {
		case 1:
			strncpy(sAcronym, acronyms[0], 50);
			break;
		case 2:
			strncpy(sAcronym, acronyms[1], 50);
			break;
		case 3:
			strncpy(sAcronym, acronyms[2], 50);
			break;
		case 4:
			strncpy(sAcronym, acronyms[3], 50);
			break;
		}
		strncpy(testFilePath, argv[3], 500);
	} else if (strcmp(argv[1], Quantum) == 0) {
		quantum = atoi(argv[2]);
		strncpy(testFilePath, argv[3], 500);
		sType = atoi(argv[5]);
		switch (sType) {
		case 1:
			strncpy(sAcronym, acronyms[0], 50);
			break;
		case 2:
			strncpy(sAcronym, acronyms[1], 50);
			break;
		case 3:
			strncpy(sAcronym, acronyms[2], 50);
			break;
		case 4:
			strncpy(sAcronym, acronyms[3], 50);
			break;
		}
	}
}


void readFile(){

	FILE *file;
	char line[100];

	file = fopen(testFilePath, "r");
	if (file == NULL) {
		perror("Error opening file");
	}

	const char *nP = fgets(line, 100, file);
	numProcesses = atoi(nP);
	p = malloc(sizeof(struct Process) * (numProcesses));
	if (p == NULL) {
		fprintf(stderr, "Memory Allocation Failed");
		exit(1);
	}

	int	num = 0;
	int	arrayIndex = 0;
	while (fgets(line, 1000, file) != NULL) {
		char *t = strtok(line, " ");
		int i = 0;
		p[arrayIndex].turnAroundTime = 0;
		p[arrayIndex].startTime = 0;
		p[arrayIndex].waitTime = 0;
		p[arrayIndex].timesRan = 0;
		while (t != NULL) {
			num = atoi(t);
			t = strtok(NULL, " ");
			switch (i) {
			case 0:
				p[arrayIndex].id = num;
				break;
			case 1:
				p[arrayIndex].burst = num;
				break;
			case 2:
				p[arrayIndex].priority = num;
				break;
			}
			i++;
		}
		arrayIndex++;
		pSize++;
	}

	fclose(file);
}


void printP_Stats(int id, int burst, int priority, int waitTime, int turnAroundTime){
	printf("#  %d    %2d    %4d    %4d    %4d\n", id, burst, priority, waitTime, turnAroundTime);
}


void FCFS(){
	int	currTime = 0;
	int	startTime = 0;
	for (int i = 0; i < numProcesses; i++) {
		startTime = currTime;
		p[i].waitTime = currTime;
		currTime = currTime + p[i].burst;
		p[i].turnAroundTime = startTime + p[i].burst;
		printP_Stats(p[i].id, p[i].burst, p[i].priority, p[i].waitTime, p[i].turnAroundTime);
	}

	int	totalWait = 0;
	int	totalTurnAround = 0;
	for (int j = 0; j < numProcesses; j++) {
		totalWait = totalWait + p[j].waitTime;
		totalTurnAround = totalTurnAround + p[j].turnAroundTime;
	}

	averageWaitTime = (float)totalWait / (float)numProcesses;
	averageTurnAroundTime = (float)totalTurnAround / (float)numProcesses;
}


void insertionSortSJF(){
	struct Process	curr;
	for (int i = 1; i < numProcesses; ++i) {
		curr = p[i];
		int		j = i - 1;

		while (j >= 0 && p[j].burst > curr.burst) {
			p[j + 1] = p[j];
			j--;
		}
		p[j + 1] = curr;
	}
}


void insertionSortPriority(){
	int	j = 0;
	struct Process curr;
	for (int i = 1; i < numProcesses; i++) {
		curr = p[i];
		j = i - 1;

		while (j >= 0 && p[j].priority > curr.priority) {
			p[j + 1] = p[j];
			j--;
		}
		p[j + 1] = curr;
	}
}


void SJF(){
	insertionSortSJF();
	FCFS();
}


void priority(){
	insertionSortPriority();
	FCFS();
}


void RR(){
	int	currTime = 0;
	int	startTime = 0;
	origNumProcesses = numProcesses;
	for (int i = 0; i < numProcesses; i++) {
		int	remainder = p[i].burst - quantum;
		startTime = currTime;
		p[i].waitTime = currTime + p[i].waitTime;
		currTime = currTime + p[i].burst;

		if (remainder > 0) {
			numProcesses++;
			p = realloc(p, (numProcesses) * (sizeof(struct Process)));
			if (p == NULL) {
				fprintf(stderr, "Memory Allocation Failed");
				exit(1);
			}
			p[numProcesses - 1].id = p[i].id;
			p[numProcesses - 1].burst = remainder;
			p[numProcesses - 1].priority = p[i].priority;
			p[numProcesses - 1].turnAroundTime = p[i].turnAroundTime;
			p[numProcesses - 1].waitTime = p[i].waitTime;
			if (p[i].timesRan == 0) {
				p[numProcesses - 1].startTime = startTime;
			}
			p[numProcesses - 1].timesRan = p[i].timesRan + 1;
		}
		if (remainder <= 0) {
			p[i].turnAroundTime = p[i].startTime + currTime;
		}
		printP_Stats(p[i].id, p[i].burst, p[i].priority, p[i].waitTime, p[i].turnAroundTime);
	}

	int	totalWait = 0;
	int	totalTurnAround = 0;
	int	*idCheck;
	int	idC_Size = 0;
	idCheck = malloc(idC_Size * sizeof(int));
	for (int j = numProcesses - 1; j > 0; j--) {
		int	check = 0;
		for (int i = 0; i < idC_Size; i++) {
			if (p[j].id == p[i].id) {
				check = 1;
			}
		}
		if (check != 1) {
			totalWait = totalWait + p[j].waitTime;
			totalTurnAround = totalTurnAround + p[j].turnAroundTime;
			idC_Size++;
			idCheck = realloc(idCheck, (idC_Size * sizeof(int)));
			idCheck[idC_Size - 1] = p[j].id;
		}
	}
	averageWaitTime = (float)totalWait / (float)origNumProcesses;
	averageTurnAroundTime = (float)totalTurnAround / (float)origNumProcesses;
}


void runProccesses(){

	switch (sType) {
	case 1:
		FCFS();
		break;
	case 2:
		SJF();
		break;
	case 3:
		priority();
		break;
	case 4:
		RR();
		break;
	}
}



int main(int argc, char **argv){
	readArguments(argc, argv);
	readFile();
	printInitialSection();
	runProccesses();
	printFinalSection();

	return 0;
}
