#ifndef CINDER_MATH_H
#define CINDER_MATH_H

#include <math.h>

#define PI 3.1415926535
#define MIN(a, b) (a) <= (b) ? (a) : (b)
#define MAX(a, b) (a) >= (b) ? (a) : (b)
#define ToRadians(angle) ((float(angle) * float(PI)) / 180.0f)
#define ToDegrees(angle) ((float(angle) * 180.0f) / float(PI))

namespace Cinder
{
	namespace Math
	{
		namespace Functions
		{
			template<typename T>
			T Clamp(T value, T min, T max);

			template<typename T>
			void Swap(T &a, T &b);
		}		

		template<class T>
		struct Vector2
		{		
			union
			{
				struct
				{
					T X, Y;
				};

				struct
				{
					T U, V;
				};

				T Elements[2];
			};

			Vector2(T x = T(0), T y = T(0));

			//////////////////////////
			
			void operator=(Vector2<T> &v2Right);			
			bool operator==(Vector2<T> &v2Right);			
			bool operator!=(Vector2<T> &v2Right);			
			T operator[](unsigned int index);

			/////////////////

			void operator+=(Vector2<T> &v2Right);
			void operator+=(T right);

			void operator-=(Vector2<T> &v2Right);
			void operator-=(T right);

			void operator*=(Vector2<T> &v2Right);
			void operator*=(T right);

			void operator/=(Vector2<T> &v2Right);
			void operator/=(T right);

			/////////////////


			Vector2<T> XX();
			Vector2<T> XY();

			Vector2<T> YX();
			Vector2<T> YY();
		};

		/////////////

		template<typename T>
		Vector2<T> operator+(Vector2<T> &v2Left, Vector2<T> &v2Right);
		template<typename T>
		Vector2<T> operator+(Vector2<T> &v2Left, T right);
		template<typename T>
		Vector2<T> operator+(T left, Vector2<T> &v2Right);

		template<typename T>
		Vector2<T> operator-(Vector2<T> &v2Left, Vector2<T> &v2Right);
		template<typename T>
		Vector2<T> operator-(Vector2<T> &v2Left, T right);
		template<typename T>
		Vector2<T> operator-(T left, Vector2<T> &v2Right);

		template<typename T>
		Vector2<T> operator*(Vector2<T> &v2Left, Vector2<T> &v2Right);
		template<typename T>
		Vector2<T> operator*(Vector2<T> &v2Left, T right);
		template<typename T>
		Vector2<T> operator*(T left, Vector2<T> &v2Right);

		template<typename T>
		Vector2<T> operator/(Vector2<T> &v2Left, Vector2<T> &v2Right);
		template<typename T>
		Vector2<T> operator/(Vector2<T> &v2Left, T right);
		template<typename T>
		Vector2<T> operator/(T left, Vector2<T> &v2Right);

		/////////////

		typedef Vector2<int> vec2i;
		typedef Vector2<float> vec2f;

		template<class T>
		struct Vector3
		{
			union
			{
				struct
				{
					T X, Y, Z;
				};

				struct
				{
					T R, G, B;
				};

				T Elements[3];
			};

			Vector3(T x = T(0), T y = T(0), T z = T(0));

			Vector3(Vector2<T> &vec2, T z = T(0));

			Vector3(T x, Vector2<T> &vec2);

			Vector3(Vector2<T> &vec2);

			////////////////////////

			void operator=(Vector3<T> &v3Right);
			bool operator==(Vector3<T> &v3Right);
			bool operator!=(Vector3<T> &v3Right);
			T operator[](unsigned int index);

			/////////////////////////

			void operator+=(Vector3<T> &v3Right);
			void operator+=(T right);

			void operator-=(Vector3<T> &v3Right);
			void operator-=(T right);

			void operator*=(Vector3<T> &v3Right);
			void operator*=(T right);

			void operator/=(Vector3<T> &v3Right);
			void operator/=(T right);

			//////////////////////					

			Vector2<T> XX();
			Vector2<T> XY();
			Vector2<T> XZ();

			//////////////////////

			Vector2<T> YX();
			Vector2<T> YY();
			Vector2<T> YZ();

			//////////////////////
			
			Vector2<T> ZX();
			Vector2<T> ZY();
			Vector2<T> ZZ();

			//////////////////////

			Vector3<T> XXX();
			Vector3<T> XXY();
			Vector3<T> XXZ();

