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

//��ʼ���ʵ�λ��
void Init_vertices()
{
   row=20;
   column=20;
   for(int i=0;i<row;i++)
   {
     for(int j=0;j<column;j++)
	 {
	     vertices[i][j].vertex_Position(j*10-160,i*(10)-80,0);//�ʵ���Ϊ20
		 vertices[i][j].vertex_Clear();//�ʵ��������
	 }
   }
}
//�������ӽ��Ӻ������������µ�
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
  glViewport(0,0,w,h);//�����ӿڴ�СΪ��ǰ���ڴ�С��ע����Opengl�����½�Ϊԭ��
  glMatrixMode(GL_PROJECTION);//���������ʽΪͶӰ�任����
  glLoadIdentity();//��ʼ��Ϊ��λ��
  aspectRation=(GLfloat)w/(GLfloat)h;//��ǰ�ӿڵĳ����
  /*���ڵĸߴ��ڿ������������ĸߴ��ڿ��ұ���һ��
    ���ڵĸ�С�ڿ������������ĸ�С�ڿ��ұ���һ��
  */
  if(w<=h)
  {
	 windowWidth=150;
	 windowHight=150/aspectRation;
	 glOrtho(-150.0,150.0,windowHight,windowHight,150.0,-150.0);//�����ң��ף���������Զ��
  }
  else
  {
	  windowWidth=150.0*aspectRation;
	  windowHight=150;
	  glOrtho(-windowWidth,windowWidth,-150.0,150.0,150.0,-150.0);
  }
	 glMatrixMode(GL_MODELVIEW);//�������Ϊģ�;���
     glLoadIdentity();//��ʼ��Ϊ��λ��
};
//�����ʵ�Ĳ���
void Compute_Parameter()
{
	 for(int i=0;i<row;i++)
	  {
	   for(int j=0;j<column;j++)//�������е��ʵ�
	    {
	      if(i!=row-1)//ͼ�����һ���ʵ����������
		  {
		  vertices[i][j].vertex_Acceleration();//������ʵ���²���
	      vertices[i][j].vertex_Velocity(delta_t);
		  vertices[i][j].vertex_Position(delta_t);
		  }
	    }
	  }
};
//������ʵ����ܺ���
void Compute_Force_Mass()
{
	 for(int i=0;i<row;i++)
	   {
	     for(int j=0;j<column;j++)//���������ʵ�
		 {
			/////////////////////////////////////////////////////////////////////�ṹ����
			 vertices[i][j].vertex_Force_Clear();//�ʵ���������
			 if((j-1)>=0&&!vertices[i][j].broken_flag)//�ʵ�δ��������ˮƽǰһ��λ�����ʵ�
			{
			  vertices[i][j].force+=vertices[i][j].vertex_Force(vertices[i][j-1].position,1);
			}//ˮƽǰһ���ʵ��뵱ǰ�ʵ������
			
			if((j+1)<=(column-1)&&!vertices[i][j].broken_flag)//�ʵ�δ��������ˮƽ��һ��λ�����ʵ�
			{
			  vertices[i][j].force+=vertices[i][j].vertex_Force(vertices[i][j+1].position,1);
			 }//ˮƽ��һ���ʵ��뵱ǰ�ʵ������

			if((i-1)>=0&&!vertices[i][j].broken_flag)//�ʵ�δ�������䴹ֱ��һ��λ�����ʵ�
		    { 
			  vertices[i][j].force+=vertices[i][j].vertex_Force(vertices[i-1][j].position,1);//�ʵ������ۻ�
			}//��ֱ��һ���ʵ��뵱ǰ�ʵ������
			
			if((i+1)<=(row-1)&&!vertices[i][j].broken_flag)//�ʵ�δ�������䴹ֱ��һ��λ�����ʵ�
			{
			  vertices[i][j].force+=vertices[i][j].vertex_Force(vertices[i+1][j].position,1);
		
			}//��ֱ��һ���ʵ��뵱ǰ�ʵ������

		//////////////////////////////////////////////////////////////////////////////���е���
			if((i-1)>=0&&(j+1)<=(column-1)&&!vertices[i][j].broken_flag)//�ʵ�δ������������λ�����ʵ�
			{
			vertices[i][j].force+=vertices[i][j].vertex_Force(vertices[i-1][j+1].position,2);
				 
			}//��ֱ���·��ʵ��뵱ǰ�ʵ������
			
			if((i-1)>=0&&(j-1)>=0&&!vertices[i][j].broken_flag)//�ʵ�δ������������λ�����ʵ�
			{
			vertices[i][j].force+=vertices[i][j].vertex_Force(vertices[i-1][j-1].position,2);
				
			}//��ֱ���·��ʵ��뵱ǰ�ʵ������
			
			if((i+1)<=(row-1)&&(j+1)<=(column-1)&&!vertices[i][j].broken_flag)//�ʵ�δ������������λ�����ʵ�
			{
			     vertices[i][j].force+=vertices[i][j].vertex_Force(vertices[i+1][j+1].position,2);//�ʵ������ۻ�
			}//��ֱ���Ϸ��ʵ��뵱ǰ�ʵ������
			
			if((i+1)<=(row-1)&&(j-1)>=0&&!vertices[i][j].broken_flag)//�ʵ�δ������������λ�����ʵ�
			{
			     vertices[i][j].force+=vertices[i][j].vertex_Force(vertices[i+1][j-1].position,2);//�ʵ������ۻ�
			}//��ֱ���Ϸ��ʵ��뵱ǰ�ʵ������

         /*////////////////////////////////////////////////////��������
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
		  if(gravity_flag)//ʩ��������־λ��Ч
		  vertices[i][j].force+=vertices[i][j].vertex_Force_Gravity();//�ۻ�������
		  if(wind_flag)//ʩ�ӷ�����־λ��Ч
		  vertices[i][j].force+=vertices[i][j].vertex_Force_Wind();//�ۻ��Ϸ���
		
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
	case'w'://y�ϼ���
		for(int i=0;i<row;i++)
		vertices[i][int(column/2)].velocity.y+=25;
		break;
	case's'://y�¼���
		for(int i=0;i<row;i++)
		vertices[i][int(column/2)].velocity.y-=25;
		break;
	case'a'://x�����
		for(int i=0;i<row;i++)
		vertices[i][int(column/2)].velocity.x-=25;
		break;
	case'd'://x�Ҽ���
		for(int i=0;i<row;i++)
		vertices[i][int(column/2)].velocity.x+=25;
		break;
	case'e'://z������
		for(int i=0;i<row;i++)
		vertices[i][int(column/2)].velocity.z-=25;
		break;
	case'c'://z������
		for(int i=0;i<row;i++)
		vertices[i][int(column/2)].velocity.z+=25;
		break;
	case' '://��ʼ����ͣ
		flag=!flag;
		break;
	case'r'://����λ��
		Init_vertices();
		break;
	case'k'://ʩ�ӷ���
		wind_flag=!wind_flag;
		break;
	case'l'://ʩ������
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
	cout<<"Space��Start\\Pause"<<endl<<"K��Wind"<<endl<<"L��Gravity"<<endl;
	cout<<"W��Up"<<endl<<"S��Down"<<endl<<"A��Left"<<endl<<"D��Right"<<endl;
	cout<<"ͨ�����ɸı䲼��λ��";
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

