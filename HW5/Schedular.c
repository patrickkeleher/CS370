#include <stdio.h>
#include <stdlib.h>
// Author Patrick Keleher
typedef struct{
    int Process_ID;
    int Arrival_Time;
    int Burst_Duration;
    int Priority;
    int TurnAroundTime;
    int WaitTime;
}Process;
//**********************************************************
typedef struct{
  int Process_ID;
  int Arrival_Time;
  int End_Time;
}Gantt;

typedef struct{
    int size;
    Process* heap[];
}Heap;

void File_Content(Process Processes[], int number);
void FCFS(Process Processes[], int number);
void SJFP(Process Processes[], int number);
void PRIORITY(Process Processes[], int number);
void RR(Process Processes[], int number, int quantum);
void Display_Output(Process Processes[],int number,Gantt Rows[],int count, char* filename);
void FCFSgantt(Process*, Gantt*, int*);
void FCFSsort(Process[], int);
void SJFPgantt(Process*, Gantt*, int);
void SJFgantt(Process*, Gantt*, int);
Process* findLowestBurst(Process[], Process[], int, int, Process*);
Process* findNewLowestBurst(Process[], Process[], int, int, Process*);
Process* findLowestPriority(Process[], Process[], int, int, Process*);
Process* findNewLowestPriority(Process[], Process[], int, int, Process*);
int origArr(Process[], Process* ,int);

int main(int argc, char **argv){

    int number_of_pross = 0;

    	if (argc != 2){
       		printf("Incorrect number of arguments.\n");
       	 	return -1;
    	}

        FILE* file1 = fopen(argv[1], "r");

    while(!feof(file1)){
        char  ch = fgetc(file1);
        if(ch == '\n'){
            number_of_pross++;
        }
    }

    Process Processes[number_of_pross];

    fseek(file1, 0, SEEK_SET);

    number_of_pross=0;


    while(fscanf(file1, "%d,%d,%d,%d",&Processes[number_of_pross].Process_ID,&Processes[number_of_pross].Arrival_Time,&Processes[number_of_pross].Burst_Duration,&Processes[number_of_pross].Priority) != EOF	){
        number_of_pross++;
    }

    fclose(file1);

    File_Content(Processes,number_of_pross);
    FCFS(Processes,number_of_pross);
    SJFP(Processes,number_of_pross);
    PRIORITY(Processes,number_of_pross);

    return 0;
}
//--------------------------------------------------------
void File_Content(Process Processes[], int number){

    int i;
    printf("PROCESS ID\tARRIVAL TIME\tBURST DURATION\tPRIORITY\n");

    for(i=0;i<number;i++){
        printf("%d\t\t%d\t\t%d\t\t%d\n",Processes[i].Process_ID,Processes[i].Arrival_Time,Processes[i].Burst_Duration,Processes[i].Priority);

    }
}
//---------------------------------------------------------------
void Display_Output(Process Processes[],int number, Gantt Rows[],int count, char *filename){

      FILE* file;
      int i;
      file=fopen(filename,"w");
      fprintf(file,"PROCESS ID\tARRIVAL TIME\tEND TIME\n");

      for(i=0;i<count;i++)
        fprintf(file,"%10d%10d%10d\n",Rows[i].Process_ID,Rows[i].Arrival_Time,Rows[i].End_Time);

      float avgWait=0;
      float avgTurnAround=0;

      for(i=0;i<number;i++){
          avgWait+=Processes[i].WaitTime;
          avgTurnAround+=Processes[i].TurnAroundTime;
      }

      fprintf(file,"Average Wait Time=%f\n",avgWait/number);
      fprintf(file,"TurnAround Time=%f\n",avgTurnAround/number);
      // Assumption: the value of count recieved is greater than exact number of rows in the Gantt chart by 1. Dont forget to adjust the value if yours is not so.
      fprintf(file,"Throughput=%f\n",((float)number/Rows[count-1].End_Time));
      fclose(file);

}
//---------------------------------------------------------------Display_Output(Processes, number, chart, 1, "FCFS");

/*
 Note: Use function Display_Output to display the result. The Processes is the
 updated Processes with their respective waittime, turnAround time.
 Use the array of Gantt Struct to mimic the behaviour of Gantt Chart. Rows is the base name of the same array.
 number is the number of processes which if not changed during processing, is taken care. count is the number of rows in the Gantt chart.

 Number of Processes will not exceed 1000. Create and array of Gantt chart which can accomodate atleast 1000 process.
 */
