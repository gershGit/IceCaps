#pragma once
#include "../Headers/Headers.h"
class VulkanRenderer;

class VulkanDrawable
{
private:
	//Command buffer for drawing
	std::vector<VkCommandBuffer> vectorCommandDraw;

public:
	// Structure storing vertex buffer metadata
	struct {
		VkBuffer buf;
		VkDeviceMemory mem;
		VkDescriptorBufferInfo bufferInfo;
	} VertexBuffer;

	// Stores the vertex input rate
	VkVertexInputBindingDescription		viIpBind;
	// Store metadata helpful in data interpretation
	VkVertexInputAttributeDescription	viIpAttrb[2];
	VulkanRenderer* rendererObj;

	//---------Functions-----------
	VulkanDrawable(VulkanRenderer* parent = 0);
	~VulkanDrawable();

	void createVertexBuffer(const void *vertexData, uint32_t dataSize, uint32_t dataStride, bool useTexture);
	void destroyVertexBuffer();

	void prepare();
	void render();

	void recordCommandBuffer(int currentImage, VkCommandBuffer* commandDraw);
};