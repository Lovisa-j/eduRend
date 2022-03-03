
//
//  Model.h
//
//  Carl Johan Gribel 2016, cjgribel@gmail.com
//

#pragma once
#ifndef MODEL_H
#define MODEL_H

#include "stdafx.h"
#include <vector>
#include "vec\vec.h"
#include "vec\mat.h"
#include "ShaderBuffers.h"
#include "Drawcall.h"
#include "OBJLoader.h"
#include "Texture.h"

using namespace linalg;
struct PhongBuffer
{
	vec4f kd;
	vec4f ka;
	vec4f ks;
};


class Model
{
	
protected:



	
	// Pointers to the current device and device context
	ID3D11Device* const			dxdevice;
	ID3D11DeviceContext* const	dxdevice_context;

	ID3D11Buffer* phongShader_buffer = nullptr;
	// Pointers to the class' vertex & index arrays
	ID3D11Buffer* vertex_buffer = nullptr;
	ID3D11Buffer* index_buffer = nullptr;

	

public:


	Model(
		ID3D11Device* dxdevice, 
		ID3D11DeviceContext* dxdevice_context) 
		:	dxdevice(dxdevice),
			dxdevice_context(dxdevice_context)

	{ 

		
		

		HRESULT hr;
		D3D11_BUFFER_DESC MatrixBuffer_desc = { 0 };
		MatrixBuffer_desc.Usage = D3D11_USAGE_DYNAMIC;
		MatrixBuffer_desc.ByteWidth = sizeof(PhongBuffer);
		MatrixBuffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		MatrixBuffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		MatrixBuffer_desc.MiscFlags = 0;
		MatrixBuffer_desc.StructureByteStride = 0;
		ASSERT(hr = dxdevice->CreateBuffer(&MatrixBuffer_desc, nullptr, &phongShader_buffer));
	
	}

	//
	// Abstract render method: must be implemented by derived classes
	//
	virtual void Render() const = 0;

	//
	// Destructor
	//
	virtual ~Model()
	{ 
		SAFE_RELEASE(vertex_buffer);
		SAFE_RELEASE(index_buffer);
		SAFE_RELEASE(phongShader_buffer);
	}
};

class QuadModel : public Model
{
	unsigned nbr_indices = 0;

public:

	QuadModel(
		ID3D11Device* dx3ddevice,
		ID3D11DeviceContext* dx3ddevice_context);

	virtual void Render() const;

	~QuadModel() { }
};

class OBJModel : public Model
{
	// index ranges, representing drawcalls, within an index array
	struct IndexRange
	{
		unsigned int start;
		unsigned int size;
		unsigned ofs;
		int mtl_index;
	};

	std::vector<IndexRange> index_ranges;
	std::vector<Material> materials;

	void append_materials(const std::vector<Material>& mtl_vec)
	{
		materials.insert(materials.end(), mtl_vec.begin(), mtl_vec.end());
	}

public:

	OBJModel(
		const std::string& objfile,
		ID3D11Device* dxdevice,
		ID3D11DeviceContext* dxdevice_context);

	virtual void Render() const;
	void PhongRender(ID3D11Buffer& phong_buffer);
	~OBJModel();
};

#endif