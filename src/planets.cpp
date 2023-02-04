#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include "shader.h"
#include "camera.h"
#include "model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow *window);
// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// time to increase angle accordingly
float threshTime = 0.0f;

// control space key
bool begin_movement = false;

int main()
{
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // glfw window creation
  // --------------------
  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Planets", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetKeyCallback(window, key_callback);

  // tell GLFW to capture our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
  // stbi_set_flip_vertically_on_load(true);

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  // Vertex shader
  Shader PlanetShader("/home/pank/Documents/7o_eks/graphics/erg/dev/src/planets.vs",
                      "/home/pank/Documents/7o_eks/graphics/erg/dev/src/planets.fs");

  Shader LightingShader("/home/pank/Documents/7o_eks/graphics/erg/dev/src/planets.vs",
                        "/home/pank/Documents/7o_eks/graphics/erg/dev/src/lighting.fs");

  PlanetShader.use();
  PlanetShader.setInt("texture0", 0);

  LightingShader.use();
  LightingShader.setInt("texture1", 1);

  Model sun("/home/pank/Documents/7o_eks/graphics/erg/dev/misc/planet/planet.obj");
  Model moon("/home/pank/Documents/7o_eks/graphics/erg/dev/misc/rock/rock.obj");
  Model earth("/home/pank/Documents/7o_eks/graphics/erg/dev/misc/earth/Model/Globe.obj");

  // Initial positions of planets based on camera
  glm::vec3 sun_init_pos = glm::vec3(0.0f, 0.0f, -60.0f);
  glm::vec3 moon_init_pos = glm::vec3(20.0f, 0.0f, 0.0f);
  glm::vec3 earth_init_pos = glm::vec3(30.0f, 0.0f, 0.0f);

  glm::vec3 sun_pos = sun_init_pos;
  glm::vec3 moon_pos = moon_init_pos;
  glm::vec3 earth_pos = earth_init_pos;

  float angle = 0.0f;

  // render loop
  // -----------
  while (!glfwWindowShouldClose(window))
  {
    // per-frame time logic
    // --------------------
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.01f, 0.01f, 0.01f, 1.0f); // black background
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();

    // First render light source (Sun)
    glm::mat4 model1 = glm::mat4(1.0f);
    model1 = glm::translate(model1, sun_pos);

    LightingShader.use();
    LightingShader.setMat4("projection", projection);
    LightingShader.setMat4("view", view);
    LightingShader.setMat4("model", model1);
    LightingShader.setVec4("color", glm::vec4(1.8f, 1.5f, 1.0f, 1.0f));
    sun.Draw(LightingShader);

    // Render Earth and moon
    PlanetShader.use();
    PlanetShader.setMat4("projection", projection);
    PlanetShader.setMat4("view", view);
    PlanetShader.setVec3("lightPos", sun_init_pos);
    PlanetShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
    PlanetShader.setVec3("viewPos", camera.Position);

    // Moon
    glm::mat4 model2 = glm::mat4(1.0f);

    // Earth
    glm::mat4 model3 = glm::mat4(1.0f);

    // Control movement
    if (begin_movement)
    {
      threshTime += deltaTime;
      if (threshTime >= 1.0 / 65)
      {
        angle += 0.001f;
        threshTime = 0;
      }
      earth_pos = glm::vec3(30.0f * glm::cos(5 * angle), 0.0f, -30.0f * glm::sin(5 * angle));
      moon_pos = glm::vec3(20.0f * glm::cos(angle), 0.0f, -20.0f * -glm::sin(angle));
    }

    // Earth positioning
    model3 = glm::translate(model1, earth_pos);

    // Apply rotation for earth
    model3 = glm::rotate(model3, 30 * angle, glm::vec3(0.0f, 1.0f, 0.0f));

    // Moon positioning
    model2 = glm::translate(model3, moon_pos);

    // Draw objects after all transformations
    PlanetShader.setMat4("model", model2);
    PlanetShader.setMat4("InvTransModel", glm::transpose(glm::inverse(model2)));
    moon.Draw(PlanetShader);

    PlanetShader.setMat4("model", model3);
    PlanetShader.setMat4("InvTransModel", glm::transpose(glm::inverse(model3)));
    earth.Draw(PlanetShader);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
}

void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);

  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

  lastX = xpos;
  lastY = ypos;

  camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// glfw: whenever a key is pressed, this callback is called to control the spcae key activation
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
  {
    begin_movement = !begin_movement;
  }
}