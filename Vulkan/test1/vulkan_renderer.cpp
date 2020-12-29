#include "vulkan_renderer.h"

// STD
#include <iostream>
using std::cerr;
using std::endl;

// VULKAN
#include <windows.h>
#include <vulkan/vulkan_win32.h>

// PROJECT
#include "vk_tools.hpp"
#include "geo_str.hpp"
#include "mesh.hpp"

namespace vulovch {

VulkanRenderer::VulkanRenderer() {
}

VulkanRenderer::~VulkanRenderer() {
}

void VulkanRenderer::Init(
	const uint32_t& n_requested_instance_extensions,
	const char* const* const requested_instance_extensions,
	const uint32_t& n_requested_phys_device_extensions,
	const char* const* const requested_phys_device_extensions) {
	this->ProcessInputRequests(
		n_requested_instance_extensions, requested_instance_extensions,
		n_requested_phys_device_extensions, requested_phys_device_extensions);
	// VULKAN INSTANCE
	this->EnumerateInstanceExtensions(mInstanceExtensions);
	this->EnumerateInstanceLayers(mInstanceLayers);
	this->CreateVulkanInstance();
	// SURFACE
	this->CreateSurface();
	// PHYSICAL DEVICE
	this->EnumeratePhysicalDevices(mPhysDevices);
	this->ChoosePhysicalDevice();
	this->EnumeratePhysicalDeviceExtensions(mPhysDeviceExtensions);
	this->EnumeratePhysicalDeviceQueueFamilies(mPhysDeviceQueueFamilies);
	this->CheckPhysicalDeviceMemory();
	this->ChooseGraphicsQueueFamilyIndex();
	// LOGICAL DEVICE
	this->CreateLogicalDevice();
	// SWAPCHAIN
	//this->CheckSwapchainSupportDetails();
	//this->CreateSwapchain();
	// PIPELINE
	this->CreatePipeline();
	// COMMAND POOL
	this->CreateCommandPool();
	// COMMAND BUFFER
	this->CreateCommandBuffer();
	// BUFFERS
	this->CreateBuffers(_mesh->GetSizeInfo());
}

void VulkanRenderer::Deconstruct(void) {
	vkUnmapMemory(mLogDevice, mDeviceMemoryVertex);
	vkUnmapMemory(mLogDevice, mDeviceMemoryIndex);
	vkDestroyBuffer(mLogDevice, mBufferVertex, nullptr);
	vkDestroyBuffer(mLogDevice, mBufferIndex, nullptr);
	vkFreeCommandBuffers(mLogDevice, mCmdPool, 1, &mCmdBuf);
	vkDestroyCommandPool(mLogDevice, mCmdPool, nullptr);
	vkDestroyDevice(mLogDevice, nullptr);
	vkDestroyInstance(mVulkanInstance, nullptr);
}

void VulkanRenderer::ProcessInputRequests(
	const uint32_t& n_requested_instance_extensions,
	const char* const* const requested_instance_extensions,
	const uint32_t& n_requested_phys_device_extensions,
	const char* const* const requested_phys_device_extensions) {

	// INSTANCE EXTENSIONS
	for (uint32_t i = 0; i < n_requested_instance_extensions; i++) {
		mRequestedInstanceExtensions.push_back(
			requested_instance_extensions[i]);
	}
	cerr << "num of requested Vulkan instance extensions = "
		<< n_requested_instance_extensions << endl;
	tools::Print(mRequestedInstanceExtensions, "requested Vulkan instance extensions");

	// Additional check
	bool necessary_ext_found = false;
	for (uint32_t i = 0; i < n_requested_instance_extensions; i++) {
		if (strcmp(requested_instance_extensions[i], mSurfaceExtName) == 0) {
			necessary_ext_found = true;
			break;
		}
	}
	if (!necessary_ext_found) {
		cerr << "necessary Vulkan instance extension " << mSurfaceExtName
			<< " not requested" << endl;
		exit(EXIT_FAILURE);
	}

	// INSTANCE LAYERS
#ifdef _DEBUG
	constexpr uint32_t n_requested_instance_layers = 1;
	static const char* requested_instance_layers[] = {
		"VK_LAYER_KHRONOS_validation" };
#else
	constexpr uint32_t n_requested_instance_layers = 0;
	static const char* requested_instance_layers = nullptr;
#endif
	for (uint32_t i = 0; i < n_requested_instance_layers; i++) {
		mRequestedInstanceLayers.push_back(requested_instance_layers[i]);
	}
	cerr << "num of requested Vulkan instance layers = "
		<< n_requested_instance_layers << endl;
	tools::Print(mRequestedInstanceLayers, "requested Vulkan instance layers");

	// PHYSICAL DEVICE EXTENSIONS
	for (uint32_t i = 0; i < n_requested_phys_device_extensions; i++) {
		mRequestedPhysDeviceExtensions.push_back(
			requested_phys_device_extensions[i]);
	}
	cerr << "num of requested physical device extensions = "
		<< n_requested_phys_device_extensions << endl;
	tools::Print(mRequestedPhysDeviceExtensions, "requested physical device extensions");
}

template<typename PROPERTYTYPE>
VkResult VulkanRenderer::EnumerateInstanceExtensions(
	std::vector<PROPERTYTYPE>& collection) {
	constexpr char property_name[] = "Vulkan instance extensions";
	VkResult res;
	// ------------------------------------------------------------------------
	uint32_t property_count = 0;
	res = vkEnumerateInstanceExtensionProperties(nullptr, &property_count,
		nullptr);
	if (res != VK_SUCCESS) {
		cerr << "failed to enumerate " << property_name << endl;
		exit(EXIT_FAILURE);
	}/* else if (property_count == 0) {
		cerr << "num of " << property_name << " = 0" << endl;
		exit(EXIT_FAILURE);
	}*/
	cerr << "num of " << property_name << " = " << property_count << endl;
	// ------------------------------------------------------------------------
	collection.resize(property_count);
	// ------------------------------------------------------------------------
	res = vkEnumerateInstanceExtensionProperties(nullptr, &property_count,
		collection.data());
	if (res != VK_SUCCESS) {
		cerr << "failed to enumerate " << property_name << endl;
		exit(EXIT_FAILURE);
	}/* else if (property_count == 0) {
		cerr << "num of " << property_name << " = 0" << endl;
		exit(EXIT_FAILURE);
	}*/
	// ------------------------------------------------------------------------
	tools::Print(collection, property_name);
	// ------------------------------------------------------------------------
	return res;
}

template<typename PROPERTYTYPE>
VkResult VulkanRenderer::EnumerateInstanceLayers(
	std::vector<PROPERTYTYPE>& collection) {
	constexpr char property_name[] = "Vulkan instance layers";
	VkResult res;
	// ------------------------------------------------------------------------
	uint32_t property_count = 0;
	res = vkEnumerateInstanceLayerProperties(&property_count,
		nullptr);
	if (res != VK_SUCCESS) {
		cerr << "failed to enumerate " << property_name << endl;
		exit(EXIT_FAILURE);
	}/* else if (property_count == 0) {
		cerr << "num of " << property_name << " = 0" << endl;
		exit(EXIT_FAILURE);
	}*/
	cerr << "num of " << property_name << " = " << property_count << endl;
	// ------------------------------------------------------------------------
	collection.resize(property_count);
	// ------------------------------------------------------------------------
	res = vkEnumerateInstanceLayerProperties(&property_count,
		collection.data());
	if (res != VK_SUCCESS) {
		cerr << "failed to enumerate " << property_name << endl;
		exit(EXIT_FAILURE);
	}/* else if (property_count == 0) {
		cerr << "num of " << property_name << " = 0" << endl;
		exit(EXIT_FAILURE);
	}*/
	// ------------------------------------------------------------------------
	tools::Print(collection, property_name);
	// ------------------------------------------------------------------------
	return res;
}

VkResult VulkanRenderer::CreateVulkanInstance(void) {
	VkApplicationInfo app_info = {};
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pNext = nullptr;
	app_info.pApplicationName = "OvchVulkanRendererApp";
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "OvchVulkanRendererEng";
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_2;

	VkInstanceCreateInfo inst_info = {};
	inst_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	inst_info.pNext = nullptr;
	inst_info.flags = 0;
	inst_info.pApplicationInfo = &app_info;
	// For now we assume that all requested Vulkan instance layers are supported
	inst_info.enabledLayerCount = static_cast<uint32_t>(
		mRequestedInstanceLayers.size());
	inst_info.ppEnabledLayerNames = mRequestedInstanceLayers.data();
	// For now we assume that all requested Vulkan instance extensions are supported
	inst_info.enabledExtensionCount = static_cast<uint32_t>(
		mRequestedInstanceExtensions.size());
	inst_info.ppEnabledExtensionNames = mRequestedInstanceExtensions.data();

	VkResult res;

	res = vkCreateInstance(&inst_info, nullptr, &mVulkanInstance);

	//TODO implement better checks
	if (res == VK_ERROR_INCOMPATIBLE_DRIVER) {
		cerr << "cannot find a compatible Vulkan ICD" << endl;
		exit(EXIT_FAILURE);
	} else if (res) {
		cerr << "unknown error" << endl;
		exit(EXIT_FAILURE);
	}

	cerr << "successfully created the Vulkan instance" << endl;

	return res;
}

VkResult VulkanRenderer::CreateSurface(void) {

	auto CreateWin32SurfaceKHR = vkGetInstanceProcAddr(mVulkanInstance, "vkCreateWin32SurfaceKHR");

	if (!CreateWin32SurfaceKHR) {
		cerr << "vkCreateWin32SurfaceKHR() not found" << endl;
		exit(EXIT_FAILURE);
	}

	VkWin32SurfaceCreateInfoKHR surf_info = {};
	surf_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surf_info.pNext = nullptr;
	surf_info.flags = 0;
	surf_info.hinstance = GetModuleHandle(nullptr); //TODO ???
	//surf_info.hwnd = ...; //TODO ???

	VkResult res;

	res = vkCreateWin32SurfaceKHR(mVulkanInstance, &surf_info, nullptr, &mSurface);
	if (res != VK_SUCCESS) {
		cerr << "failed to create the surface" << endl;
		exit(EXIT_FAILURE);
	}

	cerr << "successfully created the surface" << endl;

	return res;
}

template<typename PROPERTYTYPE>
VkResult VulkanRenderer::EnumeratePhysicalDevices(
	std::vector<PROPERTYTYPE>& collection) {
	constexpr char property_name[] = "physical devices";
	VkResult res;
	// ------------------------------------------------------------------------
	uint32_t property_count = 0;
	res = vkEnumeratePhysicalDevices(mVulkanInstance, &property_count,
		nullptr);
	if (res != VK_SUCCESS) {
		cerr << "failed to enumerate " << property_name << endl;
		exit(EXIT_FAILURE);
	} else if (property_count == 0) {
		cerr << "num of " << property_name << " = 0" << endl;
		exit(EXIT_FAILURE);
	}
	cerr << "num of " << property_name << " = " << property_count << endl;
	// ------------------------------------------------------------------------
	collection.resize(property_count);
	// ------------------------------------------------------------------------
	res = vkEnumeratePhysicalDevices(mVulkanInstance, &property_count,
		collection.data());
	if (res != VK_SUCCESS) {
		cerr << "failed to enumerate " << property_name << endl;
		exit(EXIT_FAILURE);
	} else if (property_count == 0) {
		cerr << "num of " << property_name << " = 0" << endl;
		exit(EXIT_FAILURE);
	}
	// ------------------------------------------------------------------------
	tools::Print(collection, property_name);
	// ------------------------------------------------------------------------
	return res;
}

void VulkanRenderer::ChoosePhysicalDevice(void) {
	//TODO currently implementation assumes that
	// there is only one GPU, so use mPhysDevices[0]
	mChosenPhysDevice = 0;
}

template<typename PROPERTYTYPE>
VkResult VulkanRenderer::EnumeratePhysicalDeviceExtensions(
	std::vector<PROPERTYTYPE>& collection) {
	constexpr char property_name[] = "physical device extensions";
	VkResult res;
	// ------------------------------------------------------------------------
	uint32_t property_count = 0;
	res = vkEnumerateDeviceExtensionProperties(
		mPhysDevices[mChosenPhysDevice],
		nullptr, &property_count, nullptr);
	if (res != VK_SUCCESS) {
		cerr << "failed to enumerate " << property_name << endl;
		exit(EXIT_FAILURE);
	}/* else if (property_count == 0) {
		cerr << "num of " << property_name << " = 0" << endl;
		exit(EXIT_FAILURE);
	}*/
	cerr << "num of " << property_name << " = " << property_count << endl;
	// ------------------------------------------------------------------------
	collection.resize(property_count);
	// ------------------------------------------------------------------------
	res = vkEnumerateDeviceExtensionProperties(
		mPhysDevices[mChosenPhysDevice],
		nullptr, &property_count, collection.data());
	if (res != VK_SUCCESS) {
		cerr << "failed to enumerate " << property_name << endl;
		exit(EXIT_FAILURE);
	}/* else if (property_count == 0) {
		cerr << "num of " << property_name << " = 0" << endl;
		exit(EXIT_FAILURE);
	}*/
	// ------------------------------------------------------------------------
	tools::Print(collection, property_name);
	//cerr << "..." << endl;
	// ------------------------------------------------------------------------
	return res;
}

template<typename PROPERTYTYPE>
void VulkanRenderer::EnumeratePhysicalDeviceQueueFamilies(
	std::vector<PROPERTYTYPE>& collection) {
	constexpr char property_name[] = "physical device queue families";
	//VkResult res;
	// ------------------------------------------------------------------------
	uint32_t property_count = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(
		mPhysDevices[mChosenPhysDevice],
		&property_count, nullptr);
	/*if (res != VK_SUCCESS) {
		cerr << "failed to enumerate " << property_name << endl;
		exit(EXIT_FAILURE);
	} else if (property_count == 0) {
		cerr << "num of " << property_name << " = 0" << endl;
		exit(EXIT_FAILURE);
	}*/
	cerr << "num of " << property_name << " = " << property_count << endl;
	// ------------------------------------------------------------------------
	collection.resize(property_count);
	// ------------------------------------------------------------------------
	vkGetPhysicalDeviceQueueFamilyProperties(
		mPhysDevices[mChosenPhysDevice],
		&property_count, collection.data());
	/*if (res != VK_SUCCESS) {
		cerr << "failed to enumerate " << property_name << endl;
		exit(EXIT_FAILURE);
	} else if (property_count == 0) {
		cerr << "num of " << property_name << " = 0" << endl;
		exit(EXIT_FAILURE);
	}*/
	// ------------------------------------------------------------------------
	tools::Print(collection, property_name);
	// ------------------------------------------------------------------------
	//return res;
}

void VulkanRenderer::CheckPhysicalDeviceMemory(void) {
	VkPhysicalDeviceMemoryProperties phys_dev_mem_prop;
	vkGetPhysicalDeviceMemoryProperties(mPhysDevices[mChosenPhysDevice],
		&phys_dev_mem_prop);
	tools::Print(phys_dev_mem_prop, "");
}

void VulkanRenderer::ChooseGraphicsQueueFamilyIndex(void) {
	//TODO currently implementation assumes that
	// the first queue family of the chosen physical device
	// supports VK_QUEUE_GRAPHICS_BIT
	mGraphicsQueueFamilyIndex = 0;
}

/*
	// Search for a device with the VK_QUEUE_GRAPHICS_BIT

	VkDeviceQueueCreateInfo queue_info = {};

	bool found = false;
	for (uint32_t i = 0; i < queue_family_count; i++) {
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
		cerr << "found device with VK_QUEUE_GRAPHICS_BIT. index = "
			<< mGraphics_queue_family_index << endl;
	}
*/

VkResult VulkanRenderer::CreateLogicalDevice(void) {

	const float queue_priorities[1] = { 1.0 };

	VkDeviceQueueCreateInfo queue_info = {};
	queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queue_info.pNext = nullptr;
	queue_info.flags = 0;
	queue_info.queueFamilyIndex = mGraphicsQueueFamilyIndex;
	queue_info.queueCount = 1;
	queue_info.pQueuePriorities = queue_priorities;

	VkDeviceCreateInfo device_info = {};
	device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	device_info.pNext = nullptr;
	device_info.flags = 0;
	device_info.queueCreateInfoCount = 1;
	device_info.pQueueCreateInfos = &queue_info;
	device_info.enabledLayerCount = 0;
	device_info.ppEnabledLayerNames = nullptr;
	// For now we assume that all requested physical device extensions are supported
	device_info.enabledExtensionCount = static_cast<uint32_t>(
		mRequestedPhysDeviceExtensions.size());
	device_info.ppEnabledExtensionNames = mRequestedPhysDeviceExtensions.data();
	device_info.pEnabledFeatures = nullptr;

	VkResult res;

	res = vkCreateDevice(mPhysDevices[mChosenPhysDevice],
		&device_info, nullptr, &mLogDevice);

	if (res != VK_SUCCESS) {
		cerr << "failed to create the logical device" << endl;
		exit(EXIT_FAILURE);
	}

	cerr << "successfully created the logical device" << endl;

	return res;
}

VkResult VulkanRenderer::CheckSwapchainSupportDetails(void) {

	VkResult res;

	/*VkPhysicalDeviceSurfaceInfo2KHR surf_info2 = {};
	surf_info2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SURFACE_INFO_2_KHR;
	surf_info2.pNext = nullptr;
	surf_info2.surface = mSurface;
	VkSurfaceCapabilities2KHR surf_capab2 = {};
	surf_capab2.sType = VK_STRUCTURE_TYPE_SURFACE_CAPABILITIES_2_KHR;
	surf_capab2.pNext = nullptr;

	res = vkGetPhysicalDeviceSurfaceCapabilities2KHR(
		mPhysDevices[mChosenPhysDevice], &surf_info2, &surf_capab2);
	if (res != VK_SUCCESS) {
		cerr << "failed to acquire surface capabilities" << endl;
		exit(EXIT_FAILURE);
	}*/

	///*
	VkSurfaceCapabilitiesKHR surf_capab;
	res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
		mPhysDevices[mChosenPhysDevice], mSurface, &surf_capab);
	if (res != VK_SUCCESS) {
		cerr << "failed to acquire surface capabilities" << endl;
		exit(EXIT_FAILURE);
	}
	cerr << "surface capabilities:" << endl;
	tools::Print(surf_capab, "\t");
	//*/

