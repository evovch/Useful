#ifndef CLS_VULKANRENDERER_H
#define CLS_VULKANRENDERER_H

// STD
#include <vector>

// VULKAN
#include <vulkan/vulkan.h>

class cls_VulkanRenderer
{
public:
	cls_VulkanRenderer();
	~cls_VulkanRenderer();

	void Init(void);

	void Deconstruct(void);

private:

	VkResult CreateVulkanInstance(void);
	VkResult EnumeratePhysicalDevices(void);
	VkResult CreateLogicalDevices(void);
	VkResult CreateCommandBuffer(void);

private:

	VkInstance mVulkanInstance;

	std::vector<VkPhysicalDevice> mGPUs;
	// Number of physical devices (gpu_count) can be extracted as the size of this vector

	std::vector<VkQueueFamilyProperties> mQueueProps;

	uint32_t mGraphics_queue_family_index;

	VkDevice mDevice;

	VkCommandPool mCmdPool;

	VkCommandBuffer mCmd;

};

#endif // CLS_VULKANRENDERER_H
