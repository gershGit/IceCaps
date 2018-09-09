#pragma once
#include "../Headers/Headers.h"
class VulkanRenderer;

class VulkanDrawable
{
private:
	std::vector<VkCommandBuffer> vecCmdDraw;			// Command buffer for drawing
	void recordCommandBuffer(int currentImage, VkCommandBuffer* cmdDraw);
	VkViewport viewport;
	VkRect2D   scissor;
	VkSemaphore presentCompleteSemaphore;
	VkSemaphore drawingCompleteSemaphore;

	VulkanRenderer* rendererObj;
	VkPipeline*		pipeline;

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

	//---------Functions-----------
	VulkanDrawable(VulkanRenderer* parent);
	~VulkanDrawable();

	void setPipeline(VkPipeline* vulkanPipeline) { pipeline = vulkanPipeline; }
	VkPipeline* getPipeline() { return pipeline; }

	void destroySynchronizationObjects();

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