	return res;
}

VkResult VulkanRenderer::CreateSwapchain(void) {

	VkResult res;

	VkSwapchainCreateInfoKHR swapchain_info = {};
	swapchain_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchain_info.pNext = nullptr;
	swapchain_info.surface = mSurface;

	res = vkCreateSwapchainKHR(mLogDevice, &swapchain_info, nullptr, &mSwapchain);

	return res;
}

VkShaderModule VulkanRenderer::CreateShaderModule(const std::vector<char>& source) {
	VkShaderModuleCreateInfo module_info = {};
	module_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	module_info.pNext = nullptr;
	module_info.flags = 0;
	module_info.codeSize = source.size();
	module_info.pCode = reinterpret_cast<const uint32_t*>(source.data());

	VkShaderModule module;

	VkResult res;

	res = vkCreateShaderModule(mLogDevice, &module_info, nullptr, &module);
	if (res != VK_SUCCESS) {
		cerr << "failed to create the shader module" << endl;
		exit(EXIT_FAILURE);
	}

	cerr << "successfully created the shader module" << endl;

	return module;
}

VkResult VulkanRenderer::CreatePipeline(void) {
	/*
	VkResult res;

	const std::vector<char> shader_source_v = tools::ImportShaderBinary(mShaderPathVert);
	const std::vector<char> shader_source_f = tools::ImportShaderBinary(mShaderPathFrag);

	VkShaderModule shader_module_v = this->CreateShaderModule(shader_source_v);
	VkShaderModule shader_module_f = this->CreateShaderModule(shader_source_f);

	vkDestroyShaderModule

	return res;
	*/
	return VK_SUCCESS;
}

