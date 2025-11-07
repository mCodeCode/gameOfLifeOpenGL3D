#version 330 core
out vec4 FragColor;

in vec4 voxelColor;


void main()
{
    FragColor = voxelColor;
}