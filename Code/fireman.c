#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<ncurses.h>
#include<unistd.h>
#include<poll.h>

int xp=25,yp=24,g1x=4,g1y=40,g2x = 166,g2y =5,out =0;    				 //Global Variables
char fl = 'R';
FILE *fp1,*fp2,*p1;
char map[182][50];


//struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };
int hit()										//To Check whether any key is pressed or not
{
  struct pollfd mypoll = { STDIN_FILENO, POLLIN|POLLPRI };
    char string[10];
    char ch;
    if( poll(&mypoll, 1, 50) )
    {
        ch = getchar();
	getchar();
	getchar();
       return 1;
    }
    else
    {
        return 0;
    }

  
}


struct path
{
   int x, y;
   struct path *next;
}*head1,*head2;


void createpath()									//To Create path for Ghosts using Circular Linked Lists
{
 fp1 = fopen("path1.txt","r");
 fp2 = fopen("path2.txt","r");
 int x1,y1,x2,y2;
head1 = (struct path*)malloc(sizeof(struct path));
head2 = (struct path*)malloc(sizeof(struct path));
head1->x = g1x;
head1->y = g1y;
head1->next = NULL;
head2->x = g2x;
head2->y = g2y;
head2->next = NULL;

struct path *temp = head1;

while(!feof(fp1))
 {fscanf(fp1,"%d%d",&x1,&y1);
  
 temp->next = (struct path*)malloc(sizeof(struct path));
  temp = temp->next;
  temp->x = x1;
  temp->y = y1;
  temp->next = NULL;
}
temp->next = head1;

temp = head2;
while(!feof(fp2))
 {fscanf(fp2,"%d%d",&x2,&y2);
  
 temp->next = (struct path*)malloc(sizeof(struct path));
 temp = temp->next; 
 temp->x = x2;
  temp->y = y2;
  temp->next = NULL;


}
 
temp->next = head2;
}


void gotoxy(int x,int y)								//Function to go to a particular position (x,y) on the screen
{	
    printf("%c[%d;%df",0x1B,y,x);
	
}

void callGhost(){									//To update the position of ghosts
	gotoxy(g1x,g1y);
	printf(" ");
	gotoxy(g2x,g2y);
	printf(" ");
	head1 = head1->next;
	head2 = head2->next;
	g1x = head1->x;
	g1y = head1->y;
	g2x = head2->x;
	g2y = head2->y;

	gotoxy(g1x,g1y);
	printf("🌊️");
	gotoxy(g2x,g2y);
	printf("🌊️");

}
int crash()										//To check whether fireman crashed the wall or the ghosts
{
   if(map[xp][yp] == '*' || (xp == g1x && yp == g1y) || (xp == g2x && yp == g2y))
    return 1;
   else return 0;
}
void obstacle()										//To create wall
{
    p1=fopen("obstacle.txt", "r");
    int x, y;
   while(!feof(p1)) 
   { fscanf(p1,"%d%d",&x,&y);
     gotoxy(x,y);
     printf("⏸");
     map[x][y]='*';
   }  
}
void updatePos()									//To update the position of Fireman
{
int ox=xp, oy= yp;

if(!hit())										//Update position automatically if key not hit

switch(fl){
case 'U':if(yp!=2)
	    yp--;break;
case 'D':if(yp!=50)
	    yp++;break;
case 'R':if(xp!=182)
	    xp++;break;
case 'L':if(xp!=3)
	    xp--;  break;
}



else											//Update position if arrow key is pressed

	if(getchar() =='\033'){

	getchar();
	switch(getchar()){
	case 'A':fl = 'U';
		 if(yp!=2)
		   yp--;break;
	case 'B':fl = 'D';
		 if(yp!=50)
		  yp++;break;
	case 'C': fl = 'R';
		 if(xp!=182)
		   xp++;break;
	case 'D':fl = 'L';
        	 if(xp!=3)
		    xp--;  break;
	}
   }

//usleep(20000);

if(!crash()){										//If not crashed update position
gotoxy(ox, oy);
printf(" ");
gotoxy(xp,yp);
printf("🔥");
callGhost();
}
else
{ out = 1;										//Set out = 1 if fireman crashed 
}


}
void main()
{
initscr();
createpath();
noecho();
curs_set(FALSE);
obstacle();
gotoxy(60,40);
printf("PLAY LONGER\nSCORE HIGHER");
gotoxy(xp,yp);
printf("🔥");

gotoxy(g1x,g1y);
printf("🌊️");
gotoxy(g2x,g2y);
printf("🌊️");

clock_t start,end;
start = clock();
while(!out){										//Game Over :( if out i.e fireman crashed
updatePos();

end = clock();
double score = ((double)(end-start))/CLOCKS_PER_SEC * 1000;
gotoxy(60,30);
printf("YOU SCORED: %lf",score);
}
gotoxy(60,35);
printf("Game Over\t Press Q to Quit");
while(getchar()!='q');
endwin();
}
