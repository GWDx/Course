#include<math.h>
#include<time.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#define VV 2
#define XX 41
#define YY 20
#define LL 100
#define NN 3000
int DD;

struct snake{
	int x[LL+5],y[LL+5],u[7],f,l,t,w;
	double m;
};

struct bomb{
	int x,y,f,v,m;//m-bymineQ
};

struct snake a,b;
struct bomb c[NN+5];
int n0,n,h,tt,mode,se[7]={0,1,0,0,1};
int s[XX+4][YY+5],t[XX+4][YY+5];
int dx[5]={0,1,0,-1},dy[5]={1,0,-1,0};
double ppp[5][201][101][61];
#define pp(f,x,y,l) ppp[f][x+100][y+50][l+10]
#define modXX(x) x<0?XX-1:x==XX?0:x
#define modYY(y) y<0?YY-1:y==YY?0:y

void gotoxy(int x, int y)
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(handle, pos);
}

int win(int u,int v)
{
	int q[5]={0,'!','#'};
	gotoxy(0,YY+5);
	if(u==0)
		printf("\n%dend\nTab: 继续\nR:返回",u,q[v]);
	  else
	{
		printf("\n%d win(%c)\nTab: 继续\nR:返回",u,q[v]);
		if(u==1)
			a.w++;
		  else
			b.w++;
	}
	while(1)
	{
		Sleep(50);
		if(GetAsyncKeyState(VK_TAB))
			return 1;
		if(GetAsyncKeyState('R'))
			return 0;
	}
}

void pri();
int begi();
void reset();

void help();
void sett();
void clea();

int mine(int x,int y,int f);
int ibomb();
int mbomb();

void readsave();
void writesave();
double p(int f,int x,int y,int l)
{
	int lf,rf;
	if(pp(f,x,y,l)>=0)
		return(pp(f,x,y,l));
	if(x==0&&y==1&&l>=1)
		return(pp(f,x,y,l)=0);
	if(x==0&&y==0&&l>=0)
		return(pp(f,x,y,l)=0);
	if(x==0&&y==-1&&f==2&&l>=-1)
		return(pp(f,x,y,l)=0);
	if(l<=0||y<=0)
		return(pp(f,x,y,l)=1);
//	if(abs(x)>(y+1)/2)
//		return(pp(f,x,y,l)=1);
	lf=(f+1)&3;
	rf=(f+3)&3;
	pp(f,x,y,l)=.6*p(f,x+dx[f],y+dy[f]-2,l-2)+
				.2*p(lf,x+dx[lf],y+dy[lf]-2,l-2)+
				.2*p(rf,x+dx[rf],y+dy[rf]-2,l-2);
	return(pp(f,x,y,l));
}

void calp()
{
	int f,x,y,l;
	for(f=0;f<4;f++)
		for(x=-2*XX-15;x<=2*XX+15;x++)
			for(y=-XX-5;y<=XX+5;y++)
				for(l=-5;l<=XX+5;l++)
					pp(f,x,y,l)=-2;
	for(f=0;f<4;f++)
		for(x=-2*XX-13;x<=2*XX+13;x++)
			for(y=-XX-3;y<XX+3;y++)
				for(l=-3;l<XX+3;l++)
					p(f,x,y,l);
	if(se[3])
		for(f=0;f<4;f++)
			for(x=-2*XX-13;x<=2*XX+13;x++)
				for(y=-XX-3;y<XX+3;y++)
					for(l=-3;l<XX+3;l++)
						pp(f,x,y,l)*=1-1./(x*x+y*y+4);

	for(f=0;f<4;f++)
		for(x=-2*XX-13;x<=2*XX+13;x++)
			for(y=-XX-3;y<XX+3;y++)
				for(l=-3;l<XX+3;l++)
					if(x!=0)
					{
						if(pp(f,x,y,l)>.95)
							pp(f,x,y,l)=1;
						else pp(f,x,y,l)/=.95;
					}
}