// -------------FCFS----------------------------------------------
void FCFS_Controller(Process Processes[], int number){
  Gantt chart[1000];
  Process p[number];
  int ganttIndex = 0;
  int lastEnd = 0;

  for (int i = 0 ; i < number ; i++){
      p[i] = Processes[i];
      p[i].Process_ID = Processes[i].Process_ID;
      p[i].Burst_Duration = Processes[i].Burst_Duration;
      p[i].Arrival_Time = Processes[i].Arrival_Time;
      p[i].WaitTime = 0;
      p[i].TurnAroundTime = 0;
  }

  FCFSsort(p, number);
  for ( int i = 0 ; i < number ; i++){
      FCFSgantt(&(p[i]), &chart[ganttIndex], &lastEnd);
      ganttIndex++;
  }

  //---------End of Processing
  Display_Output(p, number, chart, number, "FCFS");

}
void FCFS(Process Processes[], int number)
{
    FCFS_Controller(Processes,number);
}
//---------------------SJFP----------------------------------------
void SJFP_Controler(Process Processes[], int number){
  Gantt SJFPchart[1000];
   int chartIndex = 0;
   Process p[number];
   #define INIT_PROCESS -1
   #define MAX_DURATION 9999999
   int TOTAL_RUN_TIME = 0;
   Process* Lowest_Process;
   Process temp;
   temp.Process_ID = INIT_PROCESS;
   temp.Burst_Duration = MAX_DURATION;
   temp.Priority = 999999;
   Process* Running_Process;
   Running_Process = &temp;

   for (int i = 0 ; i < number ; i++){
       TOTAL_RUN_TIME += Processes[i].Burst_Duration;
       p[i].Process_ID = Processes[i].Process_ID;
       p[i].Burst_Duration = Processes[i].Burst_Duration;
       p[i].Arrival_Time = Processes[i].Arrival_Time;
       p[i].WaitTime = 0;
       p[i].TurnAroundTime = 0;
   }


   for (int time = 0 ; time <= TOTAL_RUN_TIME ; time++){
       Lowest_Process = Running_Process;
       Running_Process->Burst_Duration--;
       Lowest_Process = findNewLowestBurst(Processes, p, time, number, Lowest_Process);

       if ((Lowest_Process->Process_ID) == (Running_Process->Process_ID)){
           if (Running_Process->Burst_Duration == 0){

               SJFgantt(Running_Process, &SJFPchart[chartIndex], time);
               chartIndex++;

               Running_Process = findNewLowestBurst(Processes, p, time, number, &temp);
               Running_Process->WaitTime += time - Running_Process->Arrival_Time;
               Running_Process->Arrival_Time = time;
           }
       }else{

           if ((Running_Process->Process_ID) != INIT_PROCESS){
               SJFPgantt(Running_Process, &SJFPchart[chartIndex], time);
               chartIndex++;
               Running_Process->Arrival_Time = time;
           }
           Running_Process = Lowest_Process;
           Running_Process->Arrival_Time = time;
       }
   }


   Display_Output(p, number, SJFPchart, chartIndex, "SJFP");

}
void SJFP(Process Processes[],int number)
{
    SJFP_Controler(Processes, number);
}
//------------------PRIORITY---------------------------------------