			Vector3<T> XYX();
			Vector3<T> XYY();
			Vector3<T> XYZ();

			Vector3<T> XZX();
			Vector3<T> XZY();
			Vector3<T> XZZ();

			//////////////////////

			Vector3<T> YXX();
			Vector3<T> YXY();
			Vector3<T> YXZ();
			
			Vector3<T> YYX();
			Vector3<T> YYY();
			Vector3<T> YYZ();
			
			Vector3<T> YZX();
			Vector3<T> YZY();
			Vector3<T> YZZ();

			//////////////////////

			Vector3<T> ZXX();
			Vector3<T> ZXY();
			Vector3<T> ZXZ();

			Vector3<T> ZYX();
			Vector3<T> ZYY();
			Vector3<T> ZYZ();

			Vector3<T> ZZX();
			Vector3<T> ZZY();
			Vector3<T> ZZZ();

			//////////////////////			
		};

		///////////////////////////
		template<typename T>
		Vector3<T> operator+(Vector3<T> &v3Left, Vector3<T> &v3Right);
		template<typename T>
		Vector3<T> operator+(Vector3<T> &v3Left, T right);
		template<typename T>
		Vector3<T> operator+(T left, Vector3<T> &v3Right);

		template<typename T>
		Vector3<T> operator-(Vector3<T> &v3Left, Vector3<T> &v3Right);
		template<typename T>
		Vector3<T> operator-(Vector3<T> &v3Left, T right);
		template<typename T>
		Vector3<T> operator-(T left, Vector3<T> &v3Right);

		template<typename T>
		Vector3<T> operator*(Vector3<T> &v3Left, Vector3<T> &v3Right);
		template<typename T>
		Vector3<T> operator*(Vector3<T> &v3Left, T right);
		template<typename T>
		Vector3<T> operator*(T left, Vector3<T> &v3Right);

		template<typename T>
		Vector3<T> operator/(Vector3<T> &v3Left, Vector3<T> &v3Right);
		template<typename T>
		Vector3<T> operator/(Vector3<T> &v3Left, T right);
		template<typename T>
		Vector3<T> operator/(T left, Vector3<T> &v3Right);
		///////////////////////////

		typedef Vector3<int> vec3i;
		typedef Vector3<float> vec3f;

		template<class T>
		struct Vector4
		{
			union
			{
				struct
				{
					T X, Y, Z, W;
				};

				struct
				{
					T R, G, B, A;
				};

				T Elements[4];
			};

			Vector4(T x = T(0), T y = T(0), T z = T(0), T w = T(0));
			
			Vector4(Vector3<T> &vec3, T w = T(0));

			Vector4(T x, Vector3<T> &vec3);

			Vector4(Vector3<T> &vec3);

			Vector4(Vector2<T> &vec2);

			Vector4(Vector2<T> &vec2_1, Vector2<T> &vec2_2);

			Vector4(Vector2<T> &vec2, T z = T(0), T w = T(0));

			Vector4(T x, T y, Vector2<T> &vec2);

			////////////////////////

			void operator=(Vector4<T> & v4Right);
			bool operator==(Vector4<T> &v4Right);
			bool operator!=(Vector4<T> &v4Right);
			T operator[](unsigned int index);

			////////////////////////

			void operator+=(Vector4<T> &v4Right);
			void operator+=(T right);

			void operator-=(Vector4<T> &v4Right);
			void operator-=(T right);

			void operator*=(Vector4<T> &v4Right);
			void operator*=(T right);

			void operator/=(Vector4<T> &v4Right);
			void operator/=(T right);

			//////////////////////					

			Vector2<T> XX();
			Vector2<T> XY();
			Vector2<T> XZ();
			Vector2<T> XW();

			//////////////////////

			Vector2<T> YX();
			Vector2<T> YY();
			Vector2<T> YZ();
			Vector2<T> YW();

			//////////////////////

			Vector2<T> ZX();
			Vector2<T> ZY();
			Vector2<T> ZZ();
			Vector2<T> ZW();

			///////////////////////

			Vector2<T> WX();
			Vector2<T> WY();
			Vector2<T> WZ();
			Vector2<T> WW();
			//////////////////////

			Vector3<T> XXX();
			Vector3<T> XXY();
			Vector3<T> XXZ();
			Vector3<T> XXW();