double fp(int v,int f,int x,int y)
{
	int i,ddx,ddy,l,dex,dey,cx,cy;
	double ans=1;
	for(i=n0;i<n;i++)
		if(c[i].v&&!c[i].m)
		{
			cx=c[i].x+v*dx[c[i].f];
			cy=c[i].y+v*dy[c[i].f];
//			cx=c[i].x;
//			cy=c[i].y;
			dex=x-cx;
			dey=y-cy;
			switch(c[i].f)
			{
				case 0:
					ddx=dex;
					ddy=dey;
					l=YY-1-cy;
					break;
				case 1:
					ddx=-dey;
					ddy=dex;
					l=XX-1-cx;
					break;
				case 2:
					ddx=-dex;
					ddy=-dey;
					l=cy;
					break;
				case 3:
					ddx=dey;
					ddy=-dex;
					l=cx;
					break;
			}
			ans*=p((f-c[i].f+4)&3,ddx,ddy,l);
		}
	return(ans);
}

int va(int f,int x,int y)
{
	int v=s[modXX(x+dx[f])][modYY(y+dy[f])];
	return(v!=1&&v!=2&&v!=6);
}

int vv[49]={0,0,0,0,0,9,8,8,7,7,7,6,6,6,6,5,5,5,5,5,4,4,4,4,4,4,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,1,1,1,1,1,1};
int ff(int f,int x,int y,double m,int ff,int xx,int yy)
{
	double fp1,fp2,fp3,fq0,fq1,fq2,fq3,fq,max;
	int x1,x2,x3,y1,y2,y3,lf,rf,lff,rff,opt,va1,va2,va3,u;
	lf=(f+1)&3;
	rf=(f+3)&3;
	lff=(ff+1)&3;
	rff=(ff+3)&3;
	
	c[n].f=ff;
	c[n].x=xx+dx[ff];
	c[n].y=yy+dy[ff];
	c[n].v=1;
	c[n].m=0;
	n++;
	if(va1=va(f,x,y))fp1=fp(1,f,modXX(x+dx[f]),modYY(y+dy[f])); else fp1=0;
	if(va2=va(lf,x,y))fp2=fp(1,lf,modXX(x+dx[lf]),modYY(y+dy[lf])); else fp2=0;
	if(va3=va(rf,x,y))fp3=fp(1,rf,modXX(x+dx[rf]),modYY(y+dy[rf])); else fp3=0;
	n--;
	
	x1=modXX(xx+dx[ff]);
	x2=modXX(xx+dx[lff]);
	x3=modXX(xx+dx[rff]);
	y1=modYY(yy+dy[ff]);
	y2=modYY(yy+dy[lff]);
	y3=modYY(yy+dy[rff]);
	fq1=fp(2,ff,x1,y1);
	fq2=fp(2,lff,x2,y2);
	fq3=fp(2,rff,x3,y3);
	fq0=.6*fq1+.2*fq2+.2*fq3;
	
	c[n].f=f;
	c[n].x=x+2*dx[f];
	c[n].y=y+2*dy[f];
	c[n].v=1;
	c[n].m=0;
	n++;
	fq1=fp(2,ff,x1,y1);
	fq2=fp(2,lff,x2,y2);
	fq3=fp(2,rff,x3,y3);
	fq=.6*fq1+.2*fq2+.2*fq3;
	n--;
	
	if(abs(fp2-fp3)<.03&&fp2>.3&&fp3>.3)
		if(opt=rand()&1)
			opt=1,max=fp2;
	  	  else
			opt=3,max=fp3;
	  else
		if(fp2>=fp3)
			opt=1,max=fp2;
		  else
			opt=3,max=fp3;

	if(fp1>=max)
		if((fq0-fq)*sqrt(m)>.2&&m>=1)
			opt=4;
		  else
		{
			if(fp1-max>.03||rand()&7)
				opt=2;
		}
	  else
		if(fq0-fq>.3*(max-fp1)&&(fq0-fq)*sqrt(m)>.2&&m>=1)
			opt=4;
	if(!va1&&max<0.3&&x+dx[f]>=0&&x+dx[f]<XX&&y+dy[f]>=0&&y+dy[f]<YY&&(m>=2||(m>=1&&!va2&&!va3)))
		opt=4;
	if(se[4]&&opt==2&&m>=5&&(u=(h==1?a.l:b.l))>4)
		if(!(rand()%vv[u]))
			opt=5;
	if(mode+h==4)gotoxy(0,YY+9);else gotoxy(0,YY+11);
	printf("%.3f\t%.3f\t%.3f\t\n%.3f\t%.3f\t%d",fp1,fp2,fp3,fq0,fq,opt);
	return(opt);
}

