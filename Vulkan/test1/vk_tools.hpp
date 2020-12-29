#pragma once

// STD
#include <string>
#include <vector>
#include <fstream>

#include <iostream>
using std::cerr;
using std::endl;

namespace vulovch {
namespace tools {

std::string ToString(const VkPhysicalDeviceType& phys_dev_type) {
	switch (phys_dev_type) {
	case (VK_PHYSICAL_DEVICE_TYPE_OTHER):          return std::string("VK_PHYSICAL_DEVICE_TYPE_OTHER");
	case (VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU): return std::string("VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU");
	case (VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU):   return std::string("VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU");
	case (VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU):    return std::string("VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU");
	case (VK_PHYSICAL_DEVICE_TYPE_CPU):            return std::string("VK_PHYSICAL_DEVICE_TYPE_CPU");
	case (VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM):       return std::string("VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM");
	default: return std::string("UNKNOWN");
	};
}

std::string QueueFlagsToString(const VkQueueFlags& queue_flags) {
	std::string res;
	if (queue_flags & VK_QUEUE_GRAPHICS_BIT) {
		if (!res.empty()) res.append(", "); res.append("VK_QUEUE_GRAPHICS_BIT"); }
	if (queue_flags & VK_QUEUE_COMPUTE_BIT) {
		if (!res.empty()) res.append(", "); res.append("VK_QUEUE_COMPUTE_BIT"); }
	if (queue_flags & VK_QUEUE_TRANSFER_BIT) {
		if (!res.empty()) res.append(", "); res.append("VK_QUEUE_TRANSFER_BIT"); }
	if (queue_flags & VK_QUEUE_SPARSE_BINDING_BIT) {
		if (!res.empty()) res.append(", "); res.append("VK_QUEUE_SPARSE_BINDING_BIT"); }
	if (queue_flags & VK_QUEUE_PROTECTED_BIT) {
		if (!res.empty()) res.append(", "); res.append("VK_QUEUE_PROTECTED_BIT"); }
	return res;
}

std::string MemoryPropertyFlagsToString(const VkMemoryPropertyFlags& mem_prop_flags) {
	std::string res;
	if (mem_prop_flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
		if (!res.empty()) res.append(", "); res.append("VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT"); }
	if (mem_prop_flags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) {
		if (!res.empty()) res.append(", "); res.append("VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT"); }
	if (mem_prop_flags & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) {
		if (!res.empty()) res.append(", "); res.append("VK_MEMORY_PROPERTY_HOST_COHERENT_BIT"); }
	if (mem_prop_flags & VK_MEMORY_PROPERTY_HOST_CACHED_BIT) {
		if (!res.empty()) res.append(", "); res.append("VK_MEMORY_PROPERTY_HOST_CACHED_BIT"); }
	if (mem_prop_flags & VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT) {
		if (!res.empty()) res.append(", "); res.append("VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT"); }
	if (mem_prop_flags & VK_MEMORY_PROPERTY_PROTECTED_BIT) {
		if (!res.empty()) res.append(", "); res.append("VK_MEMORY_PROPERTY_PROTECTED_BIT"); }
	if (mem_prop_flags & VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD) {
		if (!res.empty()) res.append(", "); res.append("VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD"); }
	if (mem_prop_flags & VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD) {
		if (!res.empty()) res.append(", "); res.append("VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD"); }
	return res;
}

std::string MemoryHeapFlagsToString(const VkMemoryHeapFlags& mem_heap_flags) {
	std::string res;
	if (mem_heap_flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
		if (!res.empty()) res.append(", "); res.append("VK_MEMORY_HEAP_DEVICE_LOCAL_BIT"); }
	if (mem_heap_flags & VK_MEMORY_HEAP_MULTI_INSTANCE_BIT) {
		if (!res.empty()) res.append(", "); res.append("VK_MEMORY_HEAP_MULTI_INSTANCE_BIT"); }
	if (mem_heap_flags & VK_MEMORY_HEAP_MULTI_INSTANCE_BIT_KHR) {
		if (!res.empty()) res.append(", "); res.append("VK_MEMORY_HEAP_MULTI_INSTANCE_BIT_KHR"); }
	return res;
}

std::string ToBinaryRep(const uint32_t& val) {
	std::string res;
	for (int i = 31; i >= 0; i--) {
		const char* const bit = ((val >> i) & 0x00000001) == 0 ? "0" : "1";
		res.append(bit);
	}
	return res;
}

std::string Extent2DToString(const VkExtent2D& ext) {
	std::string res;
	res.append("w: ");   res.append(std::to_string(ext.width));
	res.append(", h: "); res.append(std::to_string(ext.height));
	return res;
}

void Print(const VkExtensionProperties& obj, const char* suffix) {
	cerr << suffix << "name: " << obj.extensionName << endl;
}

void Print(const VkLayerProperties& obj, const char* suffix) {
	cerr << suffix << "name: " << obj.layerName << endl;
}

void Print(const VkPhysicalDevice& obj, const char* suffix) {
	VkPhysicalDeviceFeatures phys_dev_features;
	vkGetPhysicalDeviceFeatures(obj, &phys_dev_features);
	VkPhysicalDeviceProperties phys_dev_props;
	vkGetPhysicalDeviceProperties(obj, &phys_dev_props);

	cerr << endl;
	cerr << suffix << "API version: " << phys_dev_props.apiVersion << endl;
	cerr << suffix << "device ID: " << phys_dev_props.deviceID << endl;
	cerr << suffix << "device name: " << phys_dev_props.deviceName << endl;
	cerr << suffix << "device type: " << tools::ToString(phys_dev_props.deviceType) << endl;
	//TODO implement the rest
}

void Print(const VkQueueFamilyProperties& obj, const char* suffix) {
	cerr << suffix << QueueFlagsToString(obj.queueFlags) << endl;
}

void Print(const std::string& obj, const char* suffix) {
	cerr << suffix << obj << endl;
}

template<typename PROPERTYTYPE>
void Print(std::vector<PROPERTYTYPE>& collection, const char* property_name) {
	uint32_t i_entry = 0;
	for (const auto& entry : collection) {
		cerr << "\t" << property_name << "[" << i_entry << "]:";
		tools::Print(entry, "\t\t");
		i_entry++;
	}
}

void Print(const VkMemoryType& obj, const char* suffix) {
	cerr << suffix << "property flag: " << MemoryPropertyFlagsToString(obj.propertyFlags) << endl;
	cerr << suffix << "heap index: " << obj.heapIndex << endl;
}

void Print(const VkMemoryHeap& obj, const char* suffix) {
	cerr << suffix << "size: " << obj.size << endl;
	cerr << suffix << "flags: " << MemoryHeapFlagsToString(obj.flags) << endl;
}

void Print(const VkPhysicalDeviceMemoryProperties& obj, const char* suffix) {
	std::string suffix2(suffix); suffix2.append("\t");
	cerr << suffix << "memory type count: " << obj.memoryTypeCount << endl;
	for (uint32_t i = 0; i < obj.memoryTypeCount; i++) {
		Print(obj.memoryTypes[i], suffix2.c_str());
	}
	cerr << suffix << "memory heap count: " << obj.memoryHeapCount << endl;
	for (uint32_t i = 0; i < obj.memoryHeapCount; i++) {
		Print(obj.memoryHeaps[i], suffix2.c_str());
	}
}

void Print(const VkMemoryRequirements& obj, const char* suffix) {
	cerr << suffix << "size: " << obj.size << endl;
	cerr << suffix << "alignment: " << obj.alignment << endl;
	cerr << suffix << "memory type bits: " << ToBinaryRep(obj.memoryTypeBits) << endl;
}

void Print(const VkSurfaceCapabilitiesKHR& obj, const char* suffix) {
	cerr << suffix << "min image count: " << obj.minImageCount << endl;
	cerr << suffix << "max image count: " << obj.maxImageCount << endl;
	cerr << suffix << "current extent: " << Extent2DToString(obj.currentExtent) << endl;
	cerr << suffix << "min image extent: " << Extent2DToString(obj.minImageExtent) << endl;
	cerr << suffix << "max image extent: " << Extent2DToString(obj.maxImageExtent) << endl;
	cerr << suffix << "max image array layers: " << obj.maxImageArrayLayers << endl;
	//TODO implement the rest
}

void PrintStrings(const uint32_t& n, const char* const* const strings,
	const char* suffix) {
	for (uint32_t i = 0; i < n; i++) {
		const char* const str = strings[i];
		cerr << suffix << str << endl;
	}
}

std::vector<char> ImportShaderBinary(const char* filename) {
	std::ifstream file(filename, std::ios::ate | std::ios::binary);
	if (!file.is_open()) {
		std::string msg;
		msg.append("failed to open file '");
		msg.append(filename);
		msg.append("'");
		throw std::runtime_error(msg);
	}
	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	file.close();
	return buffer;
}

} // end of namespace tools
} // end of namespace vulovch
