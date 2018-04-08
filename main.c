#define GLEW_STATIC

#define PI 3.14159265358979323846

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define BAR_MAX 16
struct { size_t len; unsigned int *buf; } bars;

typedef struct {
	float curve_radius;
	int fps_cap;
	struct {
		float red;
		float green;
		float blue;
	} bar_color;
	char close_key;
	float spacing;
	int monitor; // -1 for default
	struct {
		unsigned int x, y, width, height;
		int center; // When center != 0, x and y are ignored
	} pos;
} graphics_options;

typedef struct {
	unsigned int width, height, refresh_rate;
} monitor_info;

GLFWwindow *window;

void process_audio(void) {}

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
monitor_info get_monitor_details(int id)
{
	GLFWmonitor *monitor;
	if (id < 0) {
		monitor = glfwGetPrimaryMonitor();
	}
	else {
		int count;
		GLFWmonitor **monitors = glfwGetMonitors(&count);
		if (id > count) {
			fprintf(stderr, "Invalid monitor specified!");
			return;
		}
		monitor = monitors[id];
	}

	const GLFWvidmode *mode = glfwGetVideoMode(monitor);

	monitor_info info;

	info.width = mode->width;
	info.height = mode->height;
	info.refresh_rate = mode->refreshRate;

	return info;
}

int mainloop(graphics_options p)
{
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW!");
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

	window = glfwCreateWindow(512, 1024, "VkVis", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resizeCallback);

	glfwSetWindowPos(window, 100, 100);

	const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwSetWindowPos(window, mode->width / 2 - 256, mode->height / 2 - 512);

	if (window == NULL) {
		fprintf(stderr, "Failed to create GLFW window!");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW!");
		glfwTerminate();
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	float curveRad = 0.25f;

	tick_t start, time;
	tick_t tick, freq, res;

	timer_lib_initialize();

	res = 0xFFFFFFFFFFFFFFFFULL;
	freq = timer_ticks_per_second();
	start = timer_current();

	do {
		time = timer_current();
		do {
			tick = timer_elapsed_ticks(time);
		} while (!tick);

		if (tick < res)
			res = tick;

		if (timer_elapsed(start) < 1.0f / p.fps_cap)
			continue;

		start = time;

		glClear(GL_COLOR_BUFFER_BIT);

		drawBars(&p);

		glfwSwapBuffers(window);
		glfwPollEvents();

		printf("Frame!\r\n");

	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));

	glfwTerminate();

	return 0;
}

int main(int argc, char *argv[])
{
	bars.len = 10;
	unsigned int bs[] = {16, 8, 4, 2, 1, 1, 2, 4, 8, 16};
	bars.buf = bs;

	graphics_options p;

	p.curve_radius = 0.05f;
	p.bar_color.red = 1.0f;
	p.bar_color.green = 0.0f;
	p.bar_color.blue = 0.0f;
	p.spacing = 0.1f;
	p.fps_cap = 60;

	return mainloop(p);
}
