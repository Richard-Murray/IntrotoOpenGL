#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>
#include <stdio.h>

//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>
#include "Application1.h"
#include "Application2.h"
#include "Application3.h"
#include "Application4.h"
#include "Application5.h"
#include "Application6.h"
#include "Application7.h"

int main() {

	Application7 *application = new Application7();

	application->Run();
}
