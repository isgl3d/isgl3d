/*
 * iSGL3D: http://isgl3d.com
 *
 * Copyright (c) 2010-2011 Stuart Caunt
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#import <Foundation/Foundation.h>
#import "Isgl3dMathTypes.h"
#import "Isgl3dVector.h"
#import "Isgl3dQuaternion.h"

/*
 * Use GLKit definitions for iOS >= 5.0 and if no strict ANSI compilation is set (C/C++ language dialect)
 * GLKit linkage required in this case
 */
#if !(defined(__STRICT_ANSI__)) && (__IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_5_0)

#import <GLKit/GLKMath.h>

#pragma mark - GLKit compatible definitions
#

#define Isgl3dMatrix4Transpose GLKitMatrix4Transpose
#define Isgl3dMatrix4Multiply GLKitMatrix4Multiply
#define Isgl3dMatrix4Add GLKitMatrix4Add
#define Isgl3dMatrix4Subtract GLKitMatrix4Subtract

#else

#pragma mark - GLKit corresponding matrix implementations
#

#include <stddef.h>
#include <stdbool.h>
#include <math.h>

#if defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

static __inline__ Isgl3dMatrix4 Isgl3dMatrix4Transpose(Isgl3dMatrix4 matrix)
{
#if defined(__ARM_NEON__)
    float32x4x4_t m = vld4q_f32(matrix.m);
    return *(Isgl3dMatrix4 *)&m;
#else
    Isgl3dMatrix4 m = { matrix.m[0], matrix.m[4], matrix.m[8], matrix.m[12],
        matrix.m[1], matrix.m[5], matrix.m[9], matrix.m[13],
        matrix.m[2], matrix.m[6], matrix.m[10], matrix.m[14],
        matrix.m[3], matrix.m[7], matrix.m[11], matrix.m[15] };
    return m;
#endif
}

static __inline__ Isgl3dMatrix4 Isgl3dMatrix4Multiply(Isgl3dMatrix4 matrixLeft, Isgl3dMatrix4 matrixRight)
{
#if defined(__ARM_NEON__)
    float32x4x4_t iMatrixLeft = *(float32x4x4_t *)&matrixLeft;
    float32x4x4_t iMatrixRight = *(float32x4x4_t *)&matrixRight;
    float32x4x4_t m;
    
    m.val[0] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[0], 0));
    m.val[1] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[1], 0));
    m.val[2] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[2], 0));
    m.val[3] = vmulq_n_f32(iMatrixLeft.val[0], vgetq_lane_f32(iMatrixRight.val[3], 0));
    
    m.val[0] = vmlaq_n_f32(m.val[0], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[0], 1));
    m.val[1] = vmlaq_n_f32(m.val[1], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[1], 1));
    m.val[2] = vmlaq_n_f32(m.val[2], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[2], 1));
    m.val[3] = vmlaq_n_f32(m.val[3], iMatrixLeft.val[1], vgetq_lane_f32(iMatrixRight.val[3], 1));
    
    m.val[0] = vmlaq_n_f32(m.val[0], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[0], 2));
    m.val[1] = vmlaq_n_f32(m.val[1], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[1], 2));
    m.val[2] = vmlaq_n_f32(m.val[2], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[2], 2));
    m.val[3] = vmlaq_n_f32(m.val[3], iMatrixLeft.val[2], vgetq_lane_f32(iMatrixRight.val[3], 2));
    
    m.val[0] = vmlaq_n_f32(m.val[0], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[0], 3));
    m.val[1] = vmlaq_n_f32(m.val[1], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[1], 3));
    m.val[2] = vmlaq_n_f32(m.val[2], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[2], 3));
    m.val[3] = vmlaq_n_f32(m.val[3], iMatrixLeft.val[3], vgetq_lane_f32(iMatrixRight.val[3], 3));
    
    return *(Isgl3dMatrix4 *)&m;
