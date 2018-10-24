#include<stdio.h>
#include<string.h>
#include<math.h>
int main()
{
    int totalmemory,pagesize,totalpages;
    /**Total memory, Size of page and total number of pages*/
    printf("\nEnter Total Memory: ");
    scanf("%d",&totalmemory);
    printf("\nEnter The Page Size: ");
    scanf("%d",&pagesize);
    char fname[100];
    printf("\nEnter Input File:");
    scanf("%s",fname);
    printf("%s",fname);
    FILE *file= fopen(fname,"r");
    int i=0;
    int totalprocess; //Total number of processes to be executed
    fscanf(file,"%d",&totalprocess);
    while(totalprocess>20)
    {
        printf("no of processes too much,enter a valid file name:");
        fscanf(file,"%d",&totalprocess);
    }
    int n=totalprocess, //Counter
        processid[totalprocess], //Array for process ID
        arrivaltime[totalprocess], //Array for arrival time for dedicated process
        bursttime[totalprocess], //Burst time (Duration of execution)
        processtate[totalprocess]; //State of process
        /**State of process is defined at what stage process is going threw, In our simulation basically we have four state of  process that are ReadyState, RunningState, FinishedState and DeadState(Which can't be execute)*/
        /**There are four state code that are
        -1 = Nurture State
        0 = ReadyState
        1 = RunningState
        2 = FinishState
        3 = Can't Run In This Setup*/
    int j,k,l; //Counters
    int totalmemoryrequired[totalprocess]; //Total memory required by respective process
    int individualpageframe[totalprocess][100]; //Individual page frames for particular process
    int numberofpages[totalprocess]; // Number of pages for particular process;
    //Individual page frame cannot be more than 100

    for(i=0;i<totalprocess;i++)
    {
        totalmemoryrequired[i]=0; //Total memory required by a particular process
        processtate[i]=-1; //Initialized to nurture state
        /**Taking data from formatted file, Format of file is given*/
        fscanf(file,"%d",&processid[i]);
        fscanf(file,"%d",&arrivaltime[i]);
        fscanf(file,"%d",&bursttime[i]);
        fscanf(file,"%d",&numberofpages[i]);
        for(j=0;j<numberofpages[i];j++)
        {
            fscanf(file,"%d",&k);
            individualpageframe[i][j]=k;
            totalmemoryrequired[i]+=k;
        }
    }
    for(i=0;i<totalprocess;i++)
    {
        printf("\n***************************\n");
        printf("Process ID : %d\n",processid[i]);
        printf("Arrival Time : %d Burst Time: %d\n",arrivaltime[i],bursttime[i]);
        printf("Total Number Of Pages : %d\n\n",numberofpages[i]);
        for(j=0;j<numberofpages[i];j++)
        {
            printf("Size Of Page %d is %d\n",j+1,individualpageframe[i][j]);
        }
        printf("\n***************************\n");
    }
    while(totalmemory%pagesize!=0&&pagesize>0)
    {
       printf("\nError: Enter the size of the page which is the factor of main memory size-%d: ",totalmemory);
       scanf("%d",&pagesize);
    }
    totalpages=totalmemory/pagesize;
    int time,flag;
    int responsetime[totalprocess],endtime[totalprocess],pageused[totalprocess],totalavailablememory=totalmemory;
    printf("\n--------TOTAL REQUIRED MEMORY BY PROCESSES--------------\n");
    for(i=0;i<totalprocess;i++)
    {
        printf("PROCESS %d REQUIRED( %d (%d Page)\n",processid[i],totalmemoryrequired[i]);
    }
    for(time=0;1;time++)
    {
        flag=1; //to check all process are completed or not
        for(k=0;k<totalprocess;k++)
        {
            if(processtate[k]==0||processtate[k]==1||processtate[k]==-1)/**Checking condition that all process should be either finished or can't be execute state*/
                flag=0;
        }
        if(flag==1)
            break;
        for(k=0;k<totalprocess;k++)
        {
            if(processtate[k]==3)
            {
               endtime[k]=0;
            }
            else if(processtate[k]==2)
            {
               //process is finished
            }
            else if(processtate[k]==1)
            {
                //process is in running mode
                if(time==responsetime[k]+bursttime[k])
                {

                    processtate[k]=2;
                    endtime[k]=time;
                    totalavailablememory=totalavailablememory+(pageused[k]*pagesize);
                    printf("\nProcess %d is ended at %d",processid[k],time);
                    printf("\t Total memory available is %d (Page %d)",totalavailablememory,totalavailablememory/pagesize);
                }
            }
            else if(processtate[k]==0)
            {
                //process is ready to start
                if(arrivaltime[k]<=time&&totalavailablememory>=totalmemoryrequired[k]) /**process will execute if and only if arrivaltime less than or equal to current time and total available memory is greater than memory required by that process*/
                {
                    processtate[k]=1;
                    if(totalmemoryrequired[k]%pagesize==0)
                    {
                        pageused[k]=totalmemoryrequired[k]/pagesize;
                    }
                    else
                    {
                        pageused[k]=totalmemoryrequired[k]/pagesize+1;
                    }
                    totalavailablememory=totalavailablememory-(pagesize*pageused[k]);
                    responsetime[k]=time;
                    printf("\nProcess %d is started at %d",processid[k],time);
                    printf("\t Total memory available is %d (Page %d)",totalavailablememory,totalavailablememory/pagesize);
                }
            }
            else if(processtate[k]==-1)
            {
                if(totalmemoryrequired[k]>totalavailablememory)
                {
                    processtate[k]=3;
                    printf("\n Process %d can't be execute in this setup, shortage of memory it need %d more memory\n",processid[k],totalmemoryrequired[k]-totalmemory);
                    responsetime[k]=0;
                }
                else
                {
                    processtate[k]=0;
                }
            }
        }
    }
    printf("\n************************************\n");
    for(k=0;k<totalprocess;k++)
    {
        printf("PROCESS ID : %d\n",processid[k]);
        printf("ADMISSION TIME : %d\n",arrivaltime[k]);
        printf("LIFETIME : %d\n",bursttime[k]);
        printf("RESPONSE TIME : %d\n",responsetime[k]);
        printf("STATE : %d\n",processtate[k]);
        printf("ENDTIME : %d\n",endtime[k]);
        printf("TURN1AROUNDTIME : %d\n",endtime[k]-arrivaltime[k]);
        printf("\n************************************\n");
    }
}
