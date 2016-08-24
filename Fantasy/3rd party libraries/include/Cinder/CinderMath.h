#ifndef CINDER_MATH_H
#define CINDER_MATH_H

#include <math.h>
#include <stdint.h>

#define PI 3.1415926535
#define MIN(a, b) (a) <= (b) ? (a) : (b)
#define MAX(a, b) (a) >= (b) ? (a) : (b)
#define ToRadians(angle) ((float(angle) * float(PI)) / 180.0f)
#define ToDegrees(angle) ((float(angle) * 180.0f) / float(PI))

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

namespace Cinder
{
	namespace Math
	{
		namespace Functions
		{
			float Clamp(float value, float min, float max);
		}		

		struct vec2f
		{		
			union
			{
				struct
				{
					float X, Y;
				};

				struct
				{
					float U, V;
				};

				float Elements[2];
			};

			vec2f(float x = 0, float y = 0)
				: X(x), Y(y)
			{}

			~vec2f(){}

			//////////////////////////
			
			void operator=(vec2f &v2Right);			
			bool operator==(vec2f &v2Right);			
			bool operator!=(vec2f &v2Right);			
			float& operator[](unsigned int index);

			/////////////////

			void operator+=(vec2f &v2Right);
			void operator+=(float right);

			void operator-=(vec2f &v2Right);
			void operator-=(float right);

			void operator*=(vec2f &v2Right);
			void operator*=(float right);

			void operator/=(vec2f &v2Right);
			void operator/=(float right);

			/////////////////


			vec2f XX();
			vec2f XY();

			vec2f YX();
			vec2f YY();
		};

		/////////////

		vec2f operator+(vec2f &v2Left, vec2f &v2Right);
		vec2f operator+(vec2f &v2Left, float right);
		vec2f operator+(float left, vec2f &v2Right);

		vec2f operator-(vec2f &v2Left, vec2f &v2Right);
		vec2f operator-(vec2f &v2Left, float right);
		vec2f operator-(float left, vec2f &v2Right);

		vec2f operator*(vec2f &v2Left, vec2f &v2Right);
		vec2f operator*(vec2f &v2Left, float right);
		vec2f operator*(float left, vec2f &v2Right);

		vec2f operator/(vec2f &v2Left, vec2f &v2Right);
		vec2f operator/(vec2f &v2Left, float right);
		vec2f operator/(float left, vec2f &v2Right);

		/////////////

		struct vec3f
		{
			union
			{
				struct
				{
					float X, Y, Z;
				};

				struct
				{
					float R, G, B;
				};

				float Elements[3];
			};

			vec3f(float x = 0, float y = 0, float z = 0)
				: X(x), Y(y), Z(z)
			{}

			vec3f(vec2f &vec2, float z = 0)
				: X(vec2.X), Y(vec2.Y), Z(z)
			{}

			vec3f(float x, vec2f &vec2)
				: X(x), Y(vec2.X), Z(vec2.Y)
			{}

			~vec3f(){}

			////////////////////////

			void operator=(vec3f &v3Right);
			bool operator==(vec3f &v3Right);
			bool operator!=(vec3f &v3Right);
			float& operator[](unsigned int index);

			/////////////////////////

			void operator+=(vec3f &v3Right);
			void operator+=(float right);

			void operator-=(vec3f &v3Right);
			void operator-=(float right);

			void operator*=(vec3f &v3Right);
			void operator*=(float right);

			void operator/=(vec3f &v3Right);
			void operator/=(float right);

			//////////////////////					

			vec2f XX();
			vec2f XY();
			vec2f XZ();

			//////////////////////

			vec2f YX();
			vec2f YY();
			vec2f YZ();

			//////////////////////
			
			vec2f ZX();
			vec2f ZY();
			vec2f ZZ();

			//////////////////////

			vec3f XXX();
			vec3f XXY();
			vec3f XXZ();

			vec3f XYX();
			vec3f XYY();
			vec3f XYZ();

			vec3f XZX();
			vec3f XZY();
			vec3f XZZ();

			//////////////////////

			vec3f YXX();
			vec3f YXY();
			vec3f YXZ();
			