#else
    Isgl3dMatrix4 m;
    
    m.m[0]  = matrixLeft.m[0] * matrixRight.m[0]  + matrixLeft.m[4] * matrixRight.m[1]  + matrixLeft.m[8] * matrixRight.m[2]   + matrixLeft.m[12] * matrixRight.m[3];
    m.m[4]  = matrixLeft.m[0] * matrixRight.m[4]  + matrixLeft.m[4] * matrixRight.m[5]  + matrixLeft.m[8] * matrixRight.m[6]   + matrixLeft.m[12] * matrixRight.m[7];
    m.m[8]  = matrixLeft.m[0] * matrixRight.m[8]  + matrixLeft.m[4] * matrixRight.m[9]  + matrixLeft.m[8] * matrixRight.m[10]  + matrixLeft.m[12] * matrixRight.m[11];
    m.m[12] = matrixLeft.m[0] * matrixRight.m[12] + matrixLeft.m[4] * matrixRight.m[13] + matrixLeft.m[8] * matrixRight.m[14]  + matrixLeft.m[12] * matrixRight.m[15];
    
    m.m[1]  = matrixLeft.m[1] * matrixRight.m[0]  + matrixLeft.m[5] * matrixRight.m[1]  + matrixLeft.m[9] * matrixRight.m[2]   + matrixLeft.m[13] * matrixRight.m[3];
    m.m[5]  = matrixLeft.m[1] * matrixRight.m[4]  + matrixLeft.m[5] * matrixRight.m[5]  + matrixLeft.m[9] * matrixRight.m[6]   + matrixLeft.m[13] * matrixRight.m[7];
    m.m[9]  = matrixLeft.m[1] * matrixRight.m[8]  + matrixLeft.m[5] * matrixRight.m[9]  + matrixLeft.m[9] * matrixRight.m[10]  + matrixLeft.m[13] * matrixRight.m[11];
    m.m[13] = matrixLeft.m[1] * matrixRight.m[12] + matrixLeft.m[5] * matrixRight.m[13] + matrixLeft.m[9] * matrixRight.m[14]  + matrixLeft.m[13] * matrixRight.m[15];
    
    m.m[2]  = matrixLeft.m[2] * matrixRight.m[0]  + matrixLeft.m[6] * matrixRight.m[1]  + matrixLeft.m[10] * matrixRight.m[2]  + matrixLeft.m[14] * matrixRight.m[3];
    m.m[6]  = matrixLeft.m[2] * matrixRight.m[4]  + matrixLeft.m[6] * matrixRight.m[5]  + matrixLeft.m[10] * matrixRight.m[6]  + matrixLeft.m[14] * matrixRight.m[7];
    m.m[10] = matrixLeft.m[2] * matrixRight.m[8]  + matrixLeft.m[6] * matrixRight.m[9]  + matrixLeft.m[10] * matrixRight.m[10] + matrixLeft.m[14] * matrixRight.m[11];
    m.m[14] = matrixLeft.m[2] * matrixRight.m[12] + matrixLeft.m[6] * matrixRight.m[13] + matrixLeft.m[10] * matrixRight.m[14] + matrixLeft.m[14] * matrixRight.m[15];
    
    m.m[3]  = matrixLeft.m[3] * matrixRight.m[0]  + matrixLeft.m[7] * matrixRight.m[1]  + matrixLeft.m[11] * matrixRight.m[2]  + matrixLeft.m[15] * matrixRight.m[3];
    m.m[7]  = matrixLeft.m[3] * matrixRight.m[4]  + matrixLeft.m[7] * matrixRight.m[5]  + matrixLeft.m[11] * matrixRight.m[6]  + matrixLeft.m[15] * matrixRight.m[7];
    m.m[11] = matrixLeft.m[3] * matrixRight.m[8]  + matrixLeft.m[7] * matrixRight.m[9]  + matrixLeft.m[11] * matrixRight.m[10] + matrixLeft.m[15] * matrixRight.m[11];
    m.m[15] = matrixLeft.m[3] * matrixRight.m[12] + matrixLeft.m[7] * matrixRight.m[13] + matrixLeft.m[11] * matrixRight.m[14] + matrixLeft.m[15] * matrixRight.m[15];
    
    return m;
#endif
}

