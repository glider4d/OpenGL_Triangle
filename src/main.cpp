#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>


int g_windowSizeX = 640;
int g_windowSizeY = 480;


//for change window size
void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height){
    std::cout<<"new window size width = "<<width<<", height = "<<height<<std::endl;
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY);
}

//for key press
void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mode){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(void)
{
    GLFWwindow* pWindow;

    /* Initialize the library */
    if (!glfwInit()){
        std::cerr<<"glfwInit failed"<<std::endl;
        return -1;
    }

    /*set opengl context*/
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "OPENGL_TRIANGLE", nullptr, nullptr);
    if (!pWindow)
    {
        std::cerr<<"glfwCreateWindow failed"<<std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);
    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

    if (!gladLoadGL()){
        std::cerr<<"Can't load GLAD!"<<std::endl;
        return -1;
    }
    std::cout<<"Renderer: "<<glGetString(GL_RENDERER)<<std::endl;
    std::cout<<"OpenGL version: "<<glGetString(GL_VERSION)<<std::endl;
//    std::cout<<"OpenGL "<<GLVersion.major<<"."<<GLVersion.minor<<std::endl;



    glClearColor(1,1,0,1);
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Swap front and back buffers */
        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