int main(){
	int op1[6]={0,1,0,0,0,0};
	int op2[6]={0,0,0,1,0,0};
	int op3[6]={0,0,0,0,1,0};
	int op4[6]={0,0,0,0,0,1};
	int opt,i,j,k,rx,ry,v,*p;
	DWORD t1,t2,t0;
	a.w=b.w=0;
	srand(time(NULL));
	
C:	GetAsyncKeyState('R');
	mode=begi();
	
A:	GetAsyncKeyState(VK_TAB);
	h=1;
	clea();
	reset();
	while(!mode)
	{
		readsave();
		if(mode)
			break;
		if(mode=begi())
			reset();
	}
	tt=0;
	t0=t1=GetTickCount();
	calp();
	GetAsyncKeyState('A');
	GetAsyncKeyState('D');
	GetAsyncKeyState('W');
	GetAsyncKeyState('S');
	GetAsyncKeyState(VK_LEFT);
	GetAsyncKeyState(VK_RIGHT);
	GetAsyncKeyState(VK_UP);
	GetAsyncKeyState(VK_DOWN);
	if(h==2)goto B;
	while(1){
		h=1;
		t2=GetTickCount();
		if(GetAsyncKeyState('4'))
			writesave();
		if(GetAsyncKeyState('R'))
		{
			writesave();
			goto C;
		}
		if(mode<=2)
		{
			a.u[1]=GetAsyncKeyState('A');
			a.u[2]=GetAsyncKeyState('D');
			a.u[3]=GetAsyncKeyState('W');
			a.u[4]=GetAsyncKeyState('S');
		}
		else
		{
			opt=ff(a.f,a.x[0],a.y[0],a.m,b.f,b.x[0],b.y[0]);
			a.u[1]=op1[opt];
			a.u[2]=op2[opt];
			a.u[3]=op3[opt];
			a.u[4]=op4[opt];
		}
		if(a.u[1])
			a.f=(a.f+1)&3;
		if(a.u[2])
			a.f=(a.f+3)&3;
		if(a.u[4]&&a.m>=1&&a.l>=1)
		{
			a.m-=1;
			t[a.x[a.l]][a.y[a.l]]++;
			s[a.x[a.l]][a.y[a.l]]=0;
			a.l-=1;
		}
		if(a.u[3]&&a.m>=1)
		{
			a.t=1;
			a.m-=1;
			c[n].v=1;
			c[n].m=0;
			c[n].f=a.f;
			c[n].x=a.x[0];
			c[n].y=a.y[0];
			if(v=ibomb())
				if(win(v,1))
					goto A;
				  else
					goto C;
		}		
		else
			a.t=0;
		
		if(v=mbomb())
			if(win(v,1))
				goto A;
			  else
				goto C;
		s[a.x[0]][a.y[0]]=1;
		
		rx=modXX(a.x[0]+dx[a.f]);
		ry=modYY(a.y[0]+dy[a.f]);
		p=&s[rx][ry];
		switch(*p)
		{
			case 3:
				a.l++;
				for(i=a.l;i>0;i--)
				{
					a.x[i]=a.x[i-1];
					a.y[i]=a.y[i-1];
				}
				a.x[0]=rx;
				a.y[0]=ry;
				*p=1;
				break;
			case 5:
				a.m+=4;
			default:
				s[a.x[a.l]][a.y[a.l]]=0;
				for(i=a.l;i>0;i--)
				{
					a.x[i]=a.x[i-1];
					a.y[i]=a.y[i-1];
				}
				   //a.x[0]+dx[a.f]<0 || a.x[0]+dx[a.f]>=XX||
				   //a.y[0]+dy[a.f]<0 || a.y[0]+dy[a.f]>=YY||
				if(!va(a.f,a.x[0],a.y[0]))
				{
					if(rx==a.x[0]&&ry==a.y[0])
						v=win(0,2);
					  else
						v=win(2,2);
					if(v)
						goto A;
					  else
						goto C;
				}
				a.x[0]=rx;
				a.y[0]=ry;
				*p=1;
		}
		
		gotoxy(0,0);
		tt++;
		pri();
		t1=GetTickCount();
		if(DD+t2>t1)
			Sleep(DD-t1+t2);
//		while(!GetAsyncKeyState(VK_SPACE))Sleep(50);
		
		//b
		B:h=2;
		t2=GetTickCount();
		if(GetAsyncKeyState('4'))
			writesave();
		if(GetAsyncKeyState('R'))
		{
			writesave();
			goto C;
		}
		if(mode==1)
		{
			b.u[1]=GetAsyncKeyState(VK_LEFT);
			b.u[2]=GetAsyncKeyState(VK_RIGHT);
			b.u[3]=GetAsyncKeyState(VK_UP);
			b.u[4]=GetAsyncKeyState(VK_DOWN);
		}
		else
		{
			opt=ff(b.f,b.x[0],b.y[0],b.m,a.f,a.x[0],a.y[0]);
			b.u[1]=op1[opt];
			b.u[2]=op2[opt];
			b.u[3]=op3[opt];
			b.u[4]=op4[opt];
		}
		if(b.u[1])
			b.f=(b.f+1)&3;
		if(b.u[2])
			b.f=(b.f+3)&3;
		if(b.u[4]&&b.m>=1&&b.l>=1)
		{
			b.m-=1;
			t[b.x[b.l]][b.y[b.l]]++;
			s[b.x[b.l]][b.y[b.l]]=0;
			b.l-=1;
		}
		if(b.u[3]&&b.m>=1)
		{
			b.t=1;
			b.m-=1;
			c[n].v=1;
			c[n].m=0;
			c[n].f=b.f;
			c[n].x=b.x[0];
			c[n].y=b.y[0];
			if(v=ibomb())
				if(win(v,1))
					goto A;
				  else
					goto C;
		}
		else
			b.t=0;
		
		if(v=mbomb())
			if(win(v,1))
				goto A;
			  else
				goto C;
		s[b.x[0]][b.y[0]]=2;
		
		rx=modXX(b.x[0]+dx[b.f]);
		ry=modYY(b.y[0]+dy[b.f]);
		p=&s[rx][ry];
		switch(*p)
		{
			case 3:
				b.l++;
				for(i=b.l;i>0;i--)
				{
					b.x[i]=b.x[i-1];
					b.y[i]=b.y[i-1];
				}
				b.x[0]=rx;
				b.y[0]=ry;
				*p=2;
				break;
			case 5:
				b.m+=4;
			default:
				s[b.x[b.l]][b.y[b.l]]=0;
				for(i=b.l;i>0;i--)
				{
					b.x[i]=b.x[i-1];
					b.y[i]=b.y[i-1];
				}
				   //b.x[0]+dx[b.f]<0 || b.x[0]+dx[b.f]>=XX||
				   //b.y[0]+dy[b.f]<0 || b.y[0]+dy[b.f]>=YY||
				if(!va(b.f,b.x[0],b.y[0])){
					if(rx==a.x[0]&&ry==a.y[0])
						v=win(0,2);
					  else
						v=win(1,2);
					if(v)
						goto A;
					  else
						goto C;
				}
				   
/*				{
					if(!va(b.f,b.x[0],b.y[0]))
					if(b.x[0]+dx[b.f]==a.x[0] && b.y[0]+dy[b.f]==a.y[0])
					{
						printf("end");
						while(!GetAsyncKeyState(VK_SPACE))Sleep(50);Sleep(500);goto A;
					}
				      else
					{
						printf("\n1 win(2) \n");
						a.w++;
						while(!GetAsyncKeyState(VK_SPACE))Sleep(50);Sleep(500);goto A;
	//			 	}
					return 0;
				}
*/				b.x[0]=rx;
				b.y[0]=ry;
				*p=2;
		}
		
		
		tt++;
		while(n0!=n&&!c[n0].v&&!c[n0].m)
			n0++;
		
		if(!(tt&7))
		{
			do{
				rx=rand()%XX;
				ry=rand()%YY;
			}while(s[rx][ry]!=0&&rand()%4==0);
			if(!s[rx][ry])
				s[rx][ry]=3;//c
		}
		
		if(!(tt&15))
		{
			do{
				rx=rand()%XX;
				ry=rand()%YY;
			}while(s[rx][ry]!=0&&rand()%4==0);
			if(!s[rx][ry])
				s[rx][ry]=5;//e
		}
		
		if(se[2]&&!(tt%24))
		{
			rx=rand()%XX;
			ry=rand()%YY;
			t[rx][ry]++;
		}
		gotoxy(0,0);
		pri();
		t1=GetTickCount();
		if(DD+t2>t1)
			Sleep(DD+t2-t1);
//		while(!GetAsyncKeyState(VK_SPACE))Sleep(50);
	}
	return 0;
}

