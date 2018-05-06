#pragma once
#include "../Headers/Headers.h"
class VulkanRenderer;

class VulkanDrawable
{
private:
	//Command buffer for drawing
	std::vector<VkCommandBuffer> vectorCommandDraw;

	//Prepares render pass instance
	void recordCommandBuffer(int currentImage, VkCommandBuffer* commandDraw);

	//Viewport and Scissor variables
	VkViewport viewport;
	VkRect2D scissor;
	VkSemaphore presentCompleteSemaphore;
	VkSemaphore drawingCompleteSemaphore;

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

	//Allocate, create, record command buffer
	void prepare();
	//Render the object
	void render();

	//Initialize the viewport parameters
	void initViewports(VkCommandBuffer* commandBuffer);

	//Initialize the scissor parameters here
	void initScissors(VkCommandBuffer* commandBuffer);

	//Destroy the drawing command buffer object
	void destroyCommandBuffer();
};