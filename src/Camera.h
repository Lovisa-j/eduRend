
//
//  Camera.h
//
//	Basic camera class
//

#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "vec\vec.h"
#include "vec\mat.h"
#include "vec\math.h"

using namespace linalg;

class Camera
{
public:
	// Aperture attributes
	float vfov, aspect;	
	
	// Clip planes in view space coordinates
	// Evrything outside of [zNear, zFar] is clipped away on the GPU side
	// zNear should be > 0
	// zFar should depend on the size of the scene
	// This range should be kept as tight as possibly to improve
	// numerical precision in the z-buffer
	float zNear, zFar;	
	float yaw, pitch;
	vec3f position;
	InputHandler input_handler;

	Camera(
		float vfov,
		float aspect,
		float zNear,
		float zFar) :
		vfov(vfov), aspect(aspect), zNear(zNear), zFar(zFar)
	
	{
		position = {0.0f, 0.0f, 0.0f};
	}

	// Move to an absolute position
	//
	void moveTo(const vec3f& p)
	{
		position = p;
	}

	vec4f Get_Position() {
		
		return vec4f(position, 1);
	}
	// Move relatively
	//
	void move(const vec3f& v)
	{
		position += (get_ViewToWorldMatrix() * v.xyz0()).xyz();	
	}

	void rotate(float p_yaw, float p_pitch) {
		yaw += p_yaw;
		pitch += p_pitch;

		//limit pitch
		if (pitch <= -fPI/2) {
			pitch = -fPI/2;
		}

		if (pitch >= fPI/2) {
			pitch = fPI/2;
		}
	}
	// Return World-to-View matrix for this camera
	//
	mat4f get_WorldToViewMatrix()
	{
		//vec3f rotation = get_rotation();
		// Assuming a camera's position and rotation is defined by matrices T(p) and R,
		// the View-to-World transform is T(p)*R (for a first-person style camera).
		//
		// World-to-View then is the inverse of T(p)*R;
		//		inverse(T(p)*R) = inverse(R)*inverse(T(p)) = transpose(R)*T(-p)
		// Since now there is no rotation, this matrix is simply T(-p)
		mat4f R = mat4f::rotation(0, yaw, pitch);
		mat4f P =  mat4f::translation(-position);

		return linalg::transpose(R) * (P);
	}

	mat4f get_ViewToWorldMatrix() {
		
		mat4f R = mat4f::rotation(0, yaw, pitch);
		mat4f P = mat4f::translation(position);

		return P * R;
	}

	// Matrix transforming from View space to Clip space
	// In a performance sensitive situation this matrix should be precomputed
	// if possible
	//
	mat4f get_ProjectionMatrix()
	{
		return mat4f::projection(vfov, aspect, zNear, zFar);
	}
};

#endif