void pri()
{
	int i,j;
	int p[10]={' ','1','2','c','!','e','#'};
	int q[3]={' ','!'};
	printf("X");
	for(i=0;i<XX;i++)
		printf("X");
	printf("X\n");
	for(j=0;j<YY;j++)
	{
		printf("X");
		for(i=0;i<XX;i++)
			printf("%c",p[s[i][j]]);
		printf("X%d",j%10);
		printf("  X");
		for(i=0;i<XX;i++)
		{
			if(t[i][j])
				printf("*");
			  else
				printf(" ");
		}
		printf("X\n");
	}
	for(i=0;i<=XX;i++)
		printf("X");
	printf("X\n ");
	for(i=0;i<XX;i++)
		printf("%d",i%10);
		printf("     ");
	for(i=0;i<XX;i++)
		printf("%d",i%10);
	printf("\n1:%-4.1f %c %-3d %-3d %-2d\n2:%-4.1f %c %-3d %-3d %-2d\n",a.m,q[a.t],a.x[0],a.y[0],a.w,b.m,q[b.t],b.x[0],b.y[0],b.w);
	printf("%5d%5d%6d   ",n0,n,tt);
	for(i=1;i<5;i++)
		printf("%d",se[i]);
}

void help()
{
	int i,j,t=0;
	int x[19]={2,3,3,3,3,3,4,4,2,2,2,2,2,2,2,2,2,2,2};
	int y[19]={1,2,3,4,5,6,8,9,11,12,13,14,15,16,18,19,20,0};
	char w[19][60]={
		"按键：", 
		"\t\t玩家 1\t玩家 2",
		"\t左转\tA\t<",
		"\t右转\tD\t>",
		"\t开炮\tW\t\036",
		"\t放地雷\tS\t\037",
		"4: 存档",
		"R: 保存并返回",
		"炮弹(!)：能将蛇身打断，并将后半截变为",
		"         障碍物(#)。炮弹可以相互抵消",
		"地雷(*)：放地雷消耗1个单位长度和1个单位",
		"         炮弹。地雷遇到炮弹后会向其他",
		"         三个方向发射炮弹",
		"食物(c 或 e)：能增加蛇的长度或炮弹量", 
		"胜负判定：蛇头被炮弹击中判负，",
		"          撞到蛇或墙也判负"
	};
	GetAsyncKeyState('R');
	clea();
	gotoxy(0,0);
	for(i=0;i<=XX;i++)
		printf("X");
	printf("X\n");
	for(j=1;j<=YY;j++)
	{
		printf("X");
		if(y[t]==j)
		{
			gotoxy(x[t],j);
			printf("%s",w[t]);
			t++;
		}
		gotoxy(XX+1,j);
		printf("X\n");
	}
	for(i=0;i<=XX;i++)
		printf("X");
	printf("X\n\nR: 返回");
	while(!GetAsyncKeyState('R'));
		Sleep(50);
}