			Vector3<T> XYX();
			Vector3<T> XYY();
			Vector3<T> XYZ();
			Vector3<T> XYW();

			Vector3<T> XZX();
			Vector3<T> XZY();
			Vector3<T> XZZ();
			Vector3<T> XZW();

			Vector3<T> XWX();
			Vector3<T> XWY();
			Vector3<T> XWZ();
			Vector3<T> XWW();

			//////////////////////

			Vector3<T> YXX();
			Vector3<T> YXY();
			Vector3<T> YXZ();
			Vector3<T> YXW();
			
			Vector3<T> YYX();
			Vector3<T> YYY();
			Vector3<T> YYZ();
			Vector3<T> YYW();

			Vector3<T> YZX();
			Vector3<T> YZY();
			Vector3<T> YZZ();
			Vector3<T> YZW();

			Vector3<T> YWX();
			Vector3<T> YWY();
			Vector3<T> YWZ();
			Vector3<T> YWW();

			//////////////////////

			Vector3<T> ZXX();
			Vector3<T> ZXY();
			Vector3<T> ZXZ();
			Vector3<T> ZXW();

			Vector3<T> ZYX();
			Vector3<T> ZYY();
			Vector3<T> ZYZ();
			Vector3<T> ZYW();

			Vector3<T> ZZX();
			Vector3<T> ZZY();
			Vector3<T> ZZZ();
			Vector3<T> ZZW();

			Vector3<T> ZWX();
			Vector3<T> ZWY();
			Vector3<T> ZWZ();
			Vector3<T> ZWW();

			//////////////////////	
			
			Vector3<T> WXX();
			Vector3<T> WXY();
			Vector3<T> WXZ();
			Vector3<T> WXW();
			
			Vector3<T> WYX();
			Vector3<T> WYY();
			Vector3<T> WYZ();
			Vector3<T> WYW();

			Vector3<T> WZX();
			Vector3<T> WZY();
			Vector3<T> WZZ();
			Vector3<T> WZW();

			Vector3<T> WWX();
			Vector3<T> WWY();
			Vector3<T> WWZ();
			Vector3<T> WWW();

			//////////////////////
			//////////////////////

			Vector4<T> XXXX();
			Vector4<T> XXXY();
			Vector4<T> XXXZ();
			Vector4<T> XXXW();

			Vector4<T> XXYX();
			Vector4<T> XXYY();
			Vector4<T> XXYZ();
			Vector4<T> XXYW();
			
			Vector4<T> XXZX();
			Vector4<T> XXZY();
			Vector4<T> XXZZ();
			Vector4<T> XXZW();

			Vector4<T> XXWX();
			Vector4<T> XXWY();
			Vector4<T> XXWZ();
			Vector4<T> XXWW();

			//////////////////////

			Vector4<T> XYXX();
			Vector4<T> XYXY();
			Vector4<T> XYXZ();
			Vector4<T> XYXW();

			Vector4<T> XYYX();
			Vector4<T> XYYY();
			Vector4<T> XYYZ();
			Vector4<T> XYYW();
			
			Vector4<T> XYZX();
			Vector4<T> XYZY();
			Vector4<T> XYZZ();
			Vector4<T> XYZW();
			
			Vector4<T> XYWX();
			Vector4<T> XYWY();
			Vector4<T> XYWZ();
			Vector4<T> XYWW();

			//////////////////////

			Vector4<T> XZXX();
			Vector4<T> XZXY();
			Vector4<T> XZXZ();
			Vector4<T> XZXW();
			
			Vector4<T> XZYX();
			Vector4<T> XZYY();
			Vector4<T> XZYZ();
			Vector4<T> XZYW();

			Vector4<T> XZZX();
			Vector4<T> XZZY();
			Vector4<T> XZZZ();
			Vector4<T> XZZW();
			
			Vector4<T> XZWX();
			Vector4<T> XZWY();
			Vector4<T> XZWZ();
			Vector4<T> XZWW();

			//////////////////////	

			Vector4<T> XWXX();
			Vector4<T> XWXY();
			Vector4<T> XWXZ();
			Vector4<T> XWXW();
			
			Vector4<T> XWYX();
			Vector4<T> XWYY();
			Vector4<T> XWYZ();
			Vector4<T> XWYW();

			Vector4<T> XWZX();
			Vector4<T> XWZY();
			Vector4<T> XWZZ();
			Vector4<T> XWZW();
			