void PRIORITY_Controler(Process Processes[], int number) {
  Gantt PRIORITYchart[1000];
  int chartIndex = 0;
  Process p[number];
  #define INIT_PROCESS -1
  #define MAX_DURATION 9999999
  int TOTAL_RUN_TIME = 0;
  Process* Lowest_Process;
  Process temp;
  temp.Process_ID = INIT_PROCESS;
  temp.Burst_Duration = MAX_DURATION;
  temp.Priority = 999999;
  Process* Running_Process;
  Running_Process = &temp;
  int i =0;
  while(i < number){
      TOTAL_RUN_TIME += Processes[i].Burst_Duration;
      p[i].Process_ID = Processes[i].Process_ID;
      p[i].Burst_Duration = Processes[i].Burst_Duration;
      p[i].Arrival_Time = Processes[i].Arrival_Time;
      p[i].Priority = Processes[i].Priority;
      p[i].WaitTime = 0;
      p[i].TurnAroundTime = 0;
      i++;
  }


  for (int time = 0 ; time <= TOTAL_RUN_TIME ; time++){
      Lowest_Process = Running_Process;
      Running_Process->Burst_Duration--;
      Lowest_Process = findNewLowestPriority(Processes, p, time, number, Lowest_Process);


      if ((Lowest_Process->Process_ID) == (Running_Process->Process_ID)){
          if ((Lowest_Process->Burst_Duration) == 0){
              SJFgantt(Running_Process, &PRIORITYchart[chartIndex], time);
              chartIndex++;

              Running_Process = findNewLowestPriority(Processes, p, time, number, &temp);
              Running_Process->WaitTime += (time - Running_Process->Arrival_Time);
              Running_Process->Arrival_Time = time;
          }
      }else{

          if ((Running_Process->Process_ID) != INIT_PROCESS){
              SJFPgantt(Running_Process, &PRIORITYchart[chartIndex], time);
              chartIndex++;
              Running_Process->Arrival_Time = time;
          }
          Running_Process = Lowest_Process;
          Running_Process->Arrival_Time = time;
      }
  }

  //---------End of Processing
  Display_Output(p, number, PRIORITYchart, chartIndex, "PRIORITY");
}
void PRIORITY(Process Processes[], int number)
{
    PRIORITY_Controler( Processes, number);
}
//-----------------------------------------------------------------
Process* findNewLowestBurst(Process Processes[], Process p[], int time, int number, Process* Lowest_Process){
    int i =0;
    while ( i < number){
        if ((Processes[i].Arrival_Time <= time) && (p[i].Burst_Duration > 0)){
            if (p[i].Burst_Duration < Lowest_Process->Burst_Duration)
                Lowest_Process = &p[i];
            if (p[i].Burst_Duration == Lowest_Process->Burst_Duration){
                if (Processes[i].Arrival_Time < origArr(Processes, Lowest_Process, number)){
                    Lowest_Process = &p[i];
                }
            }
        }
        i++;
    }
    return Lowest_Process;
}
//-----------------------------------------------------------------
Process* findNewLowestPriority(Process Processes[], Process p[], int time, int number, Process* Lowest_Process){
    int i= 0;
    while ( i< number){
        if ((Processes[i].Arrival_Time <= time) && (p[i].Burst_Duration > 0)){
            if (p[i].Priority < Lowest_Process->Priority)
                Lowest_Process = &p[i];
            if (p[i].Priority == Lowest_Process->Priority){
                if (Processes[i].Arrival_Time < origArr(Processes, Lowest_Process, number)){
                    Lowest_Process = &p[i];
                }
            }
        }
        i++;
    }
    return Lowest_Process;
}
int origArr(Process Processes[], Process* p,int number){
   int i =0;
    while( i < number){
        if (Processes[i].Process_ID == p->Process_ID)
            return Processes[i].Arrival_Time;
            i++;
    }
}
//-----------------------------------------------------------------
void SJFgantt(Process* p, Gantt* g, int time){
    p->TurnAroundTime = time - p->Arrival_Time + p->WaitTime;
    g->Process_ID = p->Process_ID;
    g->Arrival_Time = p->Arrival_Time;
    g->End_Time = time;
}
void SJFPgantt(Process* p, Gantt* g, int time){
    g->Process_ID = p->Process_ID;
    g->Arrival_Time = p->Arrival_Time;
    g->End_Time = time;
}
//-----------------------------------------------------------------
void FCFSgantt(Process* p, Gantt* g, int* lastEnd){
    g->Process_ID = p->Process_ID;
    p->WaitTime = *lastEnd - p->Arrival_Time;
    p->TurnAroundTime = p->WaitTime + p->Burst_Duration;
    g->Arrival_Time = *lastEnd;
    g->End_Time = g->Arrival_Time + p->Burst_Duration;
    *lastEnd = g->End_Time;
}
//-----------------------------------------------------------------
void FCFSsort(Process Processes[], int number){
    int i =0;
   while (i < number){
        int m = i;
        for (int j = i ; j < number ; j++){
            if (Processes[j].Arrival_Time < Processes[m].Arrival_Time){
                m = j;
            }
            if (Processes[j].Arrival_Time == Processes[m].Arrival_Time){
                if (Processes[j].Process_ID < Processes[m].Process_ID){
                    m = j;
                  }}
        }
        Process temp = Processes[i];
        Processes[i] = Processes[m];
        Processes[m] = temp;
        i++;
    }
}