void sett()
{
	int i,j,t;
	int x[15]={2,3,3,3,3,3,3,3,3,3,3};
	int y[15]={2,3,4,5,6,7,9,10,11,12,0};
	int v[15]={0,0,1,1,1,1,2,2,2,2};
	int u[7][7]={
		{0,1,0,0,1},
		{0,1,1,1,0},
		{0,0,1,1,0},
		{0,0,0,0,0}};
	char w[15][60]={
		"设置",
		"                         当前状态",
		"1: 蛇被打断炮弹数量按比例减少 ",
		"2: 自动生成地雷               ",
		"3: 机器蛇“防地雷”模式       ",
		"4: 允许机器蛇放地雷           ",
		"预设模式 A: ", 
		"         B: ",
		"         C: ", 
		"         D: "
	};
	GetAsyncKeyState('R');
	GetAsyncKeyState('A');
	GetAsyncKeyState('B');
	GetAsyncKeyState('C');
	GetAsyncKeyState('D');
	clea();
E:	t=0;
	gotoxy(0,0);
	for(i=0;i<=XX;i++)
		printf("X");
	printf("X\n");
	for(j=1;j<=YY;j++)
	{
		printf("X");
		if(y[t]==j)
		{
			gotoxy(x[t],j);
			printf("%s",w[t]);
			switch(v[t])
			{ 
			case 1:
				printf("%d",se[t-1]);
				break;
			case 2:
				for(i=1;i<5;i++)
					printf("%d ",u[t-6][i]);
			} 
			t++;
		}
		gotoxy(XX+1,j);
		printf("X\n");
	}
	for(i=0;i<=XX;i++)
		printf("X");
	printf("X\n\nR: 返回");
	while(!GetAsyncKeyState('R'))
	{
		Sleep(50);
		for(i=1;i<5;i++)
			if(GetAsyncKeyState(i+'0'))
			{
				Sleep(10);
				se[i]=!se[i];
			 	goto E;
			}
		for(i=0;i<4;i++)
			if(GetAsyncKeyState(i+'A'))
			{
				Sleep(10);
				for(j=1;j<5;j++)
					se[j]=u[i][j];
			 	goto E;
			}
	}
}