			Vector4<T> XWWX();
			Vector4<T> XWWY();
			Vector4<T> XWWZ();
			Vector4<T> XWWW();

			//////////////////////
			//////////////////////

			Vector4<T> YXXX();
			Vector4<T> YXXY();
			Vector4<T> YXXZ();
			Vector4<T> YXXW();
			
			Vector4<T> YXYX();
			Vector4<T> YXYY();
			Vector4<T> YXYZ();
			Vector4<T> YXYW();
						
			Vector4<T> YXZX();
			Vector4<T> YXZY();
			Vector4<T> YXZZ();
			Vector4<T> YXZW();

			Vector4<T> YXWX();
			Vector4<T> YXWY();
			Vector4<T> YXWZ();
			Vector4<T> YXWW();

			//////////////////////

			Vector4<T> YYXX();
			Vector4<T> YYXY();
			Vector4<T> YYXZ();
			Vector4<T> YYXW();

			Vector4<T> YYYX();
			Vector4<T> YYYY();
			Vector4<T> YYYZ();
			Vector4<T> YYYW();
			
			Vector4<T> YYZX();
			Vector4<T> YYZY();
			Vector4<T> YYZZ();
			Vector4<T> YYZW();
			
			Vector4<T> YYWX();
			Vector4<T> YYWY();
			Vector4<T> YYWZ();
			Vector4<T> YYWW();

			//////////////////////

			Vector4<T> YZXX();
			Vector4<T> YZXY();
			Vector4<T> YZXZ();
			Vector4<T> YZXW();
			
			Vector4<T> YZYX();
			Vector4<T> YZYY();
			Vector4<T> YZYZ();
			Vector4<T> YZYW();
			
			Vector4<T> YZZX();
			Vector4<T> YZZY();
			Vector4<T> YZZZ();
			Vector4<T> YZZW();
			
			Vector4<T> YZWX();
			Vector4<T> YZWY();
			Vector4<T> YZWZ();
			Vector4<T> YZWW();

			//////////////////////	

			Vector4<T> YWXX();
			Vector4<T> YWXY();
			Vector4<T> YWXZ();
			Vector4<T> YWXW();

			Vector4<T> YWYX();
			Vector4<T> YWYY();
			Vector4<T> YWYZ();
			Vector4<T> YWYW();

			Vector4<T> YWZX();
			Vector4<T> YWZY();
			Vector4<T> YWZZ();
			Vector4<T> YWZW();

			Vector4<T> YWWX();
			Vector4<T> YWWY();
			Vector4<T> YWWZ();
			Vector4<T> YWWW();

			//////////////////////
			//////////////////////

			Vector4<T> ZXXX();
			Vector4<T> ZXXY();
			Vector4<T> ZXXZ();
			Vector4<T> ZXXW();

			Vector4<T> ZXYX();
			Vector4<T> ZXYY();
			Vector4<T> ZXYZ();
			Vector4<T> ZXYW();
			
			Vector4<T> ZXZX();
			Vector4<T> ZXZY();
			Vector4<T> ZXZZ();
			Vector4<T> ZXZW();

			Vector4<T> ZXWX();
			Vector4<T> ZXWY();
			Vector4<T> ZXWZ();
			Vector4<T> ZXWW();

			//////////////////////

			Vector4<T> ZYXX();
			Vector4<T> ZYXY();
			Vector4<T> ZYXZ();
			Vector4<T> ZYXW();
			
			Vector4<T> ZYYX();
			Vector4<T> ZYYY();
			Vector4<T> ZYYZ();
			Vector4<T> ZYYW();
			
			Vector4<T> ZYZX();
			Vector4<T> ZYZY();
			Vector4<T> ZYZZ();
			Vector4<T> ZYZW();

			Vector4<T> ZYWX();
			Vector4<T> ZYWY();
			Vector4<T> ZYWZ();
			Vector4<T> ZYWW();

			//////////////////////

			Vector4<T> ZZXX();
			Vector4<T> ZZXY();
			Vector4<T> ZZXZ();
			Vector4<T> ZZXW();

			Vector4<T> ZZYX();
			Vector4<T> ZZYY();
			Vector4<T> ZZYZ();
			Vector4<T> ZZYW();

			Vector4<T> ZZZX();
			Vector4<T> ZZZY();
			Vector4<T> ZZZZ();
			Vector4<T> ZZZW();

