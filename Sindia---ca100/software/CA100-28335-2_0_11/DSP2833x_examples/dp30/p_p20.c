#include "DSP2833x_Device.h"     // DSP2833x Headerfile Include File
#include "DSP2833x_Examples.h"   // DSP2833x Examples Include File

/*

float R=0.0,PRESS=0.0,KIA=0.0,KIB=0.0;

void cal_press(float i,float t)
{
    R=R+i;
	KIA=0.749*0.0001*(1-0.727*0.01*R);
	KIB=2.51*0.001*R*(1-0.0846*0.001*R);
	PRESS=0.562*0.0001*R*(273+t)*(1+KIB)-R*R*KIA;
}

int density_p_p20(int t,int cha_press)
{
    float p,tem,k=200.0;
    Uint16 i;
	
	p=cha_press;
	p/=1000;
	tem=t;
	tem/=10;
	p=p-(tem+52.44)*0.018;
	if (p>0.0)cha_press=0.0;
	if (p<0.0)
	{
	    for(i=0;i<4;i++)
		{
		    k=k/(-10.0);
			while((PRESS-p)*k<0.0)cal_press(k,t);
		}
		p=0.562*0.0001*R*(273+20)*(1+KIB)-R*R*KIA-0.101325;
		cha_press=p*1000.0;
	}
	return cha_press;

}

void press_test(void)
{
    int temp;

	temp=density_p_p20(30.0,0.5);
    temp=density_p_p20(300.0,500);

}
*/

//#include<iostream> 
//#include<stdio.h> 
//#include<cmath>//调用了fabs、pow函数 
  
//double f(int,int,int,int,double); //函数声明 
//double f1(int,int,int,int,double); 
//double get_solution(int,int,int,int,double); 
  

  
double f0(int w_f,int x_f,int y_f,int z_f,double sol_f)//当根为x时，用来求f(x)的值 
{
    double f_result;

    f_result=w_f*pow(sol_f,3)+x_f*pow(sol_f,2)+y_f*sol_f+z_f; 
    return f_result; 
} 
  
double f1(int w_f1,int x_f1,int y_f1,int z_f1,double sol_f1) //当根为x时，用来求f'(x)的值 
{ 
    double f1_result;

    f1_result=3*w_f1*pow(sol_f1,2)+2*x_f1*sol_f1+y_f1; 
    return f1_result; 
} 
  
double get_solution(int w,int x,int y,int z,double sol) //求根函数，调用了上面两个函数 
{ 
    double value,tmp; 
  
    value=sol; 
    do //使用了x1=x0-f(x0)/f'(x0)，不断循环逼近 
    { 
        tmp=f0(w,x,y,z,value); 
        value=value-tmp/f1(w,x,y,z,value); 
  
    }while(fabs(tmp)>=1e-5);//当式子的值与0的绝对值小于1e-5时就认为取到了值 
  
    return value;
}
 

void   fun(double   a,double   b,double   c,double   d, 
      double   *real_y1,double   *real_y2,double   *real_y3, 
      double   *imag_y1,double   *imag_y2,double   *imag_y3) 
{ 
  double   p,q,r,u,v,g,h,fai;

  p=(3.0*a*c-b*b)/(3*a*a); 
  q=(2.0*pow(b,3.0)-9*a*b*c+27.0*a*a*d)/(27.0*pow(a,3.0)); 
  r=b/(3.0*a); 
  h=pow(q/2.0,2.0)+pow(p/3.0,3.0); 
  g=sqrt(h); 
  if(h>=0) 
  { 
    if(-q/2.0+g <0) 
    	u=-pow(fabs(-q/2.0+g),1.0/3.0);
    else 
    	u=pow((-q/2.0+g),1.0/3.0);
    if(-q/2.0-g <0) 
    	v=-pow(fabs(-q/2.0-g),1.0/3.0);
    else 
    	v=-pow((-q/2.0-g),1.0/3.0);
    if(h==0) 
    { 
      *real_y1=u+v-r;            *imag_y1=0;
      *real_y2=-(u+v)/2-r;       *imag_y2=0;
      *real_y3=-(u+v)/2-r;       *imag_y3=0;
    } 
    else 
    {   
      *real_y1=u+v-r;            *imag_y1=0;
      *real_y2=-(u+v)/2;         *imag_y2=sqrt(3.0)*(u-v)/2; 
      *real_y3=-(u+v)/2;         *imag_y3=-sqrt(3.0)*(u-v)/2; 
    } 
  } 
  else 
  { 
    fai=acos((-q/2)/(sqrt(pow(fabs(p),3)/27))); 
    *real_y1=2*sqrt(fabs(p)/3.0)*cos(fai/3.0)-r; 
    *real_y2=-2*sqrt(fabs(p)/3.0)*cos((fai+3.1415926)/3.0)-r; 
    *real_y3=-2*sqrt(fabs(p)/3.0)*cos((fai-3.1415926)/3.0)-r;
    *imag_y1=0;
    *imag_y2=0;
    *imag_y3=0;
  } 
}
/**********************************************************************************
//牛顿迭代法求解一元三次方程
//f3为要求解的一元三次方程和其一次求导的方程
//在使用时应将原状态方程进行整理，求出系数的表达式（状态方程见资料）
***********************************************************************************/
void f3(x,y)
double x,y[2];
{
   // y[0]=x*x*(x-1.0)-1.0;
	//y[1]=3.0*x*x-2.0*x;
	 y[0]=x*x*x-9*x+6;
	 y[1]=3.0*x*x-9;
}
/**********************************************************************************
//参数X：要求解的方程可能有3个不同的解，X为一个设定值，要求解方程的根离它最近；
//参数EPS：：控制精度要求；
//参数JS：为最大迭代次数；
//void(*f)() :为要求解的方程和其一次求导方程
***********************************************************************************/
int niudondiedai(double *x,double eps,int js,void(*f)())
{
    int k,l;
	double y[2],d,p,x0,x1;

	l=js;k=l;x0=*x;
	(*f)(x0,y);
	d=eps+1.0;
	while((d>=eps)&&(l!=0))
	{
	    if(fabs(y[1])+1.0==1.0)return (-1);
		x1=x0-y[0]/y[1];
		(*f)(x1,y);
		d=fabs(x1-x0);p=fabs(y[0]);
		if(p>d)d=p;
		x0=x1;l=l-1;
	}
	*x=x1;
	k=js-1;
	return k;
}

void press_test(void) 
{ 
//    double a,b,c,d; 
//    double solution,solution_value; 
//	double   real_x1,real_x2,real_x3; 
//    double   imag_x1,imag_x2,imag_x3; 

  
//    a=1;b=0;c=-9;d=6;solution=6.0;
//    solution_value=get_solution(a,b,c,d,solution); 
//    if(solution_value<=1e-5)//当求得的根很小时，直接让它为0 
//    solution_value=0; 

  //  fun(a,b,c,d,&real_x1,&real_x2,&real_x3,&imag_x1,&imag_x2,&imag_x3);  
  int js,k;
  double x,eps;
  eps=0.000001;js=60;x=1.5;
  k= niudondiedai(&x,eps,js,f3);
}



