#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include <vector>


int g_windowSizeX = 640;
int g_windowSizeY = 480;

 
 

// /<GLfloat, 18>
std::array point = { 
    
    // first triangle
     0.5f+0.2f,  0.5f+0.2f, 0.0f,  // top right
     0.5f+0.2f, -0.5f+0.2f, 0.0f,  // bottom right
    -0.5f+0.2f,  0.5f+0.2f, 0.0f,  // top left 
    // second triangle
     0.5f, -0.5f-0.1f, 0.0f,  // bottom right
    -0.5f, -0.5f-0.1f, 0.0f,  // bottom left
    -0.5f,  0.5f-0.1f, 0.0f   // top left
    
};
 
//  /<GLfloat, 18>
std::array colors = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f
};


const char * vertex_shader = 
    "#version 460\n"
    "layout(location = 0) in vec3 vertex_position;"
    "layout(location = 1) in vec3 vertex_color;"
    "out vec3 color;"
    "void main() {"
    "   color = vertex_color;"
    "   gl_Position = vec4(vertex_position, 1.0);"
    "}";

const char * fragment_shader = 
    "#version 460\n"
    "in vec3 color;"
    "out vec4 frag_color;"
    "void main() {"
    "   frag_color = vec4(color, 1.0);"
    "}";


GLuint compileVertexShader(const char * shader_source){
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &shader_source, nullptr);
    glCompileShader(vs);

    GLint success;

    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024] = { '\0' };
        glGetShaderInfoLog(vs, 1024, nullptr, infoLog);
        std::cerr << "ERROR::SHADER: (vertex)Compile-time error:\n" << infoLog << std::endl;

        return -1;
    }
    

    return vs;
}

GLuint compileFragmentShader(const char * shader_source){
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &shader_source, nullptr);
    glCompileShader(fs);
    
    GLint success;

    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024] = { '\0' };
        glGetShaderInfoLog(fs, 1024, nullptr, infoLog);
        std::cerr << "ERROR::SHADER: (fragment)Compile-time error:\n" << infoLog << std::endl;

        return -1;
    }


    return fs;
}

GLuint createGlProgram(const char* vertex_source, const char * fragment_source){
    GLuint vs = compileVertexShader(vertex_source);
    
    GLuint fs = compileFragmentShader(fragment_source);


    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vs);
    glAttachShader(shader_program, fs);
    glLinkProgram(shader_program);


    GLint success;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar infoLog[1024] = { '\0' };
        glGetProgramInfoLog(shader_program, 1024, nullptr, infoLog);
        std::cerr << "ERROR::SHADER: Link-time error:\n" << infoLog << std::endl;
    }  

    glDeleteShader(vs);
    glDeleteShader(fs);
    return shader_program;
}

// Vertex Buffer Object
//returm GLuint
//Container().begin(), (std::declval<Container>().operator[](0)), 

//input params : std::array<GLfloat, 18> &arrays
template <typename Container>
auto CreateVBO(Container &arrays) -> decltype(std::declval<Container>().begin(), 
                                               std::declval<Container>().data(), 
                                               std::declval<Container>().operator[](0), 
                                               GLuint()) {
   
    GLuint arrays_vbo = 0;
    glGenBuffers(1, &arrays_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, arrays_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(arrays), arrays.data(), GL_STATIC_DRAW);
    return arrays_vbo;
}

// Vertex Array Object

GLuint CreateVAO(GLuint points_vbo, GLuint colors_vbo){
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    std::cout<<"vao = "<<vao<<std::endl;
    return vao;
}



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

 

    GLuint shader_program = createGlProgram(vertex_shader, fragment_shader);
 

    GLuint points_vbo = CreateVBO(point);
    GLuint colors_vbo = CreateVBO(colors);

    GLuint vao = CreateVAO(points_vbo, colors_vbo);
 
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);


        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