VkResult VulkanRenderer::CreateCommandPool(void) {
	VkCommandPoolCreateInfo cmd_pool_info = {};
	cmd_pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmd_pool_info.pNext = nullptr;
	cmd_pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	cmd_pool_info.queueFamilyIndex = mGraphicsQueueFamilyIndex;

	VkResult res;

	res = vkCreateCommandPool(mLogDevice, &cmd_pool_info, nullptr, &mCmdPool);

	if (res != VK_SUCCESS) {
		cerr << "failed to create the command pool" << endl;
		exit(EXIT_FAILURE);
	}

	cerr << "successfully created the command pool" << endl;

	return res;
}

VkResult VulkanRenderer::CreateCommandBuffer(void) {
	VkCommandBufferAllocateInfo cmd_buf_info = {};
	cmd_buf_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmd_buf_info.pNext = nullptr;
	cmd_buf_info.commandPool = mCmdPool;
	cmd_buf_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	cmd_buf_info.commandBufferCount = 1;

	VkResult res;

	res = vkAllocateCommandBuffers(mLogDevice, &cmd_buf_info, &mCmdBuf);

	if (res != VK_SUCCESS) {
		cerr << "failed to create the command buffer" << endl;
		exit(EXIT_FAILURE);
	}

	cerr << "successfully created the command buffer" << endl;

	return res;
}

