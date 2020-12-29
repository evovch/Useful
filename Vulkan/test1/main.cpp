// STD
#include <cstdio>

// PROJECT
#include "vulkan_renderer.h"
#include "mesh.hpp"

// GLFW
#include "GLFW/glfw3.h"
////#include "GLFW/glfw3native.h"

void error_callback_glfw(int error, const char* description) {
	printf("GLFW: error %d: %s\n", error, description);
}

void key_callback_glfw(GLFWwindow* window, int key,
	int scancode, int action, int mods) {
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
}

int main(int /*argc*/, char** /*argv*/)
{
	glfwSetErrorCallback(error_callback_glfw);
	if (!glfwInit()) {
		fprintf(stderr, "GLFW: error: failed to initialize.\n\nAborting.\n");
		exit(EXIT_FAILURE);
	}
	if (!glfwVulkanSupported()) {
		fprintf(stderr, "GLFW: error: Vulkan not supported.\n\nAborting.\n");
		exit(EXIT_FAILURE);
	}

	GLFWwindow* window = glfwCreateWindow(800, 600,
		"OvchVulkanRendererApp", NULL, NULL);
	if (!window) {
		fprintf(stderr, "GLFW: error: failed to create a window.\n\nAborting.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwSetWindowPos(window, 100, 100);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback_glfw);

	// Requested extensions
	uint32_t n_requested_instance_extensions = 0;
	const char** requested_instance_extensions =
		glfwGetRequiredInstanceExtensions(&n_requested_instance_extensions);
	uint32_t n_requested_device_extensions = 1;
	const char* requested_device_extensions[] = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	// Copy requests from GLFW
	std::vector<const char*> requested_instance_extensions_;
	for (uint32_t i = 0; i < n_requested_instance_extensions; i++) {
		requested_instance_extensions_.push_back(requested_instance_extensions[i]);
	}
	// Add user requests
	//requested_instance_extensions_.push_back("VK_KHR_get_surface_capabilities2");


	vulovch::VulkanRenderer* renderer1 = new vulovch::VulkanRenderer();

	vulovch::Mesh mesh1;
	mesh1.GenerateBox(0.1f, 0.1f, 0.1f, 0.0f, 0.0f, 0.0f);

	renderer1->SetMesh(&mesh1);

	renderer1->Init(
		static_cast<uint32_t>(requested_instance_extensions_.size()),
		requested_instance_extensions_.data(),
		n_requested_device_extensions, requested_device_extensions);

	while (!glfwWindowShouldClose(window)) {
		glfwWaitEvents();
	}

	glfwTerminate();

	return 0;
}