static __inline__ Isgl3dMatrix4 Isgl3dMatrix4Add(Isgl3dMatrix4 matrixLeft, Isgl3dMatrix4 matrixRight)
{
#if defined(__ARM_NEON__)
    float32x4x4_t iMatrixLeft = *(float32x4x4_t *)&matrixLeft;
    float32x4x4_t iMatrixRight = *(float32x4x4_t *)&matrixRight;
    float32x4x4_t m;
    
    m.val[0] = vaddq_f32(iMatrixLeft.val[0], iMatrixRight.val[0]);
    m.val[1] = vaddq_f32(iMatrixLeft.val[1], iMatrixRight.val[1]);
    m.val[2] = vaddq_f32(iMatrixLeft.val[2], iMatrixRight.val[2]);
    m.val[3] = vaddq_f32(iMatrixLeft.val[3], iMatrixRight.val[3]);
    
    return *(Isgl3dMatrix4 *)&m;
#else
    Isgl3dMatrix4 m;
    
    m.m[0] = matrixLeft.m[0] + matrixRight.m[0];
    m.m[1] = matrixLeft.m[1] + matrixRight.m[1];
    m.m[2] = matrixLeft.m[2] + matrixRight.m[2];
    m.m[3] = matrixLeft.m[3] + matrixRight.m[3];
    
    m.m[4] = matrixLeft.m[4] + matrixRight.m[4];
    m.m[5] = matrixLeft.m[5] + matrixRight.m[5];
    m.m[6] = matrixLeft.m[6] + matrixRight.m[6];
    m.m[7] = matrixLeft.m[7] + matrixRight.m[7];
    
    m.m[8] = matrixLeft.m[8] + matrixRight.m[8];
    m.m[9] = matrixLeft.m[9] + matrixRight.m[9];
    m.m[10] = matrixLeft.m[10] + matrixRight.m[10];
    m.m[11] = matrixLeft.m[11] + matrixRight.m[11];
    
    m.m[12] = matrixLeft.m[12] + matrixRight.m[12];
    m.m[13] = matrixLeft.m[13] + matrixRight.m[13];
    m.m[14] = matrixLeft.m[14] + matrixRight.m[14];
    m.m[15] = matrixLeft.m[15] + matrixRight.m[15];
    
    return m;
#endif
}

static __inline__ Isgl3dMatrix4 Isgl3dMatrix4Subtract(Isgl3dMatrix4 matrixLeft, Isgl3dMatrix4 matrixRight)
{
#if defined(__ARM_NEON__)
    float32x4x4_t iMatrixLeft = *(float32x4x4_t *)&matrixLeft;
    float32x4x4_t iMatrixRight = *(float32x4x4_t *)&matrixRight;
    float32x4x4_t m;
    
    m.val[0] = vsubq_f32(iMatrixLeft.val[0], iMatrixRight.val[0]);
    m.val[1] = vsubq_f32(iMatrixLeft.val[1], iMatrixRight.val[1]);
    m.val[2] = vsubq_f32(iMatrixLeft.val[2], iMatrixRight.val[2]);
    m.val[3] = vsubq_f32(iMatrixLeft.val[3], iMatrixRight.val[3]);
    
    return *(Isgl3dMatrix4 *)&m;
#else
    Isgl3dMatrix4 m;
    
    m.m[0] = matrixLeft.m[0] - matrixRight.m[0];
    m.m[1] = matrixLeft.m[1] - matrixRight.m[1];
    m.m[2] = matrixLeft.m[2] - matrixRight.m[2];
    m.m[3] = matrixLeft.m[3] - matrixRight.m[3];
    
    m.m[4] = matrixLeft.m[4] - matrixRight.m[4];
    m.m[5] = matrixLeft.m[5] - matrixRight.m[5];
    m.m[6] = matrixLeft.m[6] - matrixRight.m[6];
    m.m[7] = matrixLeft.m[7] - matrixRight.m[7];
    
    m.m[8] = matrixLeft.m[8] - matrixRight.m[8];
    m.m[9] = matrixLeft.m[9] - matrixRight.m[9];
    m.m[10] = matrixLeft.m[10] - matrixRight.m[10];
    m.m[11] = matrixLeft.m[11] - matrixRight.m[11];
    
    m.m[12] = matrixLeft.m[12] - matrixRight.m[12];
    m.m[13] = matrixLeft.m[13] - matrixRight.m[13];
    m.m[14] = matrixLeft.m[14] - matrixRight.m[14];
    m.m[15] = matrixLeft.m[15] - matrixRight.m[15];
    
    return m;
#endif
}

#endif


#pragma mark - iSGL3D specific math
#

#if (TARGET_IPHONE_SIMULATOR == 0) && (TARGET_OS_IPHONE == 1)
#define USE_ACC_MATH

#ifdef _ARM_ARCH_7
#define ISGL3D_MATRIX_MATH_ACCEL @"neon"
#else
#define ISGL3D_MATRIX_MATH_ACCEL @"vfp"
#endif