void VulkanRenderer::CreateBuffers(const mesh_size_info_t& size_info) {
	VkMemoryRequirements mem_req_v;
	this->CreateBufferVertex(_mesh->GetSizeInfo(), mem_req_v);
	this->CreateDeviceMemoryVertex(_mesh->GetSizeInfo(), mem_req_v);
	this->BindAndMapMemoryToBufferVertex(_mesh->GetSizeInfo());

	VkMemoryRequirements mem_req_tr;
	this->CreateBufferIndex(_mesh->GetSizeInfo(), mem_req_tr);
	this->CreateDeviceMemoryIndex(_mesh->GetSizeInfo(), mem_req_tr);
	this->BindAndMapMemoryToBufferIndex(_mesh->GetSizeInfo());
}

// private: ===================================================================

VkResult VulkanRenderer::CreateBufferVertex(const mesh_size_info_t& size_info,
	VkMemoryRequirements& o_mem_req) {
	VkDeviceSize buffer_size = size_info._n_v * sizeof(geo_str::vertex_t);

	VkBufferCreateInfo buf_info = {};
	buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buf_info.pNext = nullptr;
	buf_info.flags = 0;
	buf_info.size = buffer_size;
	buf_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	buf_info.queueFamilyIndexCount = 0;
	buf_info.pQueueFamilyIndices = nullptr;

	VkResult res;

	res = vkCreateBuffer(mLogDevice, &buf_info, nullptr, &mBufferVertex);

	if (res != VK_SUCCESS) {
		cerr << "failed to create the vertex buffer" << endl;
		exit(EXIT_FAILURE);
	}

	cerr << "successfully created the vertex buffer" << endl;

	//VkMemoryRequirements mem_req;
	vkGetBufferMemoryRequirements(mLogDevice, mBufferVertex, &o_mem_req);
	cerr << "vertex buffer memory requirements:" << endl;
	tools::Print(o_mem_req, "\t");

	return res;
}

