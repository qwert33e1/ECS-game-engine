#pragma once
#include "Material.h"

class PhongShader : public GPUProgram
{
	const char *vertSource = R"(
	#version 330
	uniform mat4 MVP, M, Minv; 
	uniform vec4 wLiPos; 
	uniform vec3 wEye; 
	
	layout(location = 0) in vec3 vtxPos;
	layout(location = 1) in vec3 vtxNorm;
	layout(location = 2) in vec2 vtxTex;
	
	out vec3 wNormal;
	out vec3 wView;
	out vec3 wLight;
	out vec2 tex;
	out vec3 wPos;
	
	void main() {
		gl_Position = MVP * vec4(vtxPos, 1);

		vec4 worldPos = M * vec4(vtxPos, 1);
		wPos = worldPos.xyz / worldPos.w;
		wLight = wLiPos.xyz * worldPos.w - worldPos.xyz * wLiPos.w;
		wView = wEye - wPos;
		wNormal = (vec4(vtxNorm, 0) * Minv).xyz;
		tex = vtxTex;
	}
)";

	const char *fragSource = R"(
	#version 330
	uniform vec3 kd, ks, ka;
	uniform float shine; 
	uniform vec3 La, Le; 
	uniform bool noMat;

	in vec3 wNormal; 
	in vec3 wView; 
	in vec3 wLight;
	in vec3 wPos;
	in vec2 tex;

	out vec4 fragColor;

	void main() {
		vec3 fKd, fKa, fKs;
		float fShine;

		if (noMat) {
			vec2 pos = tex * 100.0; 
			bool isWhite = (int(floor(pos.x)) + int(floor(pos.y))) % 2 == 0;        
			fKd = isWhite ? vec3(0.4, 0.4, 0.4) : vec3(0.3, 0.1, 0.0);
			fKa = fKd * 3.0; 
			fKs = vec3(0, 0, 0);
			fShine = 1.0;
		}
		else {
			fKd = kd;
			fKa = ka; 
			fKs = ks;
			fShine = shine;
		}	

		vec3 N = normalize(wNormal);
		vec3 V = normalize(wView);
		vec3 L = normalize(wLight);
		vec3 H = normalize(L + V);


		float cost = max(dot(N,L), 0), cosd = max(dot(N,H), 0);

		vec3 color = fKa * La;
			
		color += (fKd * cost + fKs * pow(cosd,shine)) * Le;

		fragColor = vec4(color, 1);
	}
)";

public:
	PhongShader() { create(vertSource, fragSource); }

	void Bind(RenderState state)
	{
		Use();
		setUniform(state.MVP, "MVP");
		setUniform(state.M, "M");
		setUniform(state.Minv, "Minv");
		setUniform(state.wEye, "wEye");

		if (state.material != nullptr)
		{
			setUniform(false, "noMat");
			setUniform(state.material->kd, "kd");
			setUniform(state.material->ks, "ks");
			setUniform(state.material->ka, "ka");
			setUniform(state.material->shine, "shine");
		}
		else
		{
			setUniform(true, "noMat");
		}

		setUniform(state.light.wLightPos, "wLiPos");
		setUniform(state.light.La, "La");
		setUniform(state.light.Le, "Le");
	}
};