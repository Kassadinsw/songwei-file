#include "stdafx.h"
#include<iostream>
#include<math.h>
#include"vector_mine.h"
#include "gl/glut.h"
#include "gl/glext.H"
using namespace std;
float delta_t=0.1f;
vertex vertices[20][20];
unsigned int row;
unsigned int column;
bool flag=false;
bool wind_flag=false;
bool gravity_flag=false;
bool mouseLeftDown;
bool mouseRightDown;
float mouseX, mouseY;
float cameraDistance;
float cameraAngleX;
float cameraAngleY;

//初始化质点位置
void Init_vertices()
{
   row=20;
   column=20;
   for(int i=0;i<row;i++)
   {
     for(int j=0;j<column;j++)
	 {
	     vertices[i][j].vertex_Position(j*10-160,i*(10)-80,0);//质点间距为20
		 vertices[i][j].vertex_Clear();//质点参数清零
	 }
   }
}
//鼠标操作视角子函数，从网上下的
void mouseCB(int button, int state, int x, int y)
{
    mouseX = x;
    mouseY = y;

    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseLeftDown = true;
        }
        else if(state == GLUT_UP)
            mouseLeftDown = false;
    }

    else if(button == GLUT_RIGHT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            mouseRightDown = true;
        }
        else if(state == GLUT_UP)
            mouseRightDown = false;
    }
}

void mouseMotionCB(int x, int y)
{
    if(mouseLeftDown)
    {
        cameraAngleY += (x - mouseX);
        cameraAngleX += (y - mouseY);
        mouseX = x;
        mouseY = y;
    }
    if(mouseRightDown)
    {
        cameraDistance += (y - mouseY) * 0.2f;
        mouseY = y;
    }

    glutPostRedisplay();
}

void Setup(void)
{
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glEnable(GL_DEPTH_TEST);
	
	GLfloat lightpos[]={1.0,1.0,1.0,0.0};
	glLightfv(GL_LIGHT0,GL_POSITION,lightpos);
	GLfloat diffuseLight[]={1.0f,1.0f,1.0f,1.0f};
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);;
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseLight);
	
	glShadeModel(GL_SMOOTH);
}

