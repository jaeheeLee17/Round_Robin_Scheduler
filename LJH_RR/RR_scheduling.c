#include <stdlib.h>
#include <stdio.h>

typedef struct _process {
	int process_id;
	int arrival_time;
	int waiting_time;
	int return_time;
	int burst_time;
	int turnaround_time;
	int response_time;
	int completed;
} Process;

void process_init(Process p_struct_arr[], int p_cnt) {
	for (int i = 0; i < p_cnt; i++) {
		p_struct_arr[i].process_id = -1;
		p_struct_arr[i].arrival_time = 0;
		p_struct_arr[i].waiting_time = 0;
		p_struct_arr[i].return_time = 0;
		p_struct_arr[i].burst_time = 0;
		p_struct_arr[i].turnaround_time = 0;
		p_struct_arr[i].response_time = 0;
		p_struct_arr[i].completed = 0;
	}
}

int comp_with_arrival_time(const void *x, const void *y) {
	Process *p1 = (Process *)x;
	Process *p2 = (Process *)y;

	if (p1->arrival_time < p2->arrival_time) {
		return -1;
	} else if (p1->arrival_time > p2->arrival_time) {
		return 1;
	} else {
		return 0;
	}
}

void sorting_process_by_arrival_time(Process p_struct_arr[], int p_len) {
	qsort(p_struct_arr, p_len, sizeof(Process), comp_with_arrival_time);
}

void find_waiting_time(Process p_struct_arr[], int p_len, int time_quantum) {
	int cur_time = 0;

	int *rem_bt = (int *)malloc(sizeof(int) * p_len);
	int *find_response_time = (int *)malloc(sizeof(int) * p_len);

	for (int i = 0; i < p_len; i++) {
		rem_bt[i] = p_struct_arr[i].burst_time;
		find_response_time[i] = 0;
	}

	while (1) {
		int check = 1;

		for (int i = 0; i < p_len; i++) {
			if (rem_bt[i] > 0) {
				check = 0;
				if (find_response_time[i] == 0) {
					p_struct_arr[i].response_time = cur_time - p_struct_arr[i].arrival_time;
					find_response_time[i] = 1;
				}

				if (rem_bt[i] > time_quantum) {
					cur_time += time_quantum;
					rem_bt[i] -= time_quantum;
				} else {
					cur_time += rem_bt[i];
					p_struct_arr[i].waiting_time = cur_time - p_struct_arr[i].burst_time;
					rem_bt[i] = 0;
				}
			}
		}

		if (check == 1) {
			break;
		}
	}
	free(rem_bt);
	free(find_response_time);
}

void find_turnaround_time(Process p_struct_arr[], int p_len) {
	for (int i = 0; i < p_len; i++) {
		p_struct_arr[i].turnaround_time =  p_struct_arr[i].burst_time + p_struct_arr[i].waiting_time - p_struct_arr[i].arrival_time;
	}
}

void draw_gantt_chart(Process p_struct_arr[], int p_len, int time_quantum) {

}

void Round_Robin(Process p_struct_arr[], int p_len, int time_quantum) {
	int waiting_time_sum = 0;
	int turnaround_time_sum = 0;
	int response_time_sum = 0;

	sorting_process_by_arrival_time(p_struct_arr, p_len);
	find_waiting_time(p_struct_arr, p_len, time_quantum);
	find_turnaround_time(p_struct_arr, p_len);

	for (int i = 0; i < p_len; i++) {
		p_struct_arr[i].waiting_time = p_struct_arr[i].turnaround_time - p_struct_arr[i].burst_time;

		waiting_time_sum += p_struct_arr[i].waiting_time;
		turnaround_time_sum += p_struct_arr[i].turnaround_time;
		response_time_sum += p_struct_arr[i].response_time;
	}

	// draw_gantt_chart(p_struct_arr, p_len, time_quantum);

	printf("\tRound Robin Scheduling Report ( Time Quantum = %d )\n\n", time_quantum);
	printf("\t+-----+---------------+------------+-----------------+--------------+-----------------+\n");
	printf("\t| PID | Arrival Time  | Burst Time |  Response Time  | Waiting Time | Turnaround Time |\n");
	printf("\t+-----+---------------+------------+-----------------+--------------+-----------------+\n");

	for (int i = 0; i < p_len; i++) {
		printf("\t| %3d |      %3d      |     %3d    |       %3d       |      %3d     |        %3d      |\n", p_struct_arr[i].process_id, p_struct_arr[i].arrival_time, p_struct_arr[i].burst_time, p_struct_arr[i].response_time, p_struct_arr[i].waiting_time, p_struct_arr[i].turnaround_time);
		printf("\t+-----+---------------+------------+-----------------+--------------+-----------------+\n");
	}
	printf("\n");

	printf("\n\tAverage Waiting Time : %-2.2lf\n", waiting_time_sum * 1.0 / p_len);
	printf("\n\tAverage Turnaround Time : %-2.2lf\n", turnaround_time_sum * 1.0 / p_len);
}

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

	sorting_process_by_arrival_time(p_struct_arr, j);

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

	Round_Robin(p_struct_arr, j, time_quantum);

	fclose(rfp);
	free(p_struct_arr);

	return 0;
}
