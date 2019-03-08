#include "Vulkan/V_MeshFactory.h"
#include "Vulkan/Vulkan_Headers.h"
#include "Vulkan/V_BufferHelper.h"

//Builds and attaches a vertex or index buffer to a vulkan mesh
void V_MeshFactory::buildVertexBuffer(v_mesh & mesh, std::vector<vertex> &vertices, V_CommandPool * transferPool, V_Device * device)
{
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	//Create a staging buffer to hold the vertices
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	V_BufferHelper::createBuffer(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	//Copy the vertices over
	void* data;
	vkMapMemory(device->getLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, vertices.data(), (size_t)bufferSize);
	vkUnmapMemory(device->getLogicalDevice(), stagingBufferMemory);

	//Create a vertex buffer to hold the final vertices
	V_BufferHelper::createBuffer(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mesh.vBuffer, mesh.vBufferVRAM);

	//Copy from the staging buffer to the vertex buffer
	V_BufferHelper::copyBuffer(transferPool, stagingBuffer, mesh.vBuffer, bufferSize);

	//Free the staging buffer
	vkDestroyBuffer(device->getLogicalDevice(), stagingBuffer, nullptr);
	vkFreeMemory(device->getLogicalDevice(), stagingBufferMemory, nullptr);
}
void V_MeshFactory::buildIndexBuffer(v_mesh & mesh, std::vector<uint16_t>& indices, V_CommandPool * transferPool, V_Device * device)
{
	//Get size from index vector
	VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

	//Set the meshes index count
	mesh.indicesCount = (int) indices.size();

	//Create a staging buffer
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	V_BufferHelper::createBuffer(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	//Copy indices to staging buffer
	void* data;
	vkMapMemory(device->getLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices.data(), (size_t)bufferSize);
	vkUnmapMemory(device->getLogicalDevice(), stagingBufferMemory);

	//Create an index buffer
	V_BufferHelper::createBuffer(device, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mesh.indexBuffer, mesh.iBufferVRAM);

	//Transfer data from staging buffer to index buffer
	V_BufferHelper::copyBuffer(transferPool, stagingBuffer, mesh.indexBuffer, bufferSize);

	//Free staging buffer
	vkDestroyBuffer(device->getLogicalDevice(), stagingBuffer, nullptr);
	vkFreeMemory(device->getLogicalDevice(), stagingBufferMemory, nullptr);
}

//Uses a filename to load a mesh
void V_MeshFactory::loadFromFile(const char * fileName, v_mesh & mesh, configurationStructure &config)
{
	//Output information
	std::cout << "\tLoading model at " << fileName << std::endl;

	//Attempt to open file
	FILE* file;
	errno_t err;
	err = fopen_s(&file, fileName, "r");
	if (file == NULL) {
		std::cout << "File open error!" << std::endl;
		return;
	}

	//Temporary variables for lines of the ice
	vertex tempVertex = vertex();
	unsigned int i1, i2, i3;

	std::vector<vertex> vertices = std::vector<vertex>();
	std::vector<uint16_t> indices = std::vector<uint16_t>();

	//Loop through the lines of the ice and store them into vectors
	while (true) {
		//Determine the type of each line
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) {
			break;
		}
		else {
			if (strcmp(lineHeader, "v") == 0) {
				//Store vertex information into the vertex vector
				fscanf(file, "%f %f %f, %f %f %f, %f %f %f, %f %f,\n", &tempVertex.position.x, &tempVertex.position.y, &tempVertex.position.z,
					&tempVertex.normal.x, &tempVertex.normal.y, &tempVertex.normal.z,
					&tempVertex.tangent.x, &tempVertex.tangent.y, &tempVertex.tangent.z,
					&tempVertex.uv.x, &tempVertex.uv.y);
				
				vertices.push_back(tempVertex);
			}
			else if (strcmp(lineHeader, "i") == 0) {
				//Store vertex normal information into the vertex vector
				fscanf(file, "%u %u %u\n", &i1, &i2, &i3);
				indices.push_back(static_cast<uint16_t>(i1));
				indices.push_back(static_cast<uint16_t>(i2));
				indices.push_back(static_cast<uint16_t>(i3));
			}
		}
	}
	fclose(file);

	//Build the vulkan objects stored in the mesh
	buildVertexBuffer(mesh, vertices, config.apiInfo.v_Instance->getTransferCommandPool(), config.apiInfo.v_Instance->getPrimaryDevice());
	buildIndexBuffer(mesh, indices, config.apiInfo.v_Instance->getTransferCommandPool(), config.apiInfo.v_Instance->getPrimaryDevice());
}

V_MeshFactory::V_MeshFactory()
{
}
V_MeshFactory::~V_MeshFactory()
{
}