void ChangeSize(GLsizei w,GLsizei h)
{
	GLfloat aspectRation;
	GLfloat windowWidth;
    GLfloat windowHight;
  if(h==0)
	  h=1;
  glViewport(0,0,w,h);//定义视口大小为当前窗口大小，注：在Opengl中左下角为原点
  glMatrixMode(GL_PROJECTION);//定义矩阵形式为投影变换矩阵
  glLoadIdentity();//初始化为单位阵
  aspectRation=(GLfloat)w/(GLfloat)h;//当前视口的长宽比
  /*窗口的高大于宽，则定义可视区域的高大于宽，且比例一致
    窗口的高小于宽，则定义可视区域的高小于宽，且比例一致
  */
  if(w<=h)
  {
	 windowWidth=150;
	 windowHight=150/aspectRation;
	 glOrtho(-150.0,150.0,windowHight,windowHight,150.0,-150.0);//（左，右，底，顶，近，远）
  }
  else
  {
	  windowWidth=150.0*aspectRation;
	  windowHight=150;
	  glOrtho(-windowWidth,windowWidth,-150.0,150.0,150.0,-150.0);
  }
	 glMatrixMode(GL_MODELVIEW);//定义矩阵为模型矩阵
     glLoadIdentity();//初始化为单位阵
};
//计算质点的参数
void Compute_Parameter()
{
	 for(int i=0;i<row;i++)
	  {
	   for(int j=0;j<column;j++)//遍历所有的质点
	    {
	      if(i!=row-1)//图形最顶上一排质点参数不更新
		  {
		  vertices[i][j].vertex_Acceleration();//其余的质点更新参数
	      vertices[i][j].vertex_Velocity(delta_t);
		  vertices[i][j].vertex_Position(delta_t);
		  }
	    }
	  }
};
//计算各质点所受合力
void Compute_Force_Mass()
{
	 for(int i=0;i<row;i++)
	   {
	     for(int j=0;j<column;j++)//遍历所有质点
		 {
			/////////////////////////////////////////////////////////////////////结构弹簧
			 vertices[i][j].vertex_Force_Clear();//质点受力清零
			 if((j-1)>=0&&!vertices[i][j].broken_flag)//质点未超弹且其水平前一个位置有质点
			{
			  vertices[i][j].force+=vertices[i][j].vertex_Force(vertices[i][j-1].position,1);
			}//水平前一个质点与当前质点的拉力
			
			if((j+1)<=(column-1)&&!vertices[i][j].broken_flag)//质点未超弹且其水平后一个位置有质点
			{
			  vertices[i][j].force+=vertices[i][j].vertex_Force(vertices[i][j+1].position,1);
			 }//水平后一个质点与当前质点的拉力

			if((i-1)>=0&&!vertices[i][j].broken_flag)//质点未超弹且其垂直下一个位置有质点
		    { 
			  vertices[i][j].force+=vertices[i][j].vertex_Force(vertices[i-1][j].position,1);//质点受力累积
			}//垂直下一个质点与当前质点的拉力
			
			if((i+1)<=(row-1)&&!vertices[i][j].broken_flag)//质点未超弹且其垂直上一个位置有质点
			{
			  vertices[i][j].force+=vertices[i][j].vertex_Force(vertices[i+1][j].position,1);
		
			}//垂直上一个质点与当前质点的拉力

		//////////////////////////////////////////////////////////////////////////////剪切弹簧
			if((i-1)>=0&&(j+1)<=(column-1)&&!vertices[i][j].broken_flag)//质点未超弹且其右下位置有质点
			{
			vertices[i][j].force+=vertices[i][j].vertex_Force(vertices[i-1][j+1].position,2);
				 
			}//垂直右下方质点与当前质点的拉力
			
			if((i-1)>=0&&(j-1)>=0&&!vertices[i][j].broken_flag)//质点未超弹且其左下位置有质点
			{
			vertices[i][j].force+=vertices[i][j].vertex_Force(vertices[i-1][j-1].position,2);
				
			}//垂直左下方质点与当前质点的拉力
			
			if((i+1)<=(row-1)&&(j+1)<=(column-1)&&!vertices[i][j].broken_flag)//质点未超弹且其右上位置有质点
			{
			     vertices[i][j].force+=vertices[i][j].vertex_Force(vertices[i+1][j+1].position,2);//质点受力累积
			}//垂直右上方质点与当前质点的拉力
			
			if((i+1)<=(row-1)&&(j-1)>=0&&!vertices[i][j].broken_flag)//质点未超弹且其左上位置有质点
			{
			     vertices[i][j].force+=vertices[i][j].vertex_Force(vertices[i+1][j-1].position,2);//质点受力累积
			}//垂直左上方质点与当前质点的拉力

         /*////////////////////////////////////////////////////弯曲弹簧
			 if((j-2)>=0&&!vertices[i][j].broken_flag)
			{
			  vertices[i][j].force+=vertices[i][j].vertex_Force(vertices[i][j-2].position);
			}
			
			if((j+2)<=(column-1)&&!vertices[i][j].broken_flag)
			{
			  vertices[i][j].force+=vertices[i][j].vertex_Force(vertices[i][j+2].position);
			 }

			if((i-2)>=0&&!vertices[i][j].broken_flag)
		    { 
			  vertices[i][j].force+=vertices[i][j].vertex_Force(vertices[i-2][j].position); 
			}
			
			if((i+2)<=(row-1)&&!vertices[i][j].broken_flag)
			{
			  vertices[i][j].force+=vertices[i][j].vertex_Force(vertices[i+2][j].position);
			}

			*/
		  if(gravity_flag)//施加重力标志位有效
		  vertices[i][j].force+=vertices[i][j].vertex_Force_Gravity();//累积上重力
		  if(wind_flag)//施加风力标志位有效
		  vertices[i][j].force+=vertices[i][j].vertex_Force_Wind();//累积上风力
		
		 }
	   }
};