int begi()	
{
	int i,j,t;
	int x[9]={15,14,14,14,14,14,14,21};
	int y[9]={5,8,10,11,13,16,17,20};
	char w[9][30]={
		"Snake Trouble",
		"1: 双人模式",
		"2: 单人（困难）",
		"3: 双机器",
		"4: 读取存档",
		"Shift: 帮助",
		"Ctrl : 设置",
		"gwdx@mail.ustc.edu.cn" 
	};
D:	GetAsyncKeyState('1');
	GetAsyncKeyState('2');
	GetAsyncKeyState('3');
	GetAsyncKeyState('4');
	GetAsyncKeyState(VK_CONTROL);
	GetAsyncKeyState(VK_SHIFT);
	t=0;
	clea();
	gotoxy(0,0); 
	for(i=0;i<=XX;i++)
		printf("X");
	printf("X\n");
	for(j=1;j<=YY;j++)
	{
		printf("X");
		if(y[t]==j)
		{
			gotoxy(x[t],j);
			printf("%s",w[t]);
			t++;
		}
		gotoxy(XX+1,j);
		printf("X\n");
	}
	for(i=0;i<=XX;i++)
		printf("X");
	printf("X\n");
	while(1)
	{
		if(GetAsyncKeyState('1'))
		{
			DD=100;
			return 1;
		}
		if(GetAsyncKeyState('2'))
		{
			DD=100;
			return 2;
		}
		if(GetAsyncKeyState('3'))
		{
			DD=10;
			return 3;
		}
		if(GetAsyncKeyState('4'))
			return 0;
		if(GetAsyncKeyState(VK_CONTROL))
		{
			sett();
			goto D;
		}
		if(GetAsyncKeyState(VK_SHIFT))
		{
			help();
			goto D;
		}
		Sleep(50);
	}
}

void clea()
{
	int i,j;
	for(i=0;i<40;i++)
		for(j=0;j<=90;j++)
		{
			gotoxy(j,i);
			printf(" ");
		}
}

int mine(int x,int y,int f)
{
	int v,i,a,b;
	if(t[x][y])
	{
		t[x][y]--;
		c[n].v=1;
		c[n].m=1;
		c[n].f=(f+1)&3;
		c[n].x=x;
		c[n].y=y;
		n++;
		c[n].v=1;
		c[n].m=1;
		c[n].f=(f+3)&3;
		c[n].x=x;
		c[n].y=y;
		n++;
/*		if((rand()&3))
		{
			v=2;
			for(i=0;i<v;i++)
			{
				c[n].v=1;
				c[n].m=1;
				c[n].f=ccc[f][i];
				c[n].x=x;
				c[n].y=y;
				n++;
			}
		}
		  else
			{
				v=2;
				a=rand()%3;
				do{ 
				b=rand()%3;
				}while(a==b); 
				c[n].v=1;
				c[n].m=1;
				c[n].f=ccc[f][a];
				c[n].x=x;
				c[n].y=y;
				n++;
				c[n].v=1;
				c[n].m=1;
				c[n].f=ccc[f][b];
				c[n].x=x;
				c[n].y=y;
				n++;
			}*/
		return 1;
	}
	return 0;
}