			vec3f YYX();
			vec3f YYY();
			vec3f YYZ();
			
			vec3f YZX();
			vec3f YZY();
			vec3f YZZ();

			//////////////////////

			vec3f ZXX();
			vec3f ZXY();
			vec3f ZXZ();

			vec3f ZYX();
			vec3f ZYY();
			vec3f ZYZ();

			vec3f ZZX();
			vec3f ZZY();
			vec3f ZZZ();

			//////////////////////			
		};

		///////////////////////////
		vec3f operator+(vec3f &v3Left, vec3f &v3Right);
		vec3f operator+(vec3f &v3Left, float right);
		vec3f operator+(float left, vec3f &v3Right);

		vec3f operator-(vec3f &v3Left, vec3f &v3Right);
		vec3f operator-(vec3f &v3Left, float right);
		vec3f operator-(float left, vec3f &v3Right);

		vec3f operator*(vec3f &v3Left, vec3f &v3Right);
		vec3f operator*(vec3f &v3Left, float right);
		vec3f operator*(float left, vec3f &v3Right);

		vec3f operator/(vec3f &v3Left, vec3f &v3Right);
		vec3f operator/(vec3f &v3Left, float right);
		vec3f operator/(float left, vec3f &v3Right);
		///////////////////////////		

		struct vec4f
		{
			union
			{
				struct
				{
					float X, Y, Z, W;
				};

				struct
				{
					float R, G, B, A;
				};

				float Elements[4];
			};

			vec4f(float x = 0, float y = 0, float z = 0, float w = 1) 
				: X(x), Y(y), Z(z), W(w)
			{}
			
			vec4f(vec3f &vec3, float w = 1) 
				: X(vec3.X), Y(vec3.Y), Z(vec3.Z), W(w)
			{}

			vec4f(float x, vec3f &vec3)
				: X(x), Y(vec3.X), Z(vec3.Y), W(vec3.Z)
			{}

			vec4f(vec2f &vec2_1, vec2f &vec2_2)
				: X(vec2_1.X), Y(vec2_1.Y), Z(vec2_2.X), W(vec2_2.Y)
			{}

			vec4f(vec2f &vec2, float z = 0, float w = 1)
				: X(vec2.X), Y(vec2.Y), Z(z), W(w)
			{}

			vec4f(float x, float y, vec2f &vec2) 
				: X(x), Y(y), Z(vec2.X), W(vec2.Y)
			{}

			~vec4f(){}

			////////////////////////

			void operator=(vec4f & v4Right);
			bool operator==(vec4f &v4Right);
			bool operator!=(vec4f &v4Right);
			float& operator[](unsigned int index);

			////////////////////////

			void operator+=(vec4f &v4Right);
			void operator+=(float right);

			void operator-=(vec4f &v4Right);
			void operator-=(float right);

			void operator*=(vec4f &v4Right);
			void operator*=(float right);

			void operator/=(vec4f &v4Right);
			void operator/=(float right);

			//////////////////////					

			vec2f XX();
			vec2f XY();
			vec2f XZ();
			vec2f XW();

			//////////////////////

			vec2f YX();
			vec2f YY();
			vec2f YZ();
			vec2f YW();

			//////////////////////

			vec2f ZX();
			vec2f ZY();
			vec2f ZZ();
			vec2f ZW();

			///////////////////////

			vec2f WX();
			vec2f WY();
			vec2f WZ();
			vec2f WW();
			//////////////////////

			vec3f XXX();
			vec3f XXY();
			vec3f XXZ();
			vec3f XXW();

			vec3f XYX();
			vec3f XYY();
			vec3f XYZ();
			vec3f XYW();

			vec3f XZX();
			vec3f XZY();
			vec3f XZZ();
			vec3f XZW();

			vec3f XWX();
			vec3f XWY();
			vec3f XWZ();
			vec3f XWW();

			//////////////////////

			vec3f YXX();
			vec3f YXY();
			vec3f YXZ();
			vec3f YXW();
			
			vec3f YYX();
			vec3f YYY();
			vec3f YYZ();
			vec3f YYW();