#endif

#define im4(sxx, sxy, sxz, tx, syx, syy, syz, ty, szx, szy, szz, tz, swx, swy, swz, tw) im4Create(sxx, sxy, sxz, tx, syx, syy, syz, ty, szx, szy, szz, tz, swx, swy, swz, tw)
#define im4Identity() im4CreateIdentity()


#pragma mark -

/**
 * Creates an Isgl3dMatrix4 structure from given values.
 * @result A specified matrix.
 */
static inline Isgl3dMatrix4 im4Create(float sxx, float sxy, float sxz, float tx, float syx, float syy, float syz, float ty, float szx, float szy, float szz, float tz, float swx, float swy, float swz, float tw)
{
	Isgl3dMatrix4 matrix = {
		sxx, syx, szx, swx,
		sxy, syy, szy, swy,
		sxz, syz, szz, swz,
		tx,  ty,  tz,  tw };
	return matrix;	
}

/**
 * Creates an Isgl3dMatrix4 structure with all values set to 0.
 * @result An empty matrix.
 */
static inline Isgl3dMatrix4 im4CreateEmpty()
{
	Isgl3dMatrix4 matrix = {
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0 };
	return matrix;	
}

/**
 * Creates an identity matrix.
 * @result An identity matrix.
 */
static inline Isgl3dMatrix4 im4CreateIdentity()
{
	Isgl3dMatrix4 matrix = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1 };
	return matrix;	
}

/**
 * Creates a matrix structure from a row-major array of 9 values.
 * @param a The row-major array of 9 values.
 * @result A matrix created from the 16 value row-major array.
 */
static inline Isgl3dMatrix4 im4CreateFromArray9(float * a)
{
	Isgl3dMatrix4 matrix = {
		a[0], a[3], a[6], 0,
		a[1], a[4], a[7], 0,
		a[2], a[5], a[8], 0,
		0,    0,    0,    1 };
	return matrix;	
}

/**
 * Creates a matrix structure from a row-major array of 16 values.
 * @param a The row-major array of 16 values.
 * @result A matrix created from the 16 value row-major array.
 */
static inline Isgl3dMatrix4 im4CreateFromArray16(float * a)
{
	Isgl3dMatrix4 matrix = {
		a[0], a[4], a[8],  a[12],
		a[1], a[5], a[9],  a[13],
		a[2], a[6], a[10], a[14],
		a[3], a[7], a[11], a[15] };
	return matrix;	
}

/**
 * Constructs a matrix with values from a column-major float array.
 * The array must contain 16 values.
 * @param a The array of column-major matrix values.
 * @result A matrix created from given OpenGL (column-major) array.
 */
static inline Isgl3dMatrix4 im4CreateFromOpenGL(float * a)
{
	Isgl3dMatrix4 matrix = {
		a[0],  a[1],  a[2],  a[3],
		a[4],  a[5],  a[6],  a[7],
		a[8],  a[9],  a[10], a[11],
		a[12], a[13], a[14], a[15] };
	return matrix;	
}

/**
 * Constructs a matrix used to scale vertices.
 * @param scaleX The scaling along the x-axis.
 * @param scaleY The scaling along the y-axis.
 * @param scaleZ The scaling along the z-axis.
 * @result A new matrix with scale factors.
 */
static inline Isgl3dMatrix4 im4CreateFromScales(float scaleX, float scaleY, float scaleZ)
{
	Isgl3dMatrix4 matrix = {
		scaleX, 0,      0,      0,
		0,      scaleY, 0,      0,
		0,      0,      scaleZ, 0,
		0,      0,      0,      1 };
	return matrix;	
}

/**
 * Constructs a matrix to project all vertices onto a plane from a given position.
 * @param plane Vector representation of the plane.
 * @param position Vector position from which the projection occurs.
 * @result A matrix to flatten vertices onto a plane as projected from a position.
 */
Isgl3dMatrix4 im4PlanarProjectionMatrixFromPosition(Isgl3dVector4 * plane, Isgl3dVector3 * position);

/**
 * Constructs a matrix to project all vertices onto a plane along a given direction.
 * @param plane Vector representation of the plane.
 * @param direction Vector direction along which the projection occurs.
 * @result A matrix to flatten vertices onto a plane as projected along a direction.
 */