			Vector4<T> ZZWX();
			Vector4<T> ZZWY();
			Vector4<T> ZZWZ();
			Vector4<T> ZZWW();

			//////////////////////	

			Vector4<T> ZWXX();
			Vector4<T> ZWXY();
			Vector4<T> ZWXZ();
			Vector4<T> ZWXW();

			Vector4<T> ZWYX();
			Vector4<T> ZWYY();
			Vector4<T> ZWYZ();
			Vector4<T> ZWYW();

			Vector4<T> ZWZX();
			Vector4<T> ZWZY();
			Vector4<T> ZWZZ();
			Vector4<T> ZWZW();

			Vector4<T> ZWWX();
			Vector4<T> ZWWY();
			Vector4<T> ZWWZ();
			Vector4<T> ZWWW();

			//////////////////////
			//////////////////////

			Vector4<T> WXXX();
			Vector4<T> WXXY();
			Vector4<T> WXXZ();
			Vector4<T> WXXW();

			Vector4<T> WXYX();
			Vector4<T> WXYY();
			Vector4<T> WXYZ();
			Vector4<T> WXYW();

			Vector4<T> WXZX();
			Vector4<T> WXZY();
			Vector4<T> WXZZ();
			Vector4<T> WXZW();

			Vector4<T> WXWX();
			Vector4<T> WXWY();
			Vector4<T> WXWZ();
			Vector4<T> WXWW();

			//////////////////////

			Vector4<T> WYXX();
			Vector4<T> WYXY();
			Vector4<T> WYXZ();
			Vector4<T> WYXW();

			Vector4<T> WYYX();
			Vector4<T> WYYY();
			Vector4<T> WYYZ();
			Vector4<T> WYYW();

			Vector4<T> WYZX();
			Vector4<T> WYZY();
			Vector4<T> WYZZ();
			Vector4<T> WYZW();

			Vector4<T> WYWX();
			Vector4<T> WYWY();
			Vector4<T> WYWZ();
			Vector4<T> WYWW();

			//////////////////////

			Vector4<T> WZXX();
			Vector4<T> WZXY();
			Vector4<T> WZXZ();
			Vector4<T> WZXW();

			Vector4<T> WZYX();
			Vector4<T> WZYY();
			Vector4<T> WZYZ();
			Vector4<T> WZYW();

			Vector4<T> WZZX();
			Vector4<T> WZZY();
			Vector4<T> WZZZ();
			Vector4<T> WZZW();

			Vector4<T> WZWX();
			Vector4<T> WZWY();
			Vector4<T> WZWZ();
			Vector4<T> WZWW();

			//////////////////////	

			Vector4<T> WWXX();
			Vector4<T> WWXY();
			Vector4<T> WWXZ();
			Vector4<T> WWXW();

			Vector4<T> WWYX();
			Vector4<T> WWYY();
			Vector4<T> WWYZ();
			Vector4<T> WWYW();

			Vector4<T> WWZX();
			Vector4<T> WWZY();
			Vector4<T> WWZZ();
			Vector4<T> WWZW();

			Vector4<T> WWWX();
			Vector4<T> WWWY();
			Vector4<T> WWWZ();
			Vector4<T> WWWW();
		};

		/////////////

		template<typename T>
		Vector4<T> operator+(Vector4<T> &v4Left, Vector4<T> &v4Right);
		template<typename T>
		Vector4<T> operator+(Vector4<T> &v4Left, T right);
		template<typename T>
		Vector4<T> operator+(T left, Vector4<T> &v4Right);

		template<typename T>
		Vector4<T> operator-(Vector4<T> &v4Left, Vector4<T> &v4Right);
		template<typename T>
		Vector4<T> operator-(Vector4<T> &v4Left, T right);
		template<typename T>
		Vector4<T> operator-(T left, Vector4<T> &v4Right);

		template<typename T>
		Vector4<T> operator*(Vector4<T> &v4Left, Vector4<T> &v4Right);
		template<typename T>
		Vector4<T> operator*(Vector4<T> &v4Left, T right);
		template<typename T>
		Vector4<T> operator*(T left, Vector4<T> &v4Right);

		template<typename T>
		Vector4<T> operator/(Vector4<T> &v4Left, Vector4<T> &v4Right);
		template<typename T>
		Vector4<T> operator/(Vector4<T> &v4Left, T right);
		template<typename T>
		Vector4<T> operator/(T left, Vector4<T> &v4Right);