void reset()
{
	int i,j,rx,ry;
	n0=n=0;
	for(j=0;j<YY;j++)
		for(i=0;i<XX;i++)
			s[i][j]=t[i][j]=0;
	
	a.x[0]=rand()%XX;
	a.y[0]=rand()%YY;
	a.f=rand()%4;
	a.l=3;
	a.m=15.0;
	s[a.x[0]][a.y[0]]=1;
	
	do{
		b.x[0]=rand()%XX;
	}while(abs(b.x[0]-a.x[0])<=XX/3 || abs(b.x[0]-a.x[0])>=2*XX/3);
	do{
		b.y[0]=rand()%YY;
	}while(abs(b.y[0]-a.y[0])<=YY/3 || abs(b.y[0]-a.y[0])>=2*YY/3);
	do{
		b.f=rand()%4;
	}while(b.f%2==a.f%2);
	b.l=3;
	b.m=15.0;
	s[b.x[0]][b.y[0]]=2;
	
	for(i=1;i<=3;i++)
	{
		do{
			rx=rand()%XX;
			ry=rand()%YY;
		}while(s[rx][ry]!=0);
		s[rx][ry]=3;//c
	}
	for(i=1;i<=2;i++)
	{
		do{
			rx=rand()%XX;
			ry=rand()%YY;
		}while(s[rx][ry]!=0);
		s[rx][ry]=5;//e
	}
}

int ibomb()
{
	int j,k,v,*p;
/*	for(i=1;i<=VV;i++)
		if(c[n].v)
		{
*/			c[n].x+=dx[c[n].f];
			c[n].y+=dy[c[n].f];
			if(c[n].x<0||c[n].x>=XX||
			   c[n].y<0||c[n].y>=YY)
			{
				c[n].v=0;
				return 0;
			}
			p=&s[c[n].x][c[n].y];
			switch(*p)
			{
			case 1:
				*p=0;
				c[n].v=0;
				for(j=0;j<=a.l;j++)
					if(c[n].x==a.x[j]&&c[n].y==a.y[j])
						break;
				for(k=j+1;k<=a.l;k++)
					s[a.x[k]][a.y[k]]=6;
				if(se[1])a.m=a.m*j/(a.l+1);
				a.l=j-1;
				if(j==0)
					return 2;
				break;
			case 2:
				*p=0;
				c[n].v=0;
				for(j=0;j<=b.l;j++)
					if(c[n].x==b.x[j]&&c[n].y==b.y[j])
						break;
				for(k=j+1;k<=b.l;k++)
					s[b.x[k]][b.y[k]]=6;
				if(se[1])b.m=b.m*j/(b.l+1);
				b.l=j-1;
				if(j==0)
					return 1;
				break;
			case 6:
				*p=0;
				c[n].v=0;
				break;
			case 4:
				*p=0;
				c[n].v=0;
				for(j=n0;j<n;j++)
					if(c[j].v&&c[n].x==c[j].x&&c[n].y==c[j].y&&c[j].f!=c[n].f)
						c[j].v=0;
				break;
			}
			if(c[n].v)
				mine(c[n].x,c[n].y,c[n].f);
/*			if(v&&mine(c[n].x,c[n].y,c[n].f))
				c[n].v=0;
		}
*/
	n++;
	return 0;
}

int msnake()
{
	
}
 