Isgl3dMatrix4 im4PlanarProjectionMatrixFromDirection(Isgl3dVector4 * plane, Isgl3dVector3 * direction);

static inline NSString * im4ToString(Isgl3dMatrix4 * m)
{
	return [NSString stringWithFormat:@"\n[%f, %f, %f, %f]\n[%f, %f, %f, %f]\n[%f, %f, %f, %f]\n[%f, %f, %f, %f]",
					m->m00, m->m10, m->m20, m->m30,
					m->m01, m->m11, m->m21, m->m31,
					m->m02, m->m12, m->m22, m->m32,
					m->m03, m->m13, m->m23, m->m33];	
}

/**
 * Returns the determinant of the full matrix.
 * @param m The matrix.
 * @return The determinant of the full matrix.
 */
static inline float im4Determinant(Isgl3dMatrix4 * m)
{
	return	  (m->m00 * m->m11 - m->m01 * m->m10) * (m->m22 * m->m33 - m->m23 * m->m32) 
			- (m->m00 * m->m12 - m->m02 * m->m10) * (m->m21 * m->m33 - m->m23 * m->m31) 
			+ (m->m00 * m->m13 - m->m03 * m->m10) * (m->m21 * m->m32 - m->m22 * m->m31) 
			+ (m->m01 * m->m12 - m->m02 * m->m11) * (m->m20 * m->m33 - m->m23 * m->m30) 
			- (m->m01 * m->m13 - m->m03 * m->m11) * (m->m20 * m->m32 - m->m22 * m->m30) 
			+ (m->m02 * m->m13 - m->m03 * m->m12) * (m->m20 * m->m31 - m->m21 * m->m30);
}

/**
 * Returns the determinant of the 3x3 part of the matrix.
 * @param m The matrix.
 * @return The determinant of the 3x3 part of the matrix.
 */
static inline float im4Determinant3x3(Isgl3dMatrix4 * m)
{
	return	  (m->m00 * m->m11 - m->m01 * m->m10) * m->m22 
			- (m->m00 * m->m12 - m->m02 * m->m10) * m->m21 
			+ (m->m01 * m->m12 - m->m02 * m->m11) * m->m20;	
}

/**
 * Inverts the matrix.
 */
void im4Invert(Isgl3dMatrix4 * m);

/**
 * Inverts the 3x3 part of the matrix.
 */
void im4Invert3x3(Isgl3dMatrix4 * m);

/**
 * Copies the matrix components of one matrix into another.
 * @param a The destination matrix.
 * @param b The source matrix.
 */
static inline void im4Copy(Isgl3dMatrix4 * a, Isgl3dMatrix4 * b)
{
	a->m00 = b->m00;	a->m10 = b->m10;	a->m20 = b->m20;	a->m30  = b->m30;
	a->m01 = b->m01;	a->m11 = b->m11;	a->m21 = b->m21;	a->m31  = b->m31;
	a->m02 = b->m02;	a->m12 = b->m12;	a->m22 = b->m22;	a->m32  = b->m32;
	a->m03 = b->m03;	a->m13 = b->m13;	a->m23 = b->m23;	a->m33  = b->m33;
}

/**
 * Performs a rotation on a matrix around a given axis.
 * @param m The matrix.
 * @param angle The angle of rotation in degrees.
 * @param x The x component of the axis of rotation.
 * @param y The y component of the axis of rotation.
 * @param z The z component of the axis of rotation.
 */
void im4Rotate(Isgl3dMatrix4 * m, float angle, float x, float y, float z);

/**
 * Performs a translation on a matrix by a given distance.
 * @param m The matrix.
 * @param x The distance along the x-axis of the translation.
 * @param y The distance along the y-axis of the translation.
 * @param z The distance along the z-axis of the translation.
 */
void im4Translate(Isgl3dMatrix4 * m, float x, float y, float z);

/**
 * Performs a translation on a matrix by a given vector.
 * @param m The matrix.
 * @param v The vector translation.
 */
void im4TranslateByVector(Isgl3dMatrix4 * m, Isgl3dVector3 * v);

/**
 * Performs a scaling on a matrix.
 * @param m The matrix.
 * @param x The amount of scaling in the x-direction.
 * @param y The amount of scaling in the y-direction.
 * @param z The amount of scaling in the z-direction.
 */
void im4Scale(Isgl3dMatrix4 * m, float x, float y, float z);