		/////////////////			

		typedef Vector4<int> vec4i;
		typedef Vector4<float> vec4f;
		typedef Vector4<float> Quaternion;

		template<class T>
		struct Matrix4
		{
			union 
			{
				T Elements_2D[4][4];
				T Elements_1D[16];
			};

			Matrix4(T value = T(1), bool diagonalOnly = true);

			///////////////////////			
			
			Vector4<T> operator[](unsigned int row);

			void operator=(Matrix4<T> &other);

			void operator+=(Matrix4<T> &other);

			void operator-=(Matrix4<T> &other);

			void operator*=(T other);
			void operator*=(Matrix4<T> &other);

			void operator/=(T other);

			///////////////////////
		};

		typedef Matrix4<int> mat4i;
		typedef Matrix4<float> mat4f;

		////////////////////////////////////////

		template<typename T>
		Matrix4<T> operator+(Matrix4<T> &left, Matrix4<T> &right);

		template<typename T>
		Matrix4<T> operator-(Matrix4<T> &left, Matrix4<T> &right);

		template<typename T>
		Matrix4<T> operator*(Matrix4<T> &left, T right);
		template<typename T>
		Matrix4<T> operator*(Matrix4<T> &left, Matrix4<T> &right);
		template<typename T>
		Vector2<T> operator*(Matrix4<T> &left, Vector2<T> &right);
		template<typename T>
		Vector3<T> operator*(Matrix4<T> &left, Vector3<T> &right);
		template<typename T>
		Vector4<T> operator*(Matrix4<T> &left, Vector4<T> &right);

		template<typename T>
		Matrix4<T> operator/(Matrix4<T> &left, T right);

		////////////////////////////////////////

		namespace Vec2
		{
			template<typename T>
			float Length(Vector2<T> &v2);

			template<typename T>
			float LengthSquare(Vector2<T> &v2);

			template<typename T>
			Vector2<T> Normalize(Vector2<T> &v2);

			template<typename T>
			Vector2<T> Sum(Vector2<T> &v2Left, Vector2<T> &v2Right);
			template<typename T>
			Vector2<T> Sum(Vector2<T> &v2Left, T right);
			template<typename T>
			Vector2<T> Sum(T left, Vector2<T> &v2Right);

			template<typename T>
			Vector2<T> Sub(Vector2<T> &v2Left, Vector2<T> &v2Right);
			template<typename T>
			Vector2<T> Sub(Vector2<T> &v2Left, T right);
			template<typename T>
			Vector2<T> Sub(T left, Vector2<T> &v2Right);

			template<typename T>
			Vector2<T> Mul(Vector2<T> &v2Left, Vector2<T> &v2Right);
			template<typename T>
			Vector2<T> Mul(Vector2<T> &v2Left, T right);
			template<typename T>
			Vector2<T> Mul(T left, Vector2<T> &v2Right);

			template<typename T>
			Vector2<T> Div(Vector2<T> &v2Left, Vector2<T> &v2Right);
			template<typename T>
			Vector2<T> Div(Vector2<T> &v2Left, T right);
			template<typename T>
			Vector2<T> Div(T left, Vector2<T> &v2Right);

			template<typename T>
			Vector2<T> Translate(Vector2<T> &point, Vector2<T> &direction);

			template<typename T>
			Vector2<T> Rotate(Vector2<T> &point, Vector2<T> &angle);

			template<typename T>
			Vector2<T> Scale(Vector2<T> &point, Vector2<T> &value);
		}

		namespace Vec3
		{
			template<typename T>
			float Length(Vector3<T> &v3);

			template<typename T>
			float LengthSquare(Vector3<T> &v3);

			template<typename T>
			Vector3<T> Normalize(Vector3<T> &v3);

			template<typename T>
			Vector3<T> Sum(Vector3<T> &v3Left, Vector3<T> &v3Right);
			template<class T>
			Vector3<T> Sum(Vector3<T> &v3Left, T right);
			template<typename T>
			Vector3<T> Sum(T left, Vector3<T> &v3Right);

			template<typename T>
			Vector3<T> Sub(Vector3<T> &v3Left, Vector3<T> &v3Right);
			template<typename T>
			Vector3<T> Sub(Vector3<T> &v3Left, T right);
			template<typename T>
			Vector3<T> Sub(T left, Vector3<T> &v3Right);

