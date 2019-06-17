#include"stdafx.h"
#include"vector_mine.h"
#include<math.h>
#include<iostream>
#define random(x)(rand()%x)//����0��x�������
using namespace std;
//
vector::vector()
{
     x=0;
	 y=0;
	 z=0;
};
vector::vector(float a,float b,float c)
{
	  x=a;
	  y=b;
	  z=c;
};

 
  /*������a����֪����vector���ڻ�a.vector
    ����һ����������inner*/
  float vector::vector_Inner(vector a)
  {
   float inner;
   inner=a.x*x+a.y*y+a.z*z;
   return inner;
  };

   /*��һ��������ģ��
    ����һ����������norm*/
  float vector::vector_Norm()
  {
    float norm;
    float sum_square=pow(x,2)+pow(y,2)+pow(z,2);
    norm=sqrt(sum_square);
	return norm;
  };

  /*��һ�������ĵ�λ����
    ����һ������unit*/
  vector vector::vector_Unit()
  {
	 vector unit;
     float norm=vector_Norm();
	 unit.x=x/norm;
     unit.y=y/norm;
	 unit.z=z/norm;
	 return unit;
  };

  void vector::operator+=(vector a)
  {
	  x=x+a.x;
	  y=y+a.y;
	  z=z+a.z;
  }

 vector vector::operator-(vector a)
  {
   vector difference;
   difference.x=x-a.x;
   difference.y=y-a.y;
   difference.z=z-a.z;
   return difference;
  };

 vector vector::operator+(vector a)
  {
   vector sum;
   sum.x=x+a.x;
   sum.y=y+a.y;
   sum.z=z+a.z;
   return sum;
  };


  //�ʵ��ʼ��
  vertex::vertex()
   {
	 mass=1.0f;
	 rest_length=10.0f;
	 spring_constant1=45.0f;
	 spring_constant2=5.0f;
	 damping_factor=0.1f;
	 broken_flag=false;
	};
  //�ʵ�λ�ó�ʼ��
  void  vertex::vertex_Position(float a,float b,float c)
	{
	 position.x=a;
	 position.y=b;
	 position.z=c;
	};

 void  vertex::vertex_Position(vector a)
	{
	 position.x=a.x;
	 position.y=a.y;
	 position.z=a.z;
	};
 ///////////////////////////////���㵱ǰ�ʵ�������aλ�ô����ʵ�ĵ�����
   vector vertex::vertex_Force_Spring1(vector a)
	{
		vector length_vector=a-position;//��λ��֮�������
		vector force_vector;//��¼������
		if(length_vector.vector_Norm()>=15)//λ��������ģ����=15�������жϳ���
		broken_flag=true;//������������־λΪ��
		else
		broken_flag=false;//δ������������־λΪ��
		if(broken_flag==false)//δ��������
		{
		float length=length_vector.vector_Norm()-rest_length;//��Ч����=��ǰ����-�ɳڳ��ȣ�������ʾ����
		float force=length*spring_constant1;//��������ģ��
		force_vector.x=length_vector.vector_Unit().x*force;//��λλ������*��ģ��
		force_vector.y=length_vector.vector_Unit().y*force;//���������Է���
		force_vector.z=length_vector.vector_Unit().z*force;
		}
		else//��������
		{
		  force_constrain.x=90*length_vector.vector_Unit().x;//90*��λλ��������90����һ���������Ϊ�ͷ���
		  force_constrain.y=90*length_vector.vector_Unit().y;
		  force_constrain.z=90*length_vector.vector_Unit().z;
		}
		return force_vector;//����δ����ʱ�ĵ�����
	};


   vector vertex::vertex_Force_Spring2(vector a)
	{
		vector length_vector=a-position;
		vector force_vector;
		if(length_vector.vector_Norm()>=28)
		broken_flag=true;
		else
		broken_flag=false;
		if(broken_flag==false)
		{
		float length=length_vector.vector_Norm()-rest_length;
		float force=length*spring_constant2;
		force_vector.x=length_vector.vector_Unit().x*force;
		force_vector.y=length_vector.vector_Unit().y*force;
		force_vector.z=length_vector.vector_Unit().z*force;
		}
		else
		{
		  force_constrain.x=30*length_vector.vector_Unit().x;
		  force_constrain.y=30*length_vector.vector_Unit().y;
		  force_constrain.z=30*length_vector.vector_Unit().z;
		}
		return force_vector;
	};

   //����������
   vector vertex::vertex_Force_Damping()
	{
	  vector force_vector;
	  force_vector.x=-velocity.x*damping_factor;//�����������뵱ǰ���ٶȷ����෴
	  force_vector.y=-velocity.y*damping_factor;
	  force_vector.z=-velocity.z*damping_factor;
	  return force_vector;
	};
	//��������
	 vector vertex::vertex_Force_Gravity()
	{
	  vector force_vector;
	  float g=-9.8;//�������ٶ�
	  force_vector.x=0;
	  force_vector.y=g*mass;//y����ʩ������
	  force_vector.z=0;
	  return force_vector;//��������
	};
	//�������
	 vector vertex::vertex_Force_Wind()
	{
	  vector force_vector;
	  force_vector.x=10;//x����㶨10��С����
	  force_vector.y=random(20)-random(20);//y�������0-20������ȥ0-20����������������y�����򣬸�����
	  force_vector.z=random(10)-random(5);
	  return force_vector;//���ط���
	};
	//�ʵ���������
	void vertex::vertex_Force_Clear()
	{
	    force.x=0;
	    force.y=0;
	    force.z=0;
		broken_flag=false;
	};
	//�ʵ��������
	void vertex::vertex_Clear()
	{
	    force.x=0;
	    force.y=0;
	    force.z=0;
		velocity.x=0;
        velocity.y=0;
		velocity.z=0;
		acceleration.x=0;
		acceleration.y=0;
		acceleration.z=0;
	};
	//�ʵ��������㺯��
	vector vertex::vertex_Force(vector a,int b)
   {
	   vector force;
	   vector force_spring;
	   if(b==1)
	    force_spring=vertex_Force_Spring1(a);
	   else
        force_spring=vertex_Force_Spring2(a);

	   vector force_damping=vertex_Force_Damping();
	   force = force_spring+force_damping;
	   return force;
   };
	//�����ʵ�ļ��ٶ�
   void vertex::vertex_Acceleration()
	{
		
		acceleration.x=force.x/mass;
		acceleration.y=force.y/mass;
		acceleration.z=force.z/mass;
	};
   //�����ʵ���ٶ�
   void vertex::vertex_Velocity(float delta_t)
	{
	    if(broken_flag==false)//δ����
		{
		 velocity.x=velocity.x+acceleration.x*delta_t;//��������
	     velocity.y=velocity.y+acceleration.y*delta_t;
	     velocity.z=velocity.z+acceleration.z*delta_t;
		}
		else//���ֳ���
		{
		 velocity.x=force_constrain.x/mass*delta_t;//�ӳ�������������ٶȣ�������������
	     velocity.y=force_constrain.y/mass*delta_t;
	     velocity.z=force_constrain.z/mass*delta_t;
		}
	};
    //�����ʵ��λ��
   void vertex::vertex_Position(float delta_t)
	{ 
		position.x=position.x+velocity.x*delta_t;
	    position.y=position.y+velocity.y*delta_t;
		position.z=position.z+velocity.z*delta_t;
	
	};