			vec3f YZX();
			vec3f YZY();
			vec3f YZZ();
			vec3f YZW();

			vec3f YWX();
			vec3f YWY();
			vec3f YWZ();
			vec3f YWW();

			//////////////////////

			vec3f ZXX();
			vec3f ZXY();
			vec3f ZXZ();
			vec3f ZXW();

			vec3f ZYX();
			vec3f ZYY();
			vec3f ZYZ();
			vec3f ZYW();

			vec3f ZZX();
			vec3f ZZY();
			vec3f ZZZ();
			vec3f ZZW();

			vec3f ZWX();
			vec3f ZWY();
			vec3f ZWZ();
			vec3f ZWW();

			//////////////////////	
			
			vec3f WXX();
			vec3f WXY();
			vec3f WXZ();
			vec3f WXW();
			
			vec3f WYX();
			vec3f WYY();
			vec3f WYZ();
			vec3f WYW();

			vec3f WZX();
			vec3f WZY();
			vec3f WZZ();
			vec3f WZW();

			vec3f WWX();
			vec3f WWY();
			vec3f WWZ();
			vec3f WWW();

			//////////////////////
			//////////////////////

			vec4f XXXX();
			vec4f XXXY();
			vec4f XXXZ();
			vec4f XXXW();

			vec4f XXYX();
			vec4f XXYY();
			vec4f XXYZ();
			vec4f XXYW();
			
			vec4f XXZX();
			vec4f XXZY();
			vec4f XXZZ();
			vec4f XXZW();

			vec4f XXWX();
			vec4f XXWY();
			vec4f XXWZ();
			vec4f XXWW();

			//////////////////////

			vec4f XYXX();
			vec4f XYXY();
			vec4f XYXZ();
			vec4f XYXW();

			vec4f XYYX();
			vec4f XYYY();
			vec4f XYYZ();
			vec4f XYYW();
			
			vec4f XYZX();
			vec4f XYZY();
			vec4f XYZZ();
			vec4f XYZW();
			
			vec4f XYWX();
			vec4f XYWY();
			vec4f XYWZ();
			vec4f XYWW();

			//////////////////////

			vec4f XZXX();
			vec4f XZXY();
			vec4f XZXZ();
			vec4f XZXW();
			
			vec4f XZYX();
			vec4f XZYY();
			vec4f XZYZ();
			vec4f XZYW();

			vec4f XZZX();
			vec4f XZZY();
			vec4f XZZZ();
			vec4f XZZW();
			
			vec4f XZWX();
			vec4f XZWY();
			vec4f XZWZ();
			vec4f XZWW();

			//////////////////////	

			vec4f XWXX();
			vec4f XWXY();
			vec4f XWXZ();
			vec4f XWXW();
			
			vec4f XWYX();
			vec4f XWYY();
			vec4f XWYZ();
			vec4f XWYW();

			vec4f XWZX();
			vec4f XWZY();
			vec4f XWZZ();
			vec4f XWZW();
			
			vec4f XWWX();
			vec4f XWWY();
			vec4f XWWZ();
			vec4f XWWW();

			//////////////////////
			//////////////////////

			vec4f YXXX();
			vec4f YXXY();
			vec4f YXXZ();
			vec4f YXXW();
			
			vec4f YXYX();
			vec4f YXYY();
			vec4f YXYZ();
			vec4f YXYW();
						
			vec4f YXZX();
			vec4f YXZY();
			vec4f YXZZ();
			vec4f YXZW();

			vec4f YXWX();
			vec4f YXWY();
			vec4f YXWZ();
			vec4f YXWW();

			//////////////////////

			vec4f YYXX();
			vec4f YYXY();
			vec4f YYXZ();
			vec4f YYXW();

			vec4f YYYX();
			vec4f YYYY();
			vec4f YYYZ();
			vec4f YYYW();
			
			vec4f YYZX();
			vec4f YYZY();
			vec4f YYZZ();
			vec4f YYZW();
			
			vec4f YYWX();
			vec4f YYWY();
			vec4f YYWZ();
			vec4f YYWW();

			//////////////////////

			vec4f YZXX();
			vec4f YZXY();
			vec4f YZXZ();
			vec4f YZXW();
			
