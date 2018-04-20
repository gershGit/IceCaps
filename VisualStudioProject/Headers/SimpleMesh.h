#pragma once

struct VertexWithColor
{
	float x, y, z, w;   // Vertex Position
	float r, g, b, a;   // Color format Red, Green, Blue, Alpha
};

struct VertexWithUV
{
	float x, y, z, w;   // Vertex Position
	float u, v;         // Texture format U,V
};

static const VertexWithColor triangleData[] =
{
	{ 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0 },
	{ 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0 },
	{ -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0 },
};