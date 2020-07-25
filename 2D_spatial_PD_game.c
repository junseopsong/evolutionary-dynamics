/*
Evolution of cooperation in spatial prisoner's dilemma game
Methods: 1) Death-Birth, 2) Imitating, or 3) Birth-Death

Jun-Seop Song
2012.07.11
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_N 50

int methodtype;
int runnum; // number of randomization
int n, map[MAX_N][MAX_N]; // n by n 2D lattice, 0:defector, 1:cooperator
int num[2]; // num[0]: number of defector, num[1]: number of cooperator
double b, c=1; // benefit, cost
int fixation_c; // number of fixation of cooperator
double w; // intensity of selection
double ft[MAX_N][MAX_N], sum;


double payoff(int x1, int y1, int x2, int y2)
{
    if(map[x1][y1]==1 && map[x2][y2]==1) return b-c;
    else if(map[x1][y1]==1 && map[x2][y2]==0) return -c;
    else if(map[x1][y1]==0 && map[x2][y2]==1) return b;
    else return 0;
}


double total_payoff(int x, int y)
{
    return payoff(x,y,(x+1)%n,y)+payoff(x,y,(x-1+n)%n,y)+payoff(x,y,x,(y+1)%n)+payoff(x,y,x,(y-1+n)%n);
}


void death_birth()
{
    int x, y;
    double p,f_c=0,f_d=0,rnd;
    
    x=rand()%n; y=rand()%n;
    
    if(map[(x+1)%n][y]==1) f_c += (1-w)+w*total_payoff((x+1)%n,y);
    else f_d += (1-w)+w*total_payoff((x+1)%n,y);

    if(map[(x-1+n)%n][y]==1) f_c += (1-w)+w*total_payoff((x-1+n)%n,y);
    else f_d += (1-w)+w*total_payoff((x-1+n)%n,y);
    
    if(map[x][(y+1)%n]==1) f_c += (1-w)+w*total_payoff(x,(y+1)%n);
    else f_d += (1-w)+w*total_payoff(x,(y+1)%n);
    
    if(map[x][(y-1+n)%n]==1) f_c += (1-w)+w*total_payoff(x,(y-1+n)%n);
    else f_d += (1-w)+w*total_payoff(x,(y-1+n)%n);
    
    p = f_c/(f_c+f_d);
    rnd = (double)rand()/(double)RAND_MAX;
    
    if(rnd < p)
    {
        num[map[x][y]]--;
        map[x][y]=1;
        num[map[x][y]]++;
    }
    else
    {
        num[map[x][y]]--;
        map[x][y]=0;
        num[map[x][y]]++;
    }
}


void imitating()
{
    int x, y;
    double p,f_c=0,f_d=0,rnd;
    
    x=rand()%n; y=rand()%n;
    
    if(map[x][y]==1) f_c += (1-w)+w*total_payoff(x,y);
    else f_d += (1-w)+w*total_payoff(x,y);
    
    if(map[(x+1)%n][y]==1) f_c += (1-w)+w*total_payoff((x+1)%n,y);
    else f_d += (1-w)+w*total_payoff((x+1)%n,y);
    
    if(map[(x-1+n)%n][y]==1) f_c += (1-w)+w*total_payoff((x-1+n)%n,y);
    else f_d += (1-w)+w*total_payoff((x-1+n)%n,y);
    
    if(map[x][(y+1)%n]==1) f_c += (1-w)+w*total_payoff(x,(y+1)%n);
    else f_d += (1-w)+w*total_payoff(x,(y+1)%n);
    
    if(map[x][(y-1+n)%n]==1) f_c += (1-w)+w*total_payoff(x,(y-1+n)%n);
    else f_d += (1-w)+w*total_payoff(x,(y-1+n)%n);
    
    p = f_c/(f_c+f_d);
    rnd = (double)rand()/(double)RAND_MAX;
    
    if(rnd < p)
    {
        num[map[x][y]]--;
        map[x][y]=1;
        num[map[x][y]]++;
    }
    else
    {
        num[map[x][y]]--;
        map[x][y]=0;
        num[map[x][y]]++;
    }
}


void change_sum1(int x, int y)
{
    sum -= (ft[x][y]+ft[(x+1)%n][y]+ft[(x-1+n)%n][y]+ft[x][(y+1)%n]+ft[x][(y-1+n)%n]);
}


void change_ft(int x, int y)
{
    ft[x][y] = (1-w) + w*total_payoff(x,y);
    ft[(x+1)%n][y] = (1-w) + w*total_payoff((x+1)%n,y);
    ft[(x-1+n)%n][y] = (1-w) + w*total_payoff((x-1+n)%n,y);
    ft[x][(y+1)%n] = (1-w) + w*total_payoff(x,(y+1)%n);
    ft[x][(y-1+n)%n] = (1-w) + w*total_payoff(x,(y-1+n)%n);
}


void change_sum2(int x, int y)
{
    sum += (ft[x][y]+ft[(x+1)%n][y]+ft[(x-1+n)%n][y]+ft[x][(y+1)%n]+ft[x][(y-1+n)%n]);
}


void birth_death()
{
    int i,j, select, check=0;
    double p=0, rnd;
    
    rnd = (double)rand()/(double)RAND_MAX;
    
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            p += ft[i][j];
            if(rnd <= p/sum)
            {
                select = rand()%4;
                
                if(select==0)
                {
                    change_sum1((i+1)%n,j);
                    
                    num[map[(i+1)%n][j]]--;
                    map[(i+1)%n][j]=map[i][j];
                    num[map[(i+1)%n][j]]++;
                    
                    change_ft((i+1)%n,j);
                    change_sum2((i+1)%n,j);
                }
                else if(select==1)
                {
                    change_sum1((i-1+n)%n,j);
                    
                    num[map[(i-1+n)%n][j]]--;
                    map[(i-1+n)%n][j]=map[i][j];
                    num[map[(i-1+n)%n][j]]++;
                    
                    change_ft((i-1+n)%n,j);
                    change_sum2((i-1+n)%n,j);
                }
                else if(select==2)
                {
                    change_sum1(i,(j+1)%n);
                    
                    num[map[i][(j+1)%n]]--;
                    map[i][(j+1)%n]=map[i][j];
                    num[map[i][(j+1)%n]]++;
                    
                    change_ft(i,(j+1)%n);
                    change_sum2(i,(j+1)%n);
                }
                else
                {
                    change_sum1(i,(j-1+n)%n);
                    
                    num[map[i][(j-1+n)%n]]--;
                    map[i][(j-1+n)%n]=map[i][j];
                    num[map[i][(j-1+n)%n]]++;
                    
                    change_ft(i,(j-1+n)%n);
                    change_sum2(i,(j-1+n)%n);
                }
                
                check=1;
                break;
            }
        }
        if(check) break;
    }
}


void process()
{
    int i,j;
    
    /* initial setting */
    num[1]=1; num[0]=n*n-1;
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++) map[i][j]=0;
    }
    map[0][0]=1;
    
    /* birth-death initial setting */
    if(methodtype==3)
    {
        sum=0;
        for(i=0;i<n;i++)
        {
            for(j=0;j<n;j++)
            {
                ft[i][j] = (1-w)+w*total_payoff(i,j);
                sum += ft[i][j];
            }
        }
    }
    
    while(num[0]!=0 && num[1]!=0)
    {
        if(methodtype==1) death_birth();
        else if(methodtype==2) imitating();
        else birth_death();
    }
    
    if(num[0]==0) fixation_c++;
}


int main()
{
    int i;
    
    printf("Evolutionary dynamics simulation (2012.07.11)\n\n");
    printf("Number of randomization : ");
    scanf("%d", &runnum);
    printf("Size of 2D lattice (N<50) : ");
    scanf("%d", &n);
    printf("Method (1:Death-Birth, 2:Imitating, 3:Birth-Death) : ");
    scanf("%d", &methodtype);
    printf("b/c : ");
    scanf("%lf", &b);
    printf("w (default: 0.01) : ");
    scanf("%lf", &w);
    
    srand((unsigned)time(NULL));
    
    for(i=1;i<=runnum;i++)
    {
        process();
        if(i%100000==0)
        {
            printf("%d    fixation probability : %lf\n", i,(double)fixation_c/(double)i);
        }
    }
    
    printf("\nFixation probability of cooperation : %lf\n", (double)fixation_c/(double)runnum);
    
    return 0;
}