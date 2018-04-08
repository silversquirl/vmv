#define PI 3.14159265358979323846

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "audio.h"
#include "graphics.h"
#include "timer.h"

GLFWwindow *window;

void drawBars(graphics_options *prop)
{
	float x, y;
	float curveRad = prop->curve_radius;
	float barWidth = 2.0f / bars.len;

	for (int b = 0; b < bars.len; b++) {
		glBegin(GL_POLYGON);

		float barx = -1.0f + barWidth * b;
		float barHeight = (float)bars.buf[b] / (float)BAR_MAX;

		glColor3f(prop->bar_color.red, prop->bar_color.green, prop->bar_color.blue);

		x = barx + prop->spacing / 2;
		y = -barHeight;

		for (int i = 0; i <= 5; i++) {
			float anglerad = PI * 90.0f / 5.0f * (float)i / 180.0f;
			float cx = sinf(anglerad) * curveRad;
			float cy = cosf(anglerad) * curveRad;
			glVertex2f(x + curveRad - cx, y + curveRad - cy);
		}

		x = barx + prop->spacing / 2;
		y = barHeight;

		for (int i = 5; i >= 0; i--) {
			float anglerad = PI * 90.0f / 5.0f * (float)i / 180.0f;
			float cx = sinf(anglerad) * curveRad;
			float cy = cosf(anglerad) * curveRad;
			glVertex2f(x + curveRad - cx, y - curveRad + cy);
		}

		x = barx + barWidth - prop->spacing / 2;
		y = barHeight;

		for (int i = 0; i <= 5; i++) {
			float anglerad = PI * 90.0f / 5.0f * (float)i / 180.0f;
			float cx = sinf(anglerad) * curveRad;
			float cy = cosf(anglerad) * curveRad;
			glVertex2f(x - curveRad + cx, y - curveRad + cy);
		}

		x = barx + barWidth - prop->spacing / 2;
		y = -barHeight;

		for (int i = 5; i >= 0; i--) {
			float anglerad = PI * 90.0f / 5.0f * (float)i / 180.0f;
			float cx = sinf(anglerad) * curveRad;
			float cy = cosf(anglerad) * curveRad;
			glVertex2f(x - curveRad + cx, y + curveRad - cy);
		}

		glEnd();
	}
}

void resizeCallback(GLFWwindow *win, int w, int h)
{
	glViewport(0, 0, w, h);
}

/* Gets information about a specific monitor. Use -1 for the default monitor. */
int get_monitor_details(int id, monitor_info *info)
{
	GLFWmonitor *monitor;
	if (id < 0) {
		monitor = glfwGetPrimaryMonitor();
	}
	else {
		int count;
		GLFWmonitor **monitors = glfwGetMonitors(&count);
		if (id > count) {
			fprintf(stderr, "Invalid monitor specified!\n");
			return -1;
		}
		monitor = monitors[id];
	}

	const GLFWvidmode *mode = glfwGetVideoMode(monitor);

	info->width = mode->width;
	info->height = mode->height;
	info->refresh_rate = mode->refreshRate;

	return 0;
}

int mainloop(graphics_options p)
{
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW!\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
#ifdef GLFW_TRANSPARENT_FRAMEBUFFER
	glfwWindowHint(GLFW_DEPTH_BITS, 16);
	glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
#endif

	window = glfwCreateWindow(512, 1024, "vmv", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resizeCallback);

	glfwSetWindowPos(window, 100, 100);

	const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwSetWindowPos(window, mode->width / 2 - 256, mode->height / 2 - 512);

	if (window == NULL) {
		fprintf(stderr, "Failed to create GLFW window!\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW!\n");
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	timer_lib_initialize();
	tick_t start = 0;

	do {
		process_audio();
		if (timer_elapsed(start) < 1.0f / p.fps_cap)
			continue;
		
		start = timer_current();

		glClear(GL_COLOR_BUFFER_BIT);

		drawBars(&p);

		glfwSwapBuffers(window);
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));

	glfwTerminate();
	timer_lib_shutdown();

	return 0;
}
