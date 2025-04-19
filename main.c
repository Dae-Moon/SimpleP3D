#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define GLT_IMPLEMENTATION
#include "gltext.h"

// camera options
#define FISH_EYE 0
#define FOV (M_PI / 2.f)
#define HALF_FOV (FOV / 2.f)
#define NEAR 0.0f
#define FAR 32.f

// movement options
#define COLLISION 1
#define MOVESPEED 5.f
#define ROTATESPEED 5.f

typedef struct vec2 { float x, y; } vec2;

static void DrawEllipse(float cx, float cy, float rx, float ry, int num_segments, float r, float g, float b, float a)
{
    float theta = 2 * 3.1415926f / (float)(num_segments);
    float c = cosf(theta); // precalculate the sine and cosine
    float s = sinf(theta);
    float t;
    float x = 1; // we start at angle = 0
    float y = 0;

    glColor4f(r, g, b, a);
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++)
    {
        // apply radius and offset
        glVertex2f(x * rx + cx, y * ry + cy); // output vertex

        // apply the rotation matrix
        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    }
    glEnd();
}

static void FillEllipse(float cx, float cy, float rx, float ry, int num_segments, float r, float g, float b, float a)
{
    float theta = 2 * 3.1415926f / (float)(num_segments);
    float c = cosf(theta);
    float s = sinf(theta);
    float t;
    float x = 1;
    float y = 0;

    glColor4f(r, g, b, a);
    glBegin(GL_TRIANGLE_FAN);
    for (int ii = 0; ii < num_segments; ii++)
    {
        glVertex2f(x * rx + cx, y * ry + cy);

        t = x;
        x = c * x - s * y;
        y = s * t + c * y;
    }
    glEnd();
}

static void DrawRectangle(float x, float y, float w, float h, float r, float g, float b, float a)
{
    glColor4f(r, g, b, a);
    glBegin(GL_LINES);
        glVertex2f(x, y);
        glVertex2f(x + w, y);

        glVertex2f(x + w, y);
        glVertex2f(x + w, y + w);

        glVertex2f(x + w, y + w);
        glVertex2f(x, y + w);

        glVertex2f(x, y + w);
        glVertex2f(x, y);
    glEnd();
}

static void FillRectangle(float x, float y, float w, float h, float r, float g, float b, float a)
{
    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + w, y);
        glVertex2f(x + w, y + w);
        glVertex2f(x, y + w);
        glVertex2f(x, y);
    glEnd();
}

