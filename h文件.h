class vector
{
 public:
  float x;
  float y;
  float z;
  vector();
  vector(float,float,float);
  float vector_Inner(vector);
  float vector_Norm();
  vector vector_Unit();
  void operator+=(vector);
  vector operator-(vector);
  vector operator+(vector);
};


class vertex
{
private:
	 float mass;
	 float rest_length;
	 float spring_constant1;
     float spring_constant2;
	 float damping_factor;
public:
	 bool broken_flag;
	 vector force;
	 vector force_constrain;
	 vector position;
	 vector velocity;
	 vector acceleration;
	 vertex();
	 void vertex_Position(float,float,float);
	 void vertex_Position(vector);
	 vector vertex_Force(vector,int);
	 vector vertex_Force_Spring1(vector);
	 vector vertex_Force_Spring2(vector);
	 vector vertex_Force_Damping();
	 vector vertex_Force_Gravity();
	 vector vertex_Force_Wind();
	 void vertex_Force_Clear();
	 void vertex_Clear();
	 void vertex_Acceleration();
	 void vertex_Velocity(float);
	 void vertex_Position(float);
};
