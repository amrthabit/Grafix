#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>


class Screenshot
{
public:
	static void TakeScreenshotWrapper(const char* prefix, int width, int height)
    {
        int pixelChannel = 3;
        int totalPixelSize = pixelChannel * width * height * sizeof(GLubyte);
        GLubyte* pixels = new GLubyte[totalPixelSize];
        glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		std::string file_name = prefix + std::to_string(glfwGetTime()) + ".ppm";
        std::ofstream fout(file_name);
        fout << "P3\n" << width << " " << height << "\n" << 255 << std::endl;
        for (size_t i = 0; i < height; i++)
        {
            for (size_t j = 0; j < width; j++)
            {
                size_t cur = pixelChannel * ((height - i - 1) * width + j);
                fout << (int)pixels[cur] << " " << (int)pixels[cur + 1] << " " << (int)pixels[cur + 2] << " ";
            }
            fout << std::endl;
        }
        delete[] pixels;
        fout.flush();
        fout.close();
	}

	static void TakeScreenshot()
	{
		int width, height;
		glfwGetFramebufferSize(glfwGetCurrentContext(), &width, &height);
        TakeScreenshotWrapper("GC3_A1_", width, height);
	}
};