VkResult VulkanRenderer::CreateBufferIndex(const mesh_size_info_t& size_info,
	VkMemoryRequirements& o_mem_req) {
	VkDeviceSize buffer_size = size_info._n_tr * sizeof(geo_str::triangle_t);

	VkBufferCreateInfo buf_info = {};
	buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buf_info.pNext = nullptr;
	buf_info.flags = 0;
	buf_info.size = buffer_size;
	buf_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
	buf_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	buf_info.queueFamilyIndexCount = 0;
	buf_info.pQueueFamilyIndices = nullptr;

	VkResult res;

	res = vkCreateBuffer(mLogDevice, &buf_info, nullptr, &mBufferIndex);

	if (res != VK_SUCCESS) {
		cerr << "failed to create the index buffer" << endl;
		exit(EXIT_FAILURE);
	}

	cerr << "successfully created the index buffer" << endl;

	//VkMemoryRequirements mem_req;
	vkGetBufferMemoryRequirements(mLogDevice, mBufferVertex, &o_mem_req);
	cerr << "index buffer memory requirements:" << endl;
	tools::Print(o_mem_req, "\t");

	return res;
}

uint32_t VulkanRenderer::ChooseMemoryType(const VkMemoryRequirements& mem_req,
	const VkMemoryPropertyFlags& req_flags) {
	VkPhysicalDeviceMemoryProperties phys_dev_mem_prop;
	vkGetPhysicalDeviceMemoryProperties(mPhysDevices[mChosenPhysDevice],
		&phys_dev_mem_prop);
	for (uint32_t i = 0; i < phys_dev_mem_prop.memoryTypeCount; i++) {
		const VkMemoryType& mem_type = phys_dev_mem_prop.memoryTypes[i];

		const bool ok1 = ((mem_type.propertyFlags & req_flags) == req_flags);
		const bool ok2 = (mem_req.memoryTypeBits >> i) & 0x1;

		if (ok1 && ok2) return i;
	}
	return -1;
}