			vec4f YZYX();
			vec4f YZYY();
			vec4f YZYZ();
			vec4f YZYW();
			
			vec4f YZZX();
			vec4f YZZY();
			vec4f YZZZ();
			vec4f YZZW();
			
			vec4f YZWX();
			vec4f YZWY();
			vec4f YZWZ();
			vec4f YZWW();

			//////////////////////	

			vec4f YWXX();
			vec4f YWXY();
			vec4f YWXZ();
			vec4f YWXW();

			vec4f YWYX();
			vec4f YWYY();
			vec4f YWYZ();
			vec4f YWYW();

			vec4f YWZX();
			vec4f YWZY();
			vec4f YWZZ();
			vec4f YWZW();

			vec4f YWWX();
			vec4f YWWY();
			vec4f YWWZ();
			vec4f YWWW();

			//////////////////////
			//////////////////////

			vec4f ZXXX();
			vec4f ZXXY();
			vec4f ZXXZ();
			vec4f ZXXW();

			vec4f ZXYX();
			vec4f ZXYY();
			vec4f ZXYZ();
			vec4f ZXYW();
			
			vec4f ZXZX();
			vec4f ZXZY();
			vec4f ZXZZ();
			vec4f ZXZW();

			vec4f ZXWX();
			vec4f ZXWY();
			vec4f ZXWZ();
			vec4f ZXWW();

			//////////////////////

			vec4f ZYXX();
			vec4f ZYXY();
			vec4f ZYXZ();
			vec4f ZYXW();
			
			vec4f ZYYX();
			vec4f ZYYY();
			vec4f ZYYZ();
			vec4f ZYYW();
			
			vec4f ZYZX();
			vec4f ZYZY();
			vec4f ZYZZ();
			vec4f ZYZW();

			vec4f ZYWX();
			vec4f ZYWY();
			vec4f ZYWZ();
			vec4f ZYWW();

			//////////////////////

			vec4f ZZXX();
			vec4f ZZXY();
			vec4f ZZXZ();
			vec4f ZZXW();

			vec4f ZZYX();
			vec4f ZZYY();
			vec4f ZZYZ();
			vec4f ZZYW();

			vec4f ZZZX();
			vec4f ZZZY();
			vec4f ZZZZ();
			vec4f ZZZW();

			vec4f ZZWX();
			vec4f ZZWY();
			vec4f ZZWZ();
			vec4f ZZWW();

			//////////////////////	

			vec4f ZWXX();
			vec4f ZWXY();
			vec4f ZWXZ();
			vec4f ZWXW();

			vec4f ZWYX();
			vec4f ZWYY();
			vec4f ZWYZ();
			vec4f ZWYW();

			vec4f ZWZX();
			vec4f ZWZY();
			vec4f ZWZZ();
			vec4f ZWZW();

			vec4f ZWWX();
			vec4f ZWWY();
			vec4f ZWWZ();
			vec4f ZWWW();

			//////////////////////
			//////////////////////

			vec4f WXXX();
			vec4f WXXY();
			vec4f WXXZ();
			vec4f WXXW();

			vec4f WXYX();
			vec4f WXYY();
			vec4f WXYZ();
			vec4f WXYW();

			vec4f WXZX();
			vec4f WXZY();
			vec4f WXZZ();
			vec4f WXZW();

			vec4f WXWX();
			vec4f WXWY();
			vec4f WXWZ();
			vec4f WXWW();

			//////////////////////

			vec4f WYXX();
			vec4f WYXY();
			vec4f WYXZ();
			vec4f WYXW();

			vec4f WYYX();
			vec4f WYYY();
			vec4f WYYZ();
			vec4f WYYW();

			vec4f WYZX();
			vec4f WYZY();
			vec4f WYZZ();
			vec4f WYZW();

			vec4f WYWX();
			vec4f WYWY();
			vec4f WYWZ();
			vec4f WYWW();

			//////////////////////

			vec4f WZXX();
			vec4f WZXY();
			vec4f WZXZ();
			vec4f WZXW();

			vec4f WZYX();
			vec4f WZYY();
			vec4f WZYZ();
			vec4f WZYW();