/**
 * Trasposes the matrix.
 * @param m The matrix.
 */
void im4Transpose(Isgl3dMatrix4 * m);

/**
 * Sets the rotation of a matrix to a specific angle about a specified axis.
 * The translation components of the matrix are not affected.
 * @param m The matrix.
 * @param angle The angle of rotation in degrees.
 * @param x The x component of the axis of rotation.
 * @param y The y component of the axis of rotation.
 * @param z The z component of the axis of rotation.
 */
void im4SetRotation(Isgl3dMatrix4 * m, float angle, float x, float y, float z);

/**
 * Calculates the rotational component of the matrix from euler angles.
 * The translation components of the matrix are not affected.
 * @param m The matrix.
 * @param ax The rotation about the x axis.
 * @param ay The rotation about the y axis.
 * @param az The rotation about the z axis.
 */
void im4SetRotationFromEuler(Isgl3dMatrix4 * m, float ax, float ay, float az);

/**
 * Calculates the rotational component of the matrix from a quaternion.
 * The translation components of the matrix are not affected.
 * @param m The matrix.
 * @param q The quaternion.
 */
void im4SetRotationFromQuaternion(Isgl3dMatrix4 * m, Isgl3dQuaternion * q);

/**
 * Sets the translation components of a matrix. The rotational components
 * are not affected.
 * @param m The matrix.
 * @param x The distance along the x-axis of the translation.
 * @param y The distance along the y-axis of the translation.
 * @param z The distance along the z-axis of the translation.
 */
static inline void im4SetTranslation(Isgl3dMatrix4 * m, float x, float y, float z)
{
	m->m30 = x;
	m->m31 = y;
	m->m32 = z;	
}

/**
 * Sets the translation components of a matrix stored in a vector. The rotational components
 * are not affected.
 * @param m The matrix.
 * @param translation The translation.
 */
static inline void im4SetTranslationByVector(Isgl3dMatrix4 * m, Isgl3dVector3 * translation)
{
	m->m30 = translation->x;
	m->m31 = translation->y;
	m->m32 = translation->z;	
}

/**
 * Returns the length of the translation of a matrix.
 * @param m The matrix.
 * @return The length of the translation of a matrix.
 */
static inline float im4TranslationLength(Isgl3dMatrix4 * m)
{
	return sqrt(m->m30 * m->m30 + m->m31 * m->m31 + m->m32 * m->m32);	
}

/**
 * Multiplies two matrices. The result is a = a x b.
 * @param a The left-hand matrix, stores result after calculation.
 * @param b The right-hand matrix.
 */
void im4Multiply(Isgl3dMatrix4 * a, Isgl3dMatrix4 * b);

/**
 * Multiplies two matrices. The result is a = b x a.
 * @param a The right-hand matrix, stores result after calculation.
 * @param b The left-hand matrix.
 */
void im4MultiplyOnLeft(Isgl3dMatrix4 * a, Isgl3dMatrix4 * b);

/**
 * Performs a multiplication on a given matrix with it being on the left, using only the 3x3 part of the matrix.
 * The resulting matrix is stored in "a".
 * The translation components of the matrix are not affected.
 * @param a The matrix to be multiplied on the left, result stored in this matrix. 
 * @param b The matrix to be multiplied on the right. 
 */
void im4MultiplyOnLeft3x3(Isgl3dMatrix4 * a, Isgl3dMatrix4 * b);

/**
 * Performs a multiplication by the matrix on the given 4-component vector.
 * @param m The matrix multiplier. 
 * @param vector The vector to be multiplied.
 * @result The result of the multiplication on given vector
 */
Isgl3dVector4 im4MultVector4(Isgl3dMatrix4 * m, Isgl3dVector4 * vector); 

/**
 * Performs a multiplication by the matrix on the given 3-component vector with the translational components of the matrix included.
 * @param m The matrix multiplier. 
 * @param vector The vector to be multiplied.
 * @result The result of the multiplication on given vector
 */
Isgl3dVector3 im4MultVector(Isgl3dMatrix4 * m, Isgl3dVector3 * vector); 

/**
 * Performs a multiplication on the given 3-component vector with the only the 3x3 part of the matrix.
 * @param m The matrix multiplier. 
 * @param vector The vector to be multiplied.
 * @result The result of the multiplication on given vector
 */
