#pragma once

// STD
#include <vector>

// VULKAN
#include <vulkan/vulkan.h>

// PROJECT
#include "mesh_size_info.hpp"

namespace vulovch {

class Mesh;

class VulkanRenderer {
public:
	VulkanRenderer();
	~VulkanRenderer();
public:
	void Init(
		const uint32_t& n_requested_instance_extensions,
		const char* const* const requested_instance_extensions,
		const uint32_t& n_requested_phys_device_extensions,
		const char* const* const requested_phys_device_extensions);
	void Deconstruct(void);
private:
	void ProcessInputRequests(
		const uint32_t& n_requested_instance_extensions,
		const char* const* const requested_instance_extensions,
		const uint32_t& n_requested_phys_device_extensions,
		const char* const* const requested_phys_device_extensions);
	// VULKAN INSTANCE
	template<typename PROPERTYTYPE>
	VkResult EnumerateInstanceExtensions(std::vector<PROPERTYTYPE>& collection);
	template<typename PROPERTYTYPE>
	VkResult EnumerateInstanceLayers(std::vector<PROPERTYTYPE>& collection);
	VkResult CreateVulkanInstance(void);
	// SURFACE
	VkResult CreateSurface(void);
	// PHYSICAL DEVICE
	template<typename PROPERTYTYPE>
	VkResult EnumeratePhysicalDevices(std::vector<PROPERTYTYPE>& collection);
	void ChoosePhysicalDevice(void);
	template<typename PROPERTYTYPE>
	VkResult EnumeratePhysicalDeviceExtensions(std::vector<PROPERTYTYPE>& collection);
	template<typename PROPERTYTYPE>
	void EnumeratePhysicalDeviceQueueFamilies(std::vector<PROPERTYTYPE>& collection);
	void CheckPhysicalDeviceMemory(void);
	void ChooseGraphicsQueueFamilyIndex(void);
	// LOGICAL DEVICE
	VkResult CreateLogicalDevice(void);
	// SWAPCHAIN
	VkResult CheckSwapchainSupportDetails(void);
	VkResult CreateSwapchain(void);
	// PIPELINE
	const char* const mShaderPathVert = "shaders/vert.spv";
	const char* const mShaderPathFrag = "shaders/frag.spv";
	VkShaderModule CreateShaderModule(const std::vector<char>& source);
	VkResult CreatePipeline(void);
	// COMMAND POOL
	VkResult CreateCommandPool(void);
	// COMMAND BUFFER
	VkResult CreateCommandBuffer(void);
	// BUFFERS
	void CreateBuffers(const mesh_size_info_t& size_info);
private:
	VkResult CreateBufferVertex(const mesh_size_info_t& size_info, VkMemoryRequirements& o_mem_req);
	VkResult CreateBufferIndex(const mesh_size_info_t& size_info, VkMemoryRequirements& o_mem_req);
	uint32_t ChooseMemoryType(const VkMemoryRequirements& mem_req, const VkMemoryPropertyFlags& req_flags);
	VkResult CreateDeviceMemoryVertex(const mesh_size_info_t& size_info, const VkMemoryRequirements& mem_req);
	VkResult CreateDeviceMemoryIndex(const mesh_size_info_t& size_info, const VkMemoryRequirements& mem_req);
	VkResult BindAndMapMemoryToBufferVertex(const mesh_size_info_t& size_info);
	VkResult BindAndMapMemoryToBufferIndex(const mesh_size_info_t& size_info);
private:
	// VULKAN INSTANCE
	std::vector<const char*> mRequestedInstanceExtensions;
	std::vector<const char*> mRequestedInstanceLayers;
	std::vector<VkExtensionProperties> mInstanceExtensions;
	std::vector<VkLayerProperties> mInstanceLayers;
	VkInstance mVulkanInstance;
	// SURFACE
	const char* const mSurfaceExtName = "VK_KHR_win32_surface";
	VkSurfaceKHR mSurface;
	// PHYSICAL DEVICE
	std::vector<const char*> mRequestedPhysDeviceExtensions;
	std::vector<VkPhysicalDevice> mPhysDevices;
	uint32_t mChosenPhysDevice;
	std::vector<VkExtensionProperties> mPhysDeviceExtensions;
	std::vector<VkQueueFamilyProperties> mPhysDeviceQueueFamilies;
	uint32_t mGraphicsQueueFamilyIndex;
	// LOGICAL DEVICE
	VkDevice mLogDevice;
	// SWAPCHAIN
	VkSwapchainKHR mSwapchain;
	// PIPELINE
	VkPipeline mPipeline;
	// COMMAND POOL
	VkCommandPool mCmdPool;
	// COMMAND BUFFER
	VkCommandBuffer mCmdBuf;
	// BUFFERS
	VkBuffer mBufferVertex;
	VkDeviceMemory mDeviceMemoryVertex;
	void* mHostMemoryVertex;
	VkBuffer mBufferIndex;
	VkDeviceMemory mDeviceMemoryIndex;
	void* mHostMemoryIndex;
	// IMAGE VIEW & FRAMEBUFFER
	// RENDER PASS
public:
	void SetMesh(const Mesh* const mesh) { _mesh = mesh; }
private:
	const Mesh* _mesh;
};

} // end of namespace vulovch