static void SetWindowSizeCallback(GLFWwindow* window, int width, int height)
{
    // Adjusting viewport to screen coordinates.
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, height, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main()
{
    const int map_width = 20;
    const int map_height = 20;
    const char map[] =
        "################## #"
        "#                  #"
        "                   #"
        "      ###        ###"
        "#     #          ###"
        "#     #            #"
        "#            #     #"
        "#            #     #"
        "#            #      "
        "#                   "
        "# ###               "
        "#                  #"
        "#            #     #"
        "#           #      #"
        "#         ##       #"
        "#    #             #"
        "#    #             #"
        "#  ######          #"
        "#                  #"
        "##### ##############";

#define TILE(x, y) ((x >= 0 && x < map_width && y >= 0 && y < map_height) ? map[x + y * map_width] : ' ')

    int width = 800;
    int height = 600;

    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW.\n");
        return EXIT_FAILURE;
    }

    GLFWwindow* window = glfwCreateWindow(width, height, "SimpleP3D", NULL, NULL);
    if (!window)
    {
        fprintf(stderr, "Failed to create GLFW window.\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwSetWindowSizeCallback(window, SetWindowSizeCallback);
    glfwMakeContextCurrent(window);
    SetWindowSizeCallback(window, width, height);
    glfwSwapInterval(0);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Инициализация GLEW
    GLenum glewInitializeStatus = glewInit();
    if (glewInitializeStatus != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW: %s.\n", glewGetErrorString(glewInitializeStatus));
        return -1;
    }

    if (!gltInit())
    {
        fprintf(stderr, "Failed to initialize glText.\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    double currentTime = 0.0;

    GLTtext* text = gltCreateText();
    gltSetText(text, "Hello World");
    char info[255];
    float frameTime = 0.f;
    float x = 1.5f;
    float y = 1.5f;
    float rotation = 0.f;
    float rotationY = 0.f;
    vec2* rays = calloc(width, sizeof(vec2));

    while (!glfwWindowShouldClose(window))
    {
        frameTime = fabsf(glfwGetTime() - currentTime);
        frameTime = frameTime < 0.25f ? frameTime : 0.25f;
        currentTime = glfwGetTime();

        glfwGetFramebufferSize(window, &width, &height);
        
        float cos = cosf(rotation);
        float sin = sinf(rotation);

        vec2 velocity = { 0.f, 0.f };

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            velocity.x += cos * MOVESPEED * frameTime;
            velocity.y += sin * MOVESPEED * frameTime;
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            velocity.x += sin * MOVESPEED * frameTime;
            velocity.y -= cos * MOVESPEED * frameTime;
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            velocity.x -= cos * MOVESPEED * frameTime;
            velocity.y -= sin * MOVESPEED * frameTime;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            velocity.x -= sin * MOVESPEED * frameTime;
            velocity.y += cos * MOVESPEED * frameTime;
        }

        if (!COLLISION || TILE((int)floorf(x + velocity.x), (int)floorf(y + velocity.y)) != '#')
        {
            x += velocity.x;
            y += velocity.y;
        }

        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        {
            rotation -= ROTATESPEED * frameTime;
        }
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        {
            rotation += ROTATESPEED * frameTime;
        }
        
        // Normalize angle to be within the range [0, 2*PI)
        if (rotation < 0.f)
            rotation += 2.f * M_PI;
        else if (rotation >= 2.f * M_PI)
            rotation -= 2.f * M_PI;

        glClear(GL_COLOR_BUFFER_BIT);

        if (rays)
            rays = (vec2*)realloc(rays, sizeof(vec2) * width);

        for (float sx = 0; sx < width; sx++)
        {
            float angle = FISH_EYE ?
                rotation - HALF_FOV + sx * FOV / width :
                atanf((sx - width / 2.f) / (width / 2.f) * tanf(HALF_FOV)) + rotation;

            cos = cosf(angle);
            sin = sinf(angle);

            if (!cos)
                cos = 0.000001f;
            if (!sin)
                sin = 0.000001f;

            const int step_x = cos >= 0 ? 1 : -1;
            const int step_y = sin >= 0 ? 1 : -1;
            const int offset_x = cos >= 0 ? 1 : 0;
            const int offset_y = sin >= 0 ? 1 : 0;

            int xm = floorf(x) + offset_x;
            int ym = floorf(y) + offset_y;
            float depth_v = 0.f;
            float depth_h = 0.f;

            while (depth_v <= FAR)
            {
                depth_v = ((float)xm - x) / cos;
                int _y = floorf(y + depth_v * sin);
                int _x = xm + (offset_x - 1);

                if (TILE(_x, _y) == '#')
                    break;

                xm += step_x;
            }

            while (depth_h <= FAR)
            {
                depth_h = ((float)ym - y) / sin;
                int _x = floorf(x + depth_h * cos);
                int _y = ym + (offset_y - 1);

                if (TILE(_x, _y) == '#')
                    break;

                ym += step_y;
            }
            
            float distance = fabsf(depth_v < depth_h ? depth_v : depth_h);
            if (distance >= NEAR && distance <= FAR)
            {
                float ceiling = height / 2 - height / (FISH_EYE ? distance : distance * cosf(angle - rotation));
                float floor = height - ceiling;
                float brightness = 1.f / distance;

                glBegin(GL_LINES);

                // draw wall.
                glColor3f(brightness, brightness, brightness);
                glVertex2f(sx, ceiling);
                glVertex2f(sx, floor);

                // wall reflection on the floor.
                glColor3f(brightness / 2.f, brightness / 2.f, brightness / 2.f);
                glVertex2f(sx, floor);
                glColor3f(0.f, 0.f, 0.f);
                glVertex2f(sx, floor + height / distance);

                glEnd();
            }

            if (rays)
            {
                rays[(int)sx].x = x + cos * distance;
                rays[(int)sx].y = y + sin * distance;
            }
        }

        float tile_width = 0.1f * width / map_width;
        float tile_height = 0.1f * width / map_height;
        FillRectangle(9.f, 9.f, 0.1f * width, 0.1f * height, 0.f, 0.f, 0.f, 1.f); // draw background map.
        for (int y = 0; y < map_height; y++)
        {
            for (int x = 0; x < map_width; x++)
            {
                switch (map[x + y * map_width])
                {
                case '#': FillRectangle(10.f + x * tile_width, 10.f + y * tile_height, tile_width, tile_height, 1.f, 1.f, 1.f, 1.f); break;
                case ' ': DrawRectangle(10.f + x * tile_width, 10.f + y * tile_height, tile_width, tile_height, 0.5f, 0.5f, 0.5f, 1.f); break;
                }

            }

        }
        FillEllipse(10.f + x * tile_width, 10.f + y * tile_height, tile_width / 2.f, tile_height / 2.f, 12, 1.f, 1.f, 1.f, 1.f); // draw player.
        for (int i = 0; i < width && rays; i++)
        {
            glBegin(GL_LINES);
            glColor3f(1.f, 0.f, 0.f);
            glVertex2f(10.f + x * tile_width, 10.f + y * tile_height);
            glVertex2f(10.f + rays[i].x * tile_width, 10.f + rays[i].y * tile_height);
            glEnd();
        }

        // Save the current state of OpenGL.
        GLint currentProgram; glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
        GLint activeTexture; glGetIntegerv(GL_ACTIVE_TEXTURE, &activeTexture);
        GLint boundTexture; glActiveTexture(GL_TEXTURE0); glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);

        sprintf(
            info,
            "FPS: %.1f\n"
            "X: %.1f, Y: %.1f\n"
            "Rotation: %.1f",
            1.f / frameTime,
            x, y,
            180.f / M_PI * rotation);
        gltSetText(text, info);

        gltBeginDraw();
        gltDrawText2D(text, 20.f + 0.1f * width, 10.f, 1.f);
        gltEndDraw();

        // Restore OpenGL state.
        glUseProgram(currentProgram);
        glActiveTexture(activeTexture);
        glBindTexture(GL_TEXTURE_2D, boundTexture);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    free(rays);

    gltDestroyText(text);
    gltTerminate();
    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}