#include <stdio.h>
#include <stdlib.h>

typedef struct choice                                   //structure to store info. about choices
{
    int insti;
    int branch;
    int choiceno;
    struct choice *nextchoice;
}choice;

typedef struct stulist                                  //structure to store data of students
{
    int rank;
    int insti;
    int bran;
    int status;
    int choiceno;
    choice *c;
    struct stulist *nextstu;
}stulist;

void take_choices(stulist *p)                           //function to take choices as input
{
    int k = 1,n = 1;
    choice *c = (choice*)malloc(sizeof(choice)),*f;
    f = c;
    printf("Enter a valid institute no. and branch no. for preference no. %d: ",n);
    scanf("%d%d",&f->insti,&f->branch);
    f->choiceno = 1;
    f->nextchoice = NULL;
    printf("Want to enter another preference(0 for no/1 for yes): ");
    scanf("%d",&k);
    n++;
    while(k)
    {
        f->nextchoice = (choice*)malloc(sizeof(choice));
        f = f->nextchoice;
        f->choiceno = n;
        printf("Enter a valid institute no. and branch no. for preference no. %d: ",n);
        scanf("%d%d",&f->insti,&f->branch);
        f->nextchoice = NULL;
        printf("Want to enter another preference(0 for no/1 for yes): ");
        scanf("%d",&k);
        n++;
    }
    p->c = c;
}

int** Round1(stulist *start,int instis,int brans,int maxstu)        //function to allot seat for round 1
{
    int **mat;
    mat = (int**)malloc((instis+1)*sizeof(int*));
    for(int i = 1;i <= instis;i++)
    {
        mat[i] = (int*)malloc((brans+1)*sizeof(int));
        for(int j = 1;j <= brans;j++)
            mat[i][j] = maxstu;
    }

    while(start != NULL)
    {
        start->insti = -1;
        start->bran = -1;
        start->choiceno = -1;
        choice *c = start->c;
        while(c != NULL)
        {
            if(mat[c->insti][c->branch])
            {
                mat[c->insti][c->branch]--;
                start->bran = c->branch;
                start->insti = c->insti;
                start->choiceno = c->choiceno;
                c->nextchoice = NULL;
                break;
            }
            c = c->nextchoice;
        }
        start = start->nextstu;
    }
    return mat;
}

void Round2(stulist *start, int **mat)                  //function to allot seat for round 2
{
    while(start != NULL)
    {
        printf("Enter status for rank %d: ",start->rank);
        scanf("%d",&start->status);
        while(start->insti == -1 && (start->status == 2 || start->status == 3))
        {
            if(start->status == 2)
                printf("You cannot freeze as nothing is alloted to you. You can either float or withdraw.\n");
            if(start->status == 3)
                printf("You cannot slide as nothing is alloted to you. You can either float or withdraw.\n");
            printf("Enter status for rank %d: ",start->rank);
            scanf("%d",&start->status);
        }
        if(start->status == 0)
        {
            if(start->insti != -1)
                mat[start->insti][start->bran]++;
            start = start->nextstu;
            continue;
        }

        if(start->status == 1)
        {
            choice *c = start->c;
            while(c != NULL && c->choiceno != start->choiceno)
            {
                if(mat[c->insti][c->branch])
                {
                    mat[c->insti][c->branch]--;
                    if(start->insti != -1)
                        mat[start->insti][start->bran]++;
                    start->bran = c->branch;
                    start->insti = c->insti;
                    start->choiceno = c->choiceno;
                    c->nextchoice = NULL;
                    break;
                }
                c = c->nextchoice;
            }
            start = start->nextstu;
            continue;
        }

        if(start->status == 2)
        {
            start = start->nextstu;
            continue;
        }

        if(start->status == 3)
        {
            choice *c = start->c;
            while(c->choiceno != start->choiceno || c != NULL)
            {
                if(mat[c->insti][c->branch] && start->insti == c->insti)
                {
                    mat[c->insti][c->branch]--;
                    mat[start->insti][start->bran]++;
                    start->bran = c->branch;
                    start->choiceno = c->choiceno;
                    c->nextchoice = NULL;
                    break;
                }
                c = c->nextchoice;
            }
            start = start->nextstu;
            continue;
        }
    }
}

void printc(stulist *start)                         //function to print alloted seats
{
    while(start != NULL)
    {
        if(start->status == 0)
        {
            printf("Rank %d has opted out of counseling process.\n",start->rank);
        }
        else
        {
            if(start->bran != -1)
                printf("Rank %d has been allotted preference no. %d which is Institute %d and Branch %d.\n",start->rank,start->choiceno,start->insti,start->bran);
            else
                printf("Rank %d has not been allotted any institute.\n",start->rank);
        }
        start = start->nextstu;
    }
}

int main()
{
    printf("Enter some necessary information for the counseling.\n");
    int i,instis,brans,stus,maxstu;
    printf("No. of institutions: ");
    scanf("%d",&instis);
    printf("No. of branches: ");
    scanf("%d",&brans);
    printf("Max capacity of a branch: ");
    scanf("%d",&maxstu);
    printf("Total no. of students: ");
    scanf("%d",&stus);
    printf("------------------------------------------\n");

    stulist **p;
    p = (stulist**)malloc((stus+1)*sizeof(stulist*));
    for(i = 1;i <= stus;i++)
        p[i] = NULL;

    printf("Enter the rank of students followed by their preferences.\n");
    i = 0;
    while(i++ < stus)
    {
        int r;
        printf("\nEnter the rank for student no. %d: ",i);
        scanf("%d",&r);
        while(r > stus || r < 1 || p[r] != NULL)
        {
            if(r > stus || r < 1)
                printf("Please enter a valid rank. A rank should be a natural no. less than or equal to total no. of students.\n");
            else
                printf("A candidate holding this rank already exist, please enter another rank.\n");
            printf("Enter the rank of student no. %d: ",i);
            scanf("%d",&r);
        }
        p[r] = (stulist*)malloc(sizeof(stulist));
        p[r]->rank = r;
        p[r]->status = 1;

        printf("Enter preference order for RANK %d\n",r);
        take_choices(p[r]);
    }
    stulist *start,*f;                                              //start is a linked list which contains all the data about students
    start = p[1];
    f = start;
    for(i = 2;i <= stus;i++)
    {
        f->nextstu = p[i];
        f = f->nextstu;
    }
    f->nextstu = NULL;

    int **mat = Round1(start,instis,brans,maxstu);                                              //function for round 1

    printf("------------------------------------------\n");
    printf("Results of Round 1\n");
    printc(start);                                                                              //printing results for round 1

    printf("------------------------------------------\n");
    printf("Process for round 2\nEnter 0 to withdraw, 1 to float, 2 to freeze, 3 to slide\n");
    Round2(start,mat);                                                                          //function for round 2

    printf("------------------------------------------\n");
    printf("Results of Round 2\n");
    printc(start);                                                                              //printing results for round 2
    return 0;
}
