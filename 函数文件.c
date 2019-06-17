#include"stdafx.h"
#include"vector_mine.h"
#include<math.h>
#include<iostream>
#define random(x)(rand()%x)//生成0至x的随机数
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

 
  /*求向量a与已知向量vector的内积a.vector
    返回一个浮点型数inner*/
  float vector::vector_Inner(vector a)
  {
   float inner;
   inner=a.x*x+a.y*y+a.z*z;
   return inner;
  };

   /*求一个向量的模长
    返回一个浮点型数norm*/
  float vector::vector_Norm()
  {
    float norm;
    float sum_square=pow(x,2)+pow(y,2)+pow(z,2);
    norm=sqrt(sum_square);
	return norm;
  };

  /*求一个向量的单位向量
    返回一个向量unit*/
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


  //质点初始化
  vertex::vertex()
   {
	 mass=1.0f;
	 rest_length=10.0f;
	 spring_constant1=45.0f;
	 spring_constant2=5.0f;
	 damping_factor=0.1f;
	 broken_flag=false;
	};
  //质点位置初始化
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
 ///////////////////////////////计算当前质点与所在a位置处的质点的弹簧力
   vector vertex::vertex_Force_Spring1(vector a)
	{
		vector length_vector=a-position;//两位置之间的向量
		vector force_vector;//记录弹簧力
		if(length_vector.vector_Norm()>=15)//位置向量的模长》=15，用以判断超弹
		broken_flag=true;//发生超弹，标志位为真
		else
		broken_flag=false;//未发生超弹，标志位为假
		if(broken_flag==false)//未发生超弹
		{
		float length=length_vector.vector_Norm()-rest_length;//有效长度=当前长度-松弛长度（正负表示方向）
		float force=length*spring_constant1;//弹簧力的模长
		force_vector.x=length_vector.vector_Unit().x*force;//单位位置向量*力模长
		force_vector.y=length_vector.vector_Unit().y*force;//给弹簧力以方向
		force_vector.z=length_vector.vector_Unit().z*force;
		}
		else//发生超弹
		{
		  force_constrain.x=90*length_vector.vector_Unit().x;//90*单位位置向量，90是以一个大的数作为惩罚力
		  force_constrain.y=90*length_vector.vector_Unit().y;
		  force_constrain.z=90*length_vector.vector_Unit().z;
		}
		return force_vector;//返回未超弹时的弹簧力
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

   //计算阻尼力
   vector vertex::vertex_Force_Damping()
	{
	  vector force_vector;
	  force_vector.x=-velocity.x*damping_factor;//阻尼力方向与当前的速度方向相反
	  force_vector.y=-velocity.y*damping_factor;
	  force_vector.z=-velocity.z*damping_factor;
	  return force_vector;
	};
	//计算重力
	 vector vertex::vertex_Force_Gravity()
	{
	  vector force_vector;
	  float g=-9.8;//重力加速度
	  force_vector.x=0;
	  force_vector.y=g*mass;//y轴上施加重力
	  force_vector.z=0;
	  return force_vector;//返回重力
	};
	//计算风力
	 vector vertex::vertex_Force_Wind()
	{
	  vector force_vector;
	  force_vector.x=10;//x方向恒定10大小的力
	  force_vector.y=random(20)-random(20);//y方向随机0-20的力减去0-20的力（即力可能是y正方向，负方向）
	  force_vector.z=random(10)-random(5);
	  return force_vector;//返回风力
	};
	//质点受力清零
	void vertex::vertex_Force_Clear()
	{
	    force.x=0;
	    force.y=0;
	    force.z=0;
		broken_flag=false;
	};
	//质点参数清零
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
	//质点内力计算函数
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
	//计算质点的加速度
   void vertex::vertex_Acceleration()
	{
		
		acceleration.x=force.x/mass;
		acceleration.y=force.y/mass;
		acceleration.z=force.z/mass;
	};
   //计算质点的速度
   void vertex::vertex_Velocity(float delta_t)
	{
	    if(broken_flag==false)//未超弹
		{
		 velocity.x=velocity.x+acceleration.x*delta_t;//正常计算
	     velocity.y=velocity.y+acceleration.y*delta_t;
	     velocity.z=velocity.z+acceleration.z*delta_t;
		}
		else//出现超弹
		{
		 velocity.x=force_constrain.x/mass*delta_t;//延超弹反方向计算速度，用以消除超弹
	     velocity.y=force_constrain.y/mass*delta_t;
	     velocity.z=force_constrain.z/mass*delta_t;
		}
	};
    //计算质点的位置
   void vertex::vertex_Position(float delta_t)
	{ 
		position.x=position.x+velocity.x*delta_t;
	    position.y=position.y+velocity.y*delta_t;
		position.z=position.z+velocity.z*delta_t;
	
	};