void TimerFunction(int value)
{
	glutPostRedisplay();
	glutTimerFunc(20,TimerFunction,1);
};

void keyboard(unsigned char key,int x,int y)
{
switch(key)
	{
	case'w'://y上加速
		for(int i=0;i<row;i++)
		vertices[i][int(column/2)].velocity.y+=25;
		break;
	case's'://y下加速
		for(int i=0;i<row;i++)
		vertices[i][int(column/2)].velocity.y-=25;
		break;
	case'a'://x左加速
		for(int i=0;i<row;i++)
		vertices[i][int(column/2)].velocity.x-=25;
		break;
	case'd'://x右加速
		for(int i=0;i<row;i++)
		vertices[i][int(column/2)].velocity.x+=25;
		break;
	case'e'://z负加速
		for(int i=0;i<row;i++)
		vertices[i][int(column/2)].velocity.z-=25;
		break;
	case'c'://z正加速
		for(int i=0;i<row;i++)
		vertices[i][int(column/2)].velocity.z+=25;
		break;
	case' '://开始、暂停
		flag=!flag;
		break;
	case'r'://重置位置
		Init_vertices();
		break;
	case'k'://施加风力
		wind_flag=!wind_flag;
		break;
	case'l'://施加重力
		gravity_flag=!gravity_flag;
		break;
	default:
		break;
	}
};


void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	if(flag==true)
	{
    Compute_Force_Mass();
	Compute_Parameter();
	}
	glPushMatrix();
	glTranslatef(0, 0, cameraDistance);
    glRotatef(cameraAngleX, 1, 0, 0);
    glRotatef(cameraAngleY, 0, 1, 0);
	for(int i=0;i<row;i++)
	{
	 for (int j=0;j<column;j++)
	 {

   	if((j+1)<=(column-1)&&(i-1)>0)
	{
		 glColor3f(1.0f,0.0f,0.0f);
		 glBegin(GL_TRIANGLES);
		 glVertex3f(
			 vertices[i][j].position.x,
			 vertices[i][j].position.y,
			 vertices[i][j].position.z);
	     glVertex3f(
			 vertices[i-1][j].position.x,
			 vertices[i-1][j].position.y,
			 vertices[i-1][j].position.z);
		 glVertex3f(
			 vertices[i-1][j+1].position.x,
			 vertices[i-1][j+1].position.y,
			 vertices[i-1][j+1].position.z);
	     glEnd();  

		 glColor3f(1.0f,1.0f,0.0f);
		 glBegin(GL_TRIANGLES);
		 glVertex3f(
			 vertices[i][j].position.x,
			 vertices[i][j].position.y,
			 vertices[i][j].position.z);
	     glVertex3f(
			 vertices[i][j+1].position.x,
			 vertices[i][j+1].position.y,
			 vertices[i][j+1].position.z);
		 glVertex3f(
			 vertices[i-1][j+1].position.x,
			 vertices[i-1][j+1].position.y,
			 vertices[i-1][j+1].position.z);
	     glEnd();  
	 }
	 }
	}
	glPopMatrix();
	glutSwapBuffers();	
}

int main()
{   
	cout<<"Space：Start\\Pause"<<endl<<"K：Wind"<<endl<<"L：Gravity"<<endl;
	cout<<"W：Up"<<endl<<"S：Down"<<endl<<"A：Left"<<endl<<"D：Right"<<endl;
	cout<<"通过鼠标可改变布料位置";
	Init_vertices();
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
	glutInitWindowSize(500,500);
	glutInitWindowPosition(100,100);
	glutCreateWindow("example");
	Setup();
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(20,TimerFunction,1);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouseCB);
    glutMotionFunc(mouseMotionCB);
	glutMainLoop();
	return 0;
}

