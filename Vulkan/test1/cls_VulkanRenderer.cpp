#include "cls_VulkanRenderer.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

cls_VulkanRenderer::cls_VulkanRenderer()
{
}

cls_VulkanRenderer::~cls_VulkanRenderer()
{
}

void cls_VulkanRenderer::Init(void)
{
	this->CreateVulkanInstance();
	this->EnumeratePhysicalDevices();
	this->CreateLogicalDevices();
	this->CreateCommandBuffer();
}

void cls_VulkanRenderer::Deconstruct(void)
{
	vkDestroyDevice(mDevice, NULL); // #FF0000
	vkDestroyInstance(mVulkanInstance, NULL); // #FF0000
}

VkResult cls_VulkanRenderer::CreateVulkanInstance(void)
{
	// initialize the VkApplicationInfo structure
	VkApplicationInfo app_info = {};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = NULL;
	app_info.pApplicationName = "OvchVulkanRendererApp";
	app_info.applicationVersion = 1;
	app_info.pEngineName = "OvchVulkanRendererEng";
	app_info.engineVersion = 1;
	app_info.apiVersion = VK_API_VERSION_1_0;

	// initialize the VkInstanceCreateInfo structure
	VkInstanceCreateInfo inst_info = {};
	inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	inst_info.pNext = NULL;
	inst_info.flags = 0;
	inst_info.pApplicationInfo = &app_info;
	inst_info.enabledExtensionCount = 0;
	inst_info.ppEnabledExtensionNames = NULL;
	inst_info.enabledLayerCount = 0;
	inst_info.ppEnabledLayerNames = NULL;

	VkResult res;

	res = vkCreateInstance(&inst_info, NULL, &mVulkanInstance); // #FF0000

	if (res == VK_ERROR_INCOMPATIBLE_DRIVER) {
		cerr << "cannot find a compatible Vulkan ICD" << endl;
		exit(EXIT_FAILURE);
	} else if (res) {
		cerr << "unknown error" << endl;
		exit(EXIT_FAILURE);
	}

	cerr << "Vulkan instance created" << endl;

	return res;
}

VkResult cls_VulkanRenderer::EnumeratePhysicalDevices(void)
{
	VkResult res;

	// =============================================================================
	// #FFFF00 First check the number of physical devices

	uint32_t gpu_count = 0;
	res = vkEnumeratePhysicalDevices(mVulkanInstance, &gpu_count, NULL); // #FF0000

	if (res != VK_SUCCESS) {
		cerr << "failed to enumerate physical devices" << endl;
		exit(EXIT_FAILURE);
	} else if (gpu_count == 0) {
		cerr << "num of phys. devices = 0" << endl;
		exit(EXIT_FAILURE);
	}

	cerr << "num of phys. devices = " << gpu_count << endl;

	mGPUs.resize(gpu_count);

	// =============================================================================
	// #FFFF00 Then actually get them

	res = vkEnumeratePhysicalDevices(mVulkanInstance, &gpu_count, mGPUs.data()); // #FF0000

	if (res != VK_SUCCESS) {
		cerr << "failed to enumerate physical devices" << endl;
		exit(EXIT_FAILURE);
	} else if (gpu_count == 0) {
		// I don't know how can this be possible
		cerr << "num of phys. devices = 0" << endl;
		exit(EXIT_FAILURE);
	}

	return res;
}

VkResult cls_VulkanRenderer::CreateLogicalDevices(void)
{
	VkResult res;

	//TODO currently implementation assumes that there is only one GPU, so use mGPUs[0]

	// =============================================================================
	// #FFFF00 First check the number of queue families

	uint32_t queue_family_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(mGPUs[0], &queue_family_count, NULL); // #FF0000

	if (queue_family_count == 0) {
		cerr << "num of queue families = 0" << endl;
		exit(EXIT_FAILURE);
	}

	cerr << "num of queue families = " << queue_family_count << endl;

	mQueueProps.resize(queue_family_count);

	// =============================================================================
	// #FFFF00 Then actually get them

    vkGetPhysicalDeviceQueueFamilyProperties(mGPUs[0], &queue_family_count, mQueueProps.data()); // #FF0000

	if (queue_family_count == 0) {
		cerr << "num of queue families = 0" << endl;
		exit(EXIT_FAILURE);
	}

	// =============================================================================
	// #FFFF00 Search for a device with the VK_QUEUE_GRAPHICS_BIT

	VkDeviceQueueCreateInfo queue_info = {};

    bool found = false;
    for (unsigned int i = 0; i < queue_family_count; i++) {
        if (mQueueProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queue_info.queueFamilyIndex = i;
            mGraphics_queue_family_index = i;
            found = true;
            break;
        }
    }

	if (found == false) {
		cerr << "no device found with VK_QUEUE_GRAPHICS_BIT" << endl;
		exit(EXIT_FAILURE);
	} else {
		cerr << "found device with VK_QUEUE_GRAPHICS_BIT. index = " << mGraphics_queue_family_index << endl;
	}

	// =============================================================================
	// #FFFF00 Create the logical device

    float queue_priorities[1] = {0.0};
    queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_info.pNext = NULL;
    queue_info.queueCount = 1;
    queue_info.pQueuePriorities = queue_priorities;

    VkDeviceCreateInfo device_info = {};
    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_info.pNext = NULL;
    device_info.queueCreateInfoCount = 1;
    device_info.pQueueCreateInfos = &queue_info;
    device_info.enabledExtensionCount = 0;
    device_info.ppEnabledExtensionNames = NULL;
    device_info.enabledLayerCount = 0;
    device_info.ppEnabledLayerNames = NULL;
    device_info.pEnabledFeatures = NULL;

    res = vkCreateDevice(mGPUs[0], &device_info, NULL, &mDevice); // #FF0000

	if (res != VK_SUCCESS) {
		cerr << "failed to create the logical device" << endl;
		exit(EXIT_FAILURE);
	}

	return res;
}

VkResult cls_VulkanRenderer::CreateCommandBuffer(void)
{
	VkResult res;

	// =============================================================================
    // #FFFF00 Create a command pool to allocate our command buffer from

    VkCommandPoolCreateInfo cmd_pool_info = {};
    cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    cmd_pool_info.pNext = NULL;
    cmd_pool_info.queueFamilyIndex = mGraphics_queue_family_index;
    cmd_pool_info.flags = 0;

    res = vkCreateCommandPool(mDevice, &cmd_pool_info, NULL, &mCmdPool); // #FF0000

	if (res != VK_SUCCESS) {
		cerr << "failed to create the command pool" << endl;
		exit(EXIT_FAILURE);
	}

	// =============================================================================
    // #FFFF00 Create the command buffer from the command pool

    VkCommandBufferAllocateInfo cmd = {};
    cmd.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    cmd.pNext = NULL;
    cmd.commandPool = mCmdPool;
    cmd.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    cmd.commandBufferCount = 1;

    res = vkAllocateCommandBuffers(mDevice, &cmd, &mCmd); // #FF0000

	if (res != VK_SUCCESS) {
		cerr << "failed to create the command buffer" << endl;
		exit(EXIT_FAILURE);
	}

	return res;
}