Isgl3dVector3 im4MultVector3x3(Isgl3dMatrix4 * m, Isgl3dVector3 * vector); 

/**
 * Performs a multiplication by the matrix on the array equivalent of a 4-component vector.
 * @param m The matrix multiplier. 
 * @param array The array to be multiplied. Result is stored in the same array.
 */
void im4MultArray4(Isgl3dMatrix4 * m, float * array); 


/**
 * Returns the 3x3 part of a matrix as a column-major float array.
 * @param m The matrix. 
 * @param array The float array into which the column-major representation of the matrix is set.
 */
static inline void im4ConvertTo3x3ColumnMajorFloatArray(Isgl3dMatrix4 * m, float * array)
{
	array[0] = m->m00; array[1] = m->m01; array[2] = m->m02;
	array[3] = m->m10; array[4] = m->m11; array[5] = m->m12;
	array[6] = m->m20; array[7] = m->m21; array[8] = m->m22;
}

/**
 * Returns the full matrix as a column-major float array.
 * @param m The matrix. 
 * @param array The float array into which the column-major representation of the matrix is set.
 */
static inline void im4ConvertToColumnMajorFloatArray(Isgl3dMatrix4 * m, float * array)
{
	array[0]  = m->m00; array[1]  = m->m01; array[2]  = m->m02; array[3]  = m->m03;
	array[4]  = m->m10; array[5]  = m->m11; array[6]  = m->m12; array[7]  = m->m13;
	array[8]  = m->m20; array[9]  = m->m21; array[10] = m->m22; array[11] = m->m23;
	array[12] = m->m30;  array[13] = m->m31;  array[14] = m->m32;  array[15] = m->m33;
}


/**
 * Returns the full matrix as a column-major float array.
 * @param m The matrix. 
 */
static inline float* im4ColumnMajorFloatArrayFromMatrix(Isgl3dMatrix4 * m)
{
	return (&m->m00);
}


/**
 * Copies data from a column-major (OpenGL standard) transformation in to a matrix.
 * @param m The matrix. 
 * @param t The column-major transformation as a 16 value float array to be copied.
 */
static inline void im4SetTransformationFromOpenGLMatrix(Isgl3dMatrix4 * m, float * t)
{
	m->m00 = t[ 0]; m->m01 = t[ 1]; m->m02 = t[ 2]; m->m03 = t[ 3];
	m->m10 = t[ 4]; m->m11 = t[ 5]; m->m12 = t[ 6]; m->m13 = t[ 7];
	m->m20 = t[ 8]; m->m21 = t[ 9]; m->m22 = t[10]; m->m23 = t[11];
	m->m30  = t[12]; m->m31  = t[13]; m->m32  = t[14]; m->m33  = t[15];
}

/**
 * Copies a matrix into a column-major (OpenGL standard) float array.
 * @param m The matrix. 
 * @param t The column-major transformation as a 16 value float array that will hold the values of this matrix after the call.
 */
static inline void im4GetTransformationAsOpenGLMatrix(Isgl3dMatrix4 * m, float * t)
{
	t[ 0] = m->m00; t[ 1] = m->m01; t[ 2] = m->m02; t[ 3] = m->m03;
	t[ 4] = m->m10; t[ 5] = m->m11; t[ 6] = m->m12; t[ 7] = m->m13;
	t[ 8] = m->m20; t[ 9] = m->m21; t[10] = m->m22; t[11] = m->m23;
	t[12] = m->m30;  t[13] = m->m31;  t[14] = m->m32;  t[15] = m->m33;
}

/**
 * Returns the equivalent Euler angles of the rotational components of a matrix.
 * @param m The matrix. 
 * @result Vector containing the rotations about x, y and z (in degrees)
 */
Isgl3dVector3 im4ToEulerAngles(Isgl3dMatrix4 * m);

/**
 * Returns the equivalent scaling values of a matrix.
 * @param m The matrix. 
 * @result Vector containing the scalex in x, y and z
 */
Isgl3dVector3 im4ToScaleValues(Isgl3dMatrix4 * m);


/**
 * Returns the equivalent scaling values of a matrix.
 * @param m The matrix. 
 * @result Vector containing the scalex in x, y and z
 */
static inline Isgl3dVector3 im4ToPosition(Isgl3dMatrix4 * m) 
{
	return iv3(m->m30, m->m31, m->m32);
}