int mbomb()
{
	int i,j,k,*p,rx,ry;
//	for(j=1;j<=VV/2;j++)
	for(i=n0;i<n;i++)
	{
		if(c[i].v&&!c[i].m)
		{
			rx=c[i].x+dx[c[i].f];
			ry=c[i].y+dy[c[i].f];
			s[c[i].x][c[i].y]=0;
			if(c[i].x==a.x[0]&&c[i].y==a.y[0]&&!a.t&&(c[i].f-a.f+4)%4==2)
				return 2;
			if(c[i].x==b.x[0]&&c[i].y==b.y[0]&&!b.t&&(c[i].f-b.f+4)%4==2)
				return 1;
			if(rx<0||rx>=XX||
			   ry<0||ry>=YY)
			{
				c[i].v=0;
				continue;
			}
			p=&s[rx][ry];
			switch(*p)
			{
				case 1:
					*p=0;
					c[i].v=0;
					for(j=0;j<=a.l;j++)
						if(rx==a.x[j]&&ry==a.y[j])
							break;
					for(k=j+1;k<=a.l;k++)
						s[a.x[k]][a.y[k]]=6;
					if(se[1])a.m=a.m*j/(a.l+1);
					a.l=j-1;
					if(j==0)
						return 2;
					break;
				case 2:
					*p=0;
					c[i].v=0;
					for(j=0;j<=b.l;j++)
						if(rx==b.x[j]&&ry==b.y[j])
							break;
					for(k=j+1;k<=b.l;k++)
						s[b.x[k]][b.y[k]]=6;
					if(se[1])b.m=b.m*j/(b.l+1);
					b.l=j-1;
					if(j==0)
						return 1;
					break;
				case 6:
					*p=0;
					c[i].v=0;
					break;
				case 4:
					*p=0;
					c[i].v=0;
					for(j=n0;j<n;j++)
						if(c[j].v&&rx==c[j].x&&ry==c[j].y&&c[j].f!=c[i].f)
							c[j].v=0;
					break;
			}
			if(c[i].v)
				mine(rx,ry,c[i].f);
//			if(c[i].v&&mine(rx,ry,c[i].f))
//				c[i].v=0;
			c[i].x=rx;
			c[i].y=ry;
		}
		if(c[i].m)
			c[i].m=0;
	}
	for(i=n0;i<n;i++)
		if(c[i].v)
			s[c[i].x][c[i].y]=4;
	return 0;
}

void readsave()
{
	char ch;
	int i,j,hh;
	FILE*f=fopen("save.txt","r");
	if(f==NULL)
	{
		gotoxy(0,YY+2);
		printf("文件未找到");
		Sleep(300);
		return;
	}
	fscanf(f,"%d%d%d\n",&mode,&h,&tt);
	for(i=1;i<5;i++)
		fscanf(f,"%d",&se[i]);
	fscanf(f,"\n");
	for(j=0;j<YY;j++)
	{
		for(i=0;i<XX;i++)
		{
			fscanf(f,"%c",&ch);
			s[i][j]=ch-'0';
		}
		fscanf(f,"\n");
	}
	for(j=0;j<YY;j++)
		for(i=0;i<XX;i++)
			fscanf(f,"%d",&t[i][j]);
	fscanf(f,"%d%d%f%d%d",&a.f,&a.l,&a.m,&a.t,&a.w);
	for(i=0;i<=a.l;i++)
		fscanf(f,"%d%d",&a.x[i],&a.y[i]);
	
	fscanf(f,"%d%d%f%d%d",&b.f,&b.l,&b.m,&b.t,&b.w);
	for(i=0;i<=b.l;i++)
		fscanf(f,"%d%d",&b.x[i],&b.y[i]);
	
	fscanf(f,"%d%d",&n0,&n);
	for(i=0;i<n;i++)
		fscanf(f,"%d%d%d%d%d",&c[i].x,&c[i].y,&c[i].f,&c[i].v,&c[i].m);
	fclose(f);
	if(hh==2)
		h=2;
	if(mode==3)
		DD=10;
	return;
}

void writesave()
{
	int i,j;
	FILE*f=fopen("save.txt","w");
	fprintf(f,"%d %d %d\n",mode,h,tt);
	for(i=1;i<5;i++)
		fprintf(f,"%d ",se[i]);
	fprintf(f,"\n");
	for(j=0;j<YY;j++)
	{
		for(i=0;i<XX;i++)
			fprintf(f,"%c",s[i][j]+'0');
		fprintf(f,"\n");
	}
	fprintf(f,"\n");
	for(j=0;j<YY;j++)
	{
		for(i=0;i<XX;i++)
			fprintf(f,"%d ",t[i][j]);
		fprintf(f,"\n");
	}
	fprintf(f,"\n");
	fprintf(f,"%d %d %f %d %d\n",a.f,a.l,a.m,a.t,a.w);
	for(i=0;i<=a.l;i++)
		fprintf(f,"%d %d\n",a.x[i],a.y[i]);
	
	fprintf(f,"\n%d %d %f %d %d\n",b.f,b.l,b.m,b.t,b.w);
	for(i=0;i<=b.l;i++)
		fprintf(f,"%d %d\n",b.x[i],b.y[i]);
	
	fprintf(f,"\n%d %d\n",n0,n);
	for(i=0;i<n;i++)
		fprintf(f,"%d %d %d %d %d\n",c[i].x,c[i].y,c[i].f,c[i].v,c[i].m);
	fclose(f);
}
