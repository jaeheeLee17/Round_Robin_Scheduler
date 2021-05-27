#include <stdlib.h>
#include <stdio.h>

typedef struct _process {
	int process_id;
	int arrival_time;
	int waiting_time;
	int burst_time;
	int turnaround_time;
} Process;

void process_init(Process p_struct_arr[], int p_cnt);
int comp_with_arrival_time(const void *x, const void *y);
void sorting_process_by_arrival_time(Process p_struct_arr[], int p_len);

int main(void) {
	FILE *rfp;
	int i = 0, j = 0, p_cnt = 100;
	int time_quantum;
	Process *p_struct_arr;

	if ((rfp = fopen("input.txt", "r")) == NULL) {
		perror("fopen: input.txt");
		exit(1);
	}

	p_struct_arr = (Process *)malloc(sizeof(Process) * p_cnt);
	process_init(p_struct_arr, p_cnt);

	while (i < p_cnt) {
		fscanf(rfp, "%d %d %d", &p_struct_arr[i].process_id, &p_struct_arr[i].arrival_time, &p_struct_arr[i].burst_time);
		i++;
	}

	printf("Sample Process List\n");
	printf("\t+-----+--------------+-------------+\n");
	printf("\t| PID | Arrival Time |  Burst Time |\n");
	printf("\t+-----+--------------+-------------+\n");
	for (j = 0; p_struct_arr[j].process_id != -1; j++) {
		printf("\t| %3d |      %3d     |     %3d     |\n",
				p_struct_arr[j].process_id, p_struct_arr[j].arrival_time, p_struct_arr[j].burst_time);
		printf("\t+-----+--------------+-------------+\n");
	}
	printf("\n");

	p_cnt = j;
	sorting_process_by_arrival_time(p_struct_arr, p_cnt);

	/*
	printf("Sample Process List\n");
        printf("\t+-----+--------------+-------------+\n");
        printf("\t| PID | Arrival Time |  Burst Time |\n");
        printf("\t+-----+--------------+-------------+\n");
        for (int i = 0; p_struct_arr[i].process_id != -1; i++) {
                printf("\t| %3d |      %3d     |     %3d     |\n",
                                p_struct_arr[i].process_id, p_struct_arr[i].arrival_time, p_struct_arr[i].burst_time);
                printf("\t+-----+--------------+-------------+\n");
        }
        printf("\n");
	*/

	printf("Enter the time quantum : ");
	scanf("%d", &time_quantum);

	int remaining_process = p_cnt;
	int check = 0;
	int *rem_bt = (int *)malloc(sizeof(int) * p_cnt);
	int waiting_time_sum = 0;
	int turnaround_time_sum = 0;

	for (int i = 0; i < p_cnt; i++) {
		rem_bt[i] = p_struct_arr[i].burst_time;
	}

	for (int cur_time = 0, i = 0; remaining_process > 0; ) {
		if (rem_bt[i] <= time_quantum && rem_bt[i] > 0) {
			cur_time += rem_bt[i];
			rem_bt[i] = 0;
			check = 1;
		} else if (rem_bt[i] > time_quantum && rem_bt[i] > 0) {
			cur_time += time_quantum;
			rem_bt[i] -= time_quantum;
		}

		if (rem_bt[i] == 0 && check == 1) {
			remaining_process--;
			p_struct_arr[i].waiting_time = cur_time - p_struct_arr[i].arrival_time - p_struct_arr[i].burst_time;
			p_struct_arr[i].turnaround_time = cur_time - p_struct_arr[i].arrival_time;
			waiting_time_sum += p_struct_arr[i].waiting_time;
			turnaround_time_sum += p_struct_arr[i].turnaround_time;
			check = 0;
		}

		if (i == p_cnt - 1) {
			i = 0;
		} else if (p_struct_arr[i + 1].arrival_time <= cur_time) {
			i++;
		} else {
			i = 0;
		}
	}

	printf("\tRound Robin Scheduling Report ( Time Quantum = %d )\n\n", time_quantum);
	printf("\t+-----+---------------+------------+--------------+-----------------+\n");
	printf("\t| PID | Arrival Time  | Burst Time | Waiting Time | Turnaround Time |\n");
	printf("\t+-----+---------------+------------+--------------+-----------------+\n");

	for (int i = 0; i < p_cnt; i++) {
		printf("\t| %3d |      %3d      |     %3d    |      %3d     |        %3d      |\n", p_struct_arr[i].process_id, p_struct_arr[i].arrival_time, p_struct_arr[i].burst_time, p_struct_arr[i].waiting_time, p_struct_arr[i].turnaround_time);
		printf("\t+-----+---------------+------------+--------------+-----------------+\n");
	}
	printf("\n");

	printf("\n\tAverage Waiting Time : %-2.2lf\n", waiting_time_sum * 1.0 / p_cnt);
	printf("\n\tAverage Turnaround Time : %-2.2lf\n", turnaround_time_sum * 1.0 / p_cnt);

	fclose(rfp);
	free(p_struct_arr);


	//Gantt chart
	int w_time=0;
	int time = 1;
	int p_id;
	for(int i = 0; p_struct_arr[i].process_id != -1; i++){
		w_time = w_time + p_struct_arr[i].burst_time;
	}
	//printf("w_time:%d\n",w_time);
	printf("\n\nGantt chart\n");
	printf("\t+------+-----+\n");
	printf("\t| time | PID |\n");
	
	while(time < w_time){
		//printf("\t+-%3d--+-----+\n",j);
		for(p_id=0; p_id<p_cnt; p_id++){
			for(int k=0; k < time_quantum; k++){
				if(p_struct_arr[p_id].burst_time > time_quantum){
					for(int t=0; t < time_quantum; t++){
						printf("\t+ %3d  |%3d  +\n",time, p_id);
						time++;
						p_struct_arr[p_id].burst_time--;
					}
					break;	
				}
				else if(p_struct_arr[p_id].burst_time <= time_quantum){
					for(int t1=0; t1 < p_struct_arr[p_id].burst_time; t1++){
						printf("\t+ %3d  |%3d  +\n",time, p_id);
						time++;
						//p_struct_arr[p_id].burst_time--;
					}
					p_struct_arr[p_id].burst_time = 0;
					break;
				}
				else if(p_struct_arr[p_id].burst_time == 0){
					break;
				}

			}
			//p_id = 0;
		}
		
	}

	printf("\t+------+-----+\n");


	///Gantt chart






	return 0;
}

void process_init(Process p_struct_arr[], int p_cnt) {
	for (int i = 0; i < p_cnt; i++) {
		p_struct_arr[i].process_id = -1;
		p_struct_arr[i].arrival_time = 0;
		p_struct_arr[i].waiting_time = 0;
		p_struct_arr[i].burst_time = 0;
		p_struct_arr[i].turnaround_time = 0;
	}
}

int comp_with_arrival_time(const void *x, const void *y) {
	Process *p1 = (Process *)x;
	Process *p2 = (Process *)y;

	return p1->arrival_time >= p2->arrival_time ?
	(p1->arrival_time > p2->arrival_time ? 1 : 0) : -1;
}

void sorting_process_by_arrival_time(Process p_struct_arr[], int p_len) {
	qsort(p_struct_arr, p_len, sizeof(Process), comp_with_arrival_time);
}