			template<typename T>
			Vector3<T> Mul(Vector3<T> &v3Left, Vector3<T> &v3Right);
			template<typename T>
			Vector3<T> Mul(Vector3<T> &v3Left, T right);
			template<typename T>
			Vector3<T> Mul(T left, Vector3<T> &v3Right);

			template<typename T>
			Vector3<T> Div(Vector3<T> &v3Left, Vector3<T> &v3Right);
			template<typename T>
			Vector3<T> Div(Vector3<T> &v3Left, T right);
			template<typename T>
			Vector3<T> Div(T left, Vector3<T> &v3Right);

			/////////////////////////////

			template<typename T>
			T Dot(Vector3<T> &v3Left, Vector3<T> &v3Right);

			template<typename T>
			Vector3<T> Cross(Vector3<T> &v3Left, Vector3<T> &v3Right);

			///////////////////////////////

			template<typename T>
			Vector3<T> Translate(Vector3<T> &point, Vector3<T> &direction);

			template<typename T>
			Vector3<T> Rotate(Vector3<T> &point, Vector3<T> &angle);

			template<typename T>
			Vector3<T> Scale(Vector3<T> &point, Vector3<T> &value);
		}

		namespace Vec4
		{
			template<typename T>
			float Length(Vector4<T> &v4);

			template<typename T>
			float LengthSquare(Vector4<T> &v4);

			template<typename T>
			Vector4<T> Normalize(Vector4<T> &v4);

			template<typename T>
			Vector4<T> Sum(Vector4<T> &v4Left, Vector4<T> &v4Right);
			template<typename T>
			Vector4<T> Sum(Vector4<T> &v4Left, T right);
			template<typename T>
			Vector4<T> Sum(T left, Vector4<T> &v4Right);

			template<typename T>
			Vector4<T> Sub(Vector4<T> &v4Left, Vector4<T> &v4Right);
			template<typename T>
			Vector4<T> Sub(Vector4<T> &v4Left, T right);
			template<typename T>
			Vector4<T> Sub(T left, Vector4<T> &v4Right);

			template<typename T>
			Vector4<T> Mul(Vector4<T> &v4Left, Vector4<T> &v4Right);
			template<typename T>
			Vector4<T> Mul(Vector4<T> &v4Left, T right);
			template<typename T>
			Vector4<T> Mul(T left, Vector4<T> &v4Right);

			template<typename T>
			Vector4<T> Div(Vector4<T> &v4Left, Vector4<T> &v4Right);
			template<typename T>
			Vector4<T> Div(Vector4<T> &v4Left, T right);
			template<typename T>
			Vector4<T> Div(T left, Vector4<T> &v4Right);
		}

		namespace Mat4
		{
			template<typename T>
			Matrix4<T> Sum(Matrix4<T> &left, Matrix4<T> &right);

			template<typename T>
			Matrix4<T> Sub(Matrix4<T> &left, Matrix4<T> &right);

			template<typename T>
			Matrix4<T> Mul(Matrix4<T> &left, T right);
			template<typename T>
			Matrix4<T> Mul(Matrix4<T> &left, Matrix4<T> &right);
			template<typename T>
			Vector2<T> Mul(Matrix4<T> &left, Vector2<T> &right);
			template<typename T>
			Vector3<T> Mul(Matrix4<T> &left, Vector3<T> &right);
			template<typename T>
			Vector4<T> Mul(Matrix4<T> &left, Vector4<T> &right);

			template<typename T>
			Matrix4<T> Div(Matrix4<T> &left, T right);

			template<typename T>
			Matrix4<T> Translation(Vector3<T> &direction);

			template<typename T>
			Matrix4<T> Scale(Vector3<T> &value);

			template<typename T>
			Matrix4<T> Rotation(Vector3<T> &angle);

			template<typename T>
			Matrix4<T> LookAt(Vector3<T> &target, Vector3<T> &up);

			template<typename T>
			Matrix4<T> Orthographic(float right, float left, float top, float bottom, float near, float far);
			template<typename T>
			Matrix4<T> Orthographic(float width, float height, float near, float far);

			template<typename T>
			Matrix4<T> Perspective(float fieldOfView, float width, float height, float near, float far);
		}
	}
}
#endif

#include "Vector2.cpp"
#include "Vector3.cpp"
#include "Vector4.cpp"
#include "Matrix4.cpp"
#include "GeneralMath.cpp"