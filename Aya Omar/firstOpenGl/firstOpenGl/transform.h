
#ifndef TRANSFORM_INCLUDED_H
#define TRANSFORM_INCLUDED_H

// include math library for matrix operations ,...
#include<glm/glm.hpp>
// and this for transformation function : rotate , translte , scale 
#include<glm/gtx/transform.hpp>


// why struct or class ????

struct transform 
{
public:
	
	// intialize parameters of transformation each with empty vec3 () except scale with vec3 (1,1,1)

	//================================= may be this diffence cause errrrrrrrrrrrroooors ========================================
	transform(const glm::vec3 &pos = glm::vec3(), const glm::vec3 &rot = glm::vec3(), const glm::vec3 &scale = glm::vec3(1, 1, 1)) :

		// take the data to use it 
		m_pos(pos) ,
		m_rot(rot) ,
		m_scale(scale)

	{}
	// ************* Note *********************
	// public get and set functions to use the private data so we can set  each rotate , pos , scale individually in the main 
	// in this case we dont need to call the constructor with paramaters , just call the default (transform transform ; )

	inline void setPos(const glm::vec3 &pos){ this->m_pos = pos; }
	inline void setRot(glm::vec3& rot) { this->m_rot = rot; }
	inline void setScale(glm::vec3& scale) { this->m_scale = scale; }

	// note : get function return type is by reference =======> so we can call getpos.x ,....
	inline glm::vec3 &getPos() { return m_pos; }
	inline glm::vec3 &getRot() { return m_rot; }
	inline glm::vec3 &getScale() { return m_scale; }

	// NOWWW : turn each vector to 4*4 transformation matrix 

	// whhhhhhhy this way of declaring get model function 

	inline  glm::mat4 getmodel() const {
		glm::mat4 pos_Matrix = glm::translate(m_pos);
		glm::mat4 scale_Matrix = glm::scale(m_scale);

		// for rotate we turn each coordinate (x, y , z ) to 4*4 matrix 

		// rotate (angle x , x axis ) ;
		glm::mat4 rot_X_matrix = glm::rotate(m_rot.x, glm::vec3(1, 0, 0));
		glm::mat4 rot_Y_matrix = glm::rotate(m_rot.y, glm::vec3(0, 1, 0));
		glm::mat4 rot_Z_Xmatrix = glm::rotate(m_rot.z, glm::vec3(0, 0, 1));

		// NOOOW we need only one transformation matrix combine all those =====> Multiply with order 

		// write in <<<<<<<<reverse order>>>>>>>>>>>>> you want

		// we need to do this : x*y then *z 

		glm::mat4 rotate_Matrix = rot_Z_Xmatrix * rot_Y_matrix * rot_X_matrix;

		// cause we need scaling first then rotate then put in position 

		return pos_Matrix *rotate_Matrix *scale_Matrix;

		// we finished creating our matrix then we need to send it to vertex shader 

		// our rest work for transformation will be in vertex shader and    shader class
	}







private:
	// the actual data 
	glm::vec3 m_pos;
	glm::vec3 m_rot;

	glm::vec3 m_scale;





};



#endif
