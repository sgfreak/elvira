// For code minimalism im not making comments anymore xD
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
char **b,*f;int n,r,c,m,y,R,C;
void a(char *s,int i){
 b=realloc(b,(n+1)*sizeof(char*));
 if(i<n)memmove(b+i+1,b+i,(n-i)*sizeof(char*));
 b[i]=strdup(s);n++;
}
void d(){
 getmaxyx(stdscr,R,C);
 if(r<y)y=r;if(r>=y+R-1)y=r-R+2;
 erase();
 for(int i=0;i<R-1;i++)if(i+y<n)mvaddnstr(i,0,b[i+y],C);
 attron(A_REVERSE);
 mvprintw(R-1,0,"[%c] %s %d,%d (%d)",m?'I':'N',f,r+1,c+1,n);
 clrtoeol();attroff(A_REVERSE);move(r-y,c);
}
int main(int argc,char **argv){
 f=argc>1?argv[1]:"untitled";
 FILE *fp=fopen(f,"r");char l[4096];
 while(fp&&fgets(l,4096,fp)){l[strcspn(l,"\r\n")]=0;a(l,n);}
 if(fp)fclose(fp);if(!n)a("",0);
 initscr();raw();noecho();keypad(stdscr,1);
 while(1){
  d();int k=getch();
  if(!m){
   if(k=='q')break;if(k=='i')m=1;
   if(k=='a'){m=1;if(c<strlen(b[r]))c++;}
   if(k=='h'&&c>0)c--;if(k=='l'&&c<strlen(b[r]))c++;
   if(k=='j'&&r<n-1)r++;if(k=='k'&&r>0)r--;
   if(k=='x'&&strlen(b[r])>0)memmove(b[r]+c,b[r]+c+1,strlen(b[r])-c);
   if(k=='o'){a("",++r);c=0;m=1;}
   if(k=='G')r=n-1;if(k=='g'&&getch()=='g')r=0;
   if(k=='0')c=0;if(k=='$')c=strlen(b[r]);
   if(k=='d'&&getch()=='d'){
    free(b[r]);
    if(n>1){memmove(b+r,b+r+1,(n-r-1)*sizeof(char*));n--;if(r>=n)r=n-1;}
    else b[0]=strdup("");c=0;
   }
   if(k==':'){
    mvaddstr(R-1,0,":");clrtoeol();int ch=getch();
    if(ch=='w'){FILE *w=fopen(f,"w");for(int i=0;i<n;i++)fprintf(w,"%s\n",b[i]);fclose(w);}
    if(ch=='q')break;
   }
  }else{
   if(k==27)m=0;
   else if(k==KEY_BACKSPACE||k==127||k==8){
    if(c>0){memmove(b[r]+c-1,b[r]+c,strlen(b[r])-c+1);c--;}
    else if(r>0){
     int p=strlen(b[r-1]);b[r-1]=realloc(b[r-1],p+strlen(b[r])+1);
     strcat(b[r-1],b[r]);free(b[r]);
     memmove(b+r,b+r+1,(n-r-1)*sizeof(char*));n--;r--;c=p;
    }
   }else if(k==13||k==10){
    a(b[r]+c,r+1);b[r]=realloc(b[r],c+1);b[r][c]=0;r++;c=0;
   }else if(k>=32&&k<=126){
    int len=strlen(b[r]);b[r]=realloc(b[r],len+2);
    memmove(b[r]+c+1,b[r]+c,len-c+1);b[r][c++]=k;
   }
  }
  if(c>strlen(b[r]))c=strlen(b[r]);
 }
 endwin();return 0;
}