			vec4f WZZX();
			vec4f WZZY();
			vec4f WZZZ();
			vec4f WZZW();

			vec4f WZWX();
			vec4f WZWY();
			vec4f WZWZ();
			vec4f WZWW();

			//////////////////////	

			vec4f WWXX();
			vec4f WWXY();
			vec4f WWXZ();
			vec4f WWXW();

			vec4f WWYX();
			vec4f WWYY();
			vec4f WWYZ();
			vec4f WWYW();

			vec4f WWZX();
			vec4f WWZY();
			vec4f WWZZ();
			vec4f WWZW();

			vec4f WWWX();
			vec4f WWWY();
			vec4f WWWZ();
			vec4f WWWW();
		};

		typedef vec4f Quaternion;

		/////////////

		vec4f operator+(vec4f &v4Left, vec4f &v4Right);
		vec4f operator+(vec4f &v4Left, float right);
		vec4f operator+(float left, vec4f &v4Right);

		vec4f operator-(vec4f &v4Left, vec4f &v4Right);
		vec4f operator-(vec4f &v4Left, float right);
		vec4f operator-(float left, vec4f &v4Right);

		vec4f operator*(vec4f &v4Left, vec4f &v4Right);
		vec4f operator*(vec4f &v4Left, float right);
		vec4f operator*(float left, vec4f &v4Right);

		vec4f operator/(vec4f &v4Left, vec4f &v4Right);
		vec4f operator/(vec4f &v4Left, float right);
		vec4f operator/(float left, vec4f &v4Right);

		/////////////////			

		struct mat4f
		{
			union 
			{
				float Elements_2D[4][4];
				float Elements_1D[16];
			};

			mat4f(float value = 1, bool diagonalOnly = true)
			{
				if (diagonalOnly)
				{
					Elements_2D[0][0] = value;		Elements_2D[0][1] = 0;			Elements_2D[0][2] = 0;			Elements_2D[0][3] = 0;
					Elements_2D[1][0] = 0;			Elements_2D[1][1] = value;		Elements_2D[1][2] = 0;			Elements_2D[1][3] = 0;
					Elements_2D[2][0] = 0;			Elements_2D[2][1] = 0;			Elements_2D[2][2] = value;		Elements_2D[2][3] = 0;
					Elements_2D[3][0] = 0;			Elements_2D[3][1] = 0;			Elements_2D[3][2] = 0;			Elements_2D[3][3] = value;
				}
				else
				{
					Elements_2D[0][0] = value;		Elements_2D[0][1] = value;		Elements_2D[0][2] = value;		Elements_2D[0][3] = value;
					Elements_2D[1][0] = value;		Elements_2D[1][1] = value;		Elements_2D[1][2] = value;		Elements_2D[1][3] = value;
					Elements_2D[2][0] = value;		Elements_2D[2][1] = value;		Elements_2D[2][2] = value;		Elements_2D[2][3] = value;
					Elements_2D[3][0] = value;		Elements_2D[3][1] = value;		Elements_2D[3][2] = value;		Elements_2D[3][3] = value;
				}

			}

			~mat4f(){}

			///////////////////////			
			
			vec4f operator[](unsigned int row);

			void operator=(mat4f &other);

			void operator+=(mat4f &other);

			void operator-=(mat4f &other);

			void operator*=(float other);
			void operator*=(mat4f &other);

			void operator/=(float other);

			///////////////////////
		};

		////////////////////////////////////////

		mat4f operator+(mat4f &left, mat4f &right);

		mat4f operator-(mat4f &left, mat4f &right);

		mat4f operator*(mat4f &left, float right);
		mat4f operator*(mat4f &left, mat4f &right);
		vec2f operator*(mat4f &left, vec2f &right);
		vec3f operator*(mat4f &left, vec3f &right);
		vec4f operator*(mat4f &left, vec4f &right);

		mat4f operator/(mat4f &left, float right);

		////////////////////////////////////////

		namespace Vec2
		{
			float Length(vec2f &v2);

			float LengthSquare(vec2f &v2);

			vec2f Normalize(vec2f &v2);