VkResult VulkanRenderer::CreateDeviceMemoryVertex(
	const mesh_size_info_t& size_info, const VkMemoryRequirements& mem_req) {
	VkDeviceSize buffer_size = size_info._n_v * sizeof(geo_str::vertex_t);

	constexpr VkMemoryPropertyFlags req_buffer_memory_props =
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

	uint32_t mem_type_idx = this->ChooseMemoryType(mem_req, req_buffer_memory_props);
	if (mem_type_idx == -1) {
		cerr << "requested memory type for the vertex buffer not found" << endl;
		exit(EXIT_FAILURE);
	}

	VkMemoryAllocateInfo mem_alloc_info = {};
	mem_alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	mem_alloc_info.pNext = nullptr;
	mem_alloc_info.allocationSize = buffer_size;
	mem_alloc_info.memoryTypeIndex = mem_type_idx;

	VkResult res;

	res = vkAllocateMemory(mLogDevice, &mem_alloc_info, nullptr, &mDeviceMemoryVertex);

	return res;
}

VkResult VulkanRenderer::CreateDeviceMemoryIndex(
	const mesh_size_info_t& size_info, const VkMemoryRequirements& mem_req) {
	VkDeviceSize buffer_size = size_info._n_tr * sizeof(geo_str::triangle_t);

	constexpr VkMemoryPropertyFlags req_buffer_memory_props =
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;

	uint32_t mem_type_idx = this->ChooseMemoryType(mem_req, req_buffer_memory_props);
	if (mem_type_idx == -1) {
		cerr << "requested memory type for the index buffer not found" << endl;
		exit(EXIT_FAILURE);
	}

	VkMemoryAllocateInfo mem_alloc_info = {};
	mem_alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	mem_alloc_info.pNext = nullptr;
	mem_alloc_info.allocationSize = buffer_size;
	mem_alloc_info.memoryTypeIndex = mem_type_idx;

	VkResult res;

	res = vkAllocateMemory(mLogDevice, &mem_alloc_info, nullptr, &mDeviceMemoryIndex);

	return res;
}

