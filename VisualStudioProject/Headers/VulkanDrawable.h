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
	VulkanRenderer* rendererObj;

	//---------Functions-----------
	VulkanDrawable(VulkanRenderer* parent);
	~VulkanDrawable();

	void destroyCommandBuffer();

	void destroySynchronizationObjects();

	void createVertexBuffer(const void *vertexData, uint32_t dataSize, uint32_t dataStride, bool useTexture);
	void initViewports(VkCommandBuffer * cmd);
	void initScissors(VkCommandBuffer * cmd);
	void destroyVertexBuffer();

	void prepare();
	void render();

	void recordCommandBuffer(int currentImage, VkCommandBuffer* commandDraw);
};