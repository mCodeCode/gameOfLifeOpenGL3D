#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 instancePos;
layout (location = 2) in vec3 voxelColorInput;


uniform mat4 view;
uniform mat4 projection;
uniform float voxelSize;

out vec3 voxelColor;



void main()
{

	//apply voxel color
	voxelColor = voxelColorInput;

	//change voxel size
	vec3 scaledPos = aPos * voxelSize;

	//apply matrix transformation(s) to vertex positions
	gl_Position = projection * view * vec4(scaledPos + instancePos, 1.0);

}