VkResult VulkanRenderer::BindAndMapMemoryToBufferVertex(
	const mesh_size_info_t& size_info) {
	VkDeviceSize buffer_size = size_info._n_v * sizeof(geo_str::vertex_t);
	VkDeviceSize offset = 0;

	VkResult res;

	res = vkBindBufferMemory(mLogDevice, mBufferVertex, mDeviceMemoryVertex, offset);

	if (res != VK_SUCCESS) {
		cerr << "failed to bind memory of the vertex buffer" << endl;
		exit(EXIT_FAILURE);
	}

	res = vkMapMemory(mLogDevice, mDeviceMemoryVertex, offset, buffer_size, 0, &mHostMemoryVertex);

	if (res != VK_SUCCESS) {
		cerr << "failed to map memory of the vertex buffer" << endl;
		exit(EXIT_FAILURE);
	}

	return res;
}

VkResult VulkanRenderer::BindAndMapMemoryToBufferIndex(
	const mesh_size_info_t& size_info) {
	VkDeviceSize buffer_size = size_info._n_tr * sizeof(geo_str::triangle_t);
	VkDeviceSize offset = 0;

	VkResult res;

	res = vkBindBufferMemory(mLogDevice, mBufferIndex, mDeviceMemoryIndex, offset);

	if (res != VK_SUCCESS) {
		cerr << "failed to bind memory of the index buffer" << endl;
		exit(EXIT_FAILURE);
	}

	res = vkMapMemory(mLogDevice, mDeviceMemoryIndex, offset, buffer_size, 0, &mHostMemoryIndex);

	if (res != VK_SUCCESS) {
		cerr << "failed to map memory of the index buffer" << endl;
		exit(EXIT_FAILURE);
	}

	return res;
}

// ============================================================================

} // end of namespace vulovch