			vec2f Sum(vec2f &v2Left, vec2f &v2Right);
			vec2f Sum(vec2f &v2Left, float right);
			vec2f Sum(float left, vec2f &v2Right);

			vec2f Sub(vec2f &v2Left, vec2f &v2Right);
			vec2f Sub(vec2f &v2Left, float right);
			vec2f Sub(float left, vec2f &v2Right);

			vec2f Mul(vec2f &v2Left, vec2f &v2Right);
			vec2f Mul(vec2f &v2Left, float right);
			vec2f Mul(float left, vec2f &v2Right);

			vec2f Div(vec2f &v2Left, vec2f &v2Right);
			vec2f Div(vec2f &v2Left, float right);
			vec2f Div(float left, vec2f &v2Right);

			vec2f Translate(vec2f &point, vec2f &direction);

			vec2f Rotate(vec2f &point, vec2f &angle);

			vec2f Scale(vec2f &point, vec2f &value);
		}

		namespace Vec3
		{
			float Length(vec3f &v3);

			float LengthSquare(vec3f &v3);

			vec3f Normalize(vec3f &v3);

			vec3f Sum(vec3f &v3Left, vec3f &v3Right);
			vec3f Sum(vec3f &v3Left, float right);
			vec3f Sum(float left, vec3f &v3Right);

			vec3f Sub(vec3f &v3Left, vec3f &v3Right);
			vec3f Sub(vec3f &v3Left, float right);
			vec3f Sub(float left, vec3f &v3Right);

			vec3f Mul(vec3f &v3Left, vec3f &v3Right);
			vec3f Mul(vec3f &v3Left, float right);
			vec3f Mul(float left, vec3f &v3Right);

			vec3f Div(vec3f &v3Left, vec3f &v3Right);
			vec3f Div(vec3f &v3Left, float right);
			vec3f Div(float left, vec3f &v3Right);

			/////////////////////////////

			float Dot(vec3f &v3Left, vec3f &v3Right);

			vec3f Cross(vec3f &v3Left, vec3f &v3Right);

			///////////////////////////////

			vec3f Translate(vec3f &point, vec3f &direction);

			vec3f Rotate(vec3f &point, vec3f &angle);

			vec3f Scale(vec3f &point, vec3f &value);
		}

		namespace Vec4
		{
			float Length(vec4f &v4);

			float LengthSquare(vec4f &v4);

			vec4f Normalize(vec4f &v4);

			vec4f Sum(vec4f &v4Left, vec4f &v4Right);
			vec4f Sum(vec4f &v4Left, float right);
			vec4f Sum(float left, vec4f &v4Right);

			vec4f Sub(vec4f &v4Left, vec4f &v4Right);
			vec4f Sub(vec4f &v4Left, float right);
			vec4f Sub(float left, vec4f &v4Right);

			vec4f Mul(vec4f &v4Left, vec4f &v4Right);
			vec4f Mul(vec4f &v4Left, float right);
			vec4f Mul(float left, vec4f &v4Right);

			vec4f Div(vec4f &v4Left, vec4f &v4Right);
			vec4f Div(vec4f &v4Left, float right);
			vec4f Div(float left, vec4f &v4Right);
		}

		namespace Mat4
		{
			mat4f Sum(mat4f &left, mat4f &right);

			mat4f Sub(mat4f &left, mat4f &right);

			mat4f Mul(mat4f &left, float right);
			mat4f Mul(mat4f &left, mat4f &right);
			vec2f Mul(mat4f &left, vec2f &right);
			vec3f Mul(mat4f &left, vec3f &right);
			vec4f Mul(mat4f &left, vec4f &right);

			mat4f Div(mat4f &left, float right);


			mat4f Translation(vec3f &direction);
			mat4f Scale(vec3f &value);
			mat4f Rotation(vec3f &angle);
			mat4f LookAt(vec3f target, vec3f up);
			mat4f Orthographic(float right, float left, float top, float bottom, float near, float far);
			mat4f Orthographic(float width, float height, float near, float far);
			mat4f Perspective(float fieldOfView, float width, float height, float near, float far);
		}
	}
}

#endif