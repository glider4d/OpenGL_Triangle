#include <glad/glad.h>
// #include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <array>
#include <string>
#include <memory>

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <stb_image.h>


int g_windowSizeX = 640;
int g_windowSizeY = 480;

 
 

// /<GLfloat, 18>
std::array point = { 
    0.0f,   50.0f, 0.0f, //top
    50.0f,  -50.0f, 0.0f, //right
    -50.0f,  -50.0f, 0.0f,  //left

    //  0.0f,   0.5f, 0.0f, //top
    // 0.5f,  -0.5f, 0.0f, //right
    // -0.5f,  -0.5f, 0.0f,  //left



    // -1.0f, -0.5f, 0.0f, // left
    // -0.5f, 1.0f, 0.0f,   // top
    // 0.f, -0.5f, 0.0f, // right
    // -0.0f, -0.5f, 0.0f, // left
    // 0.5f, 1.0f, 0.0f,   // top
    // 1.f, -0.5f, 0.0f, // right
    
     
    
    // first triangle
    //  -0.5f,  0.0f, 0.0f,  // top right
    //  0.0f, 0.5f, 0.0f,  // bottom right
    //  0.5f,  0.0f, 0.0f,  // top left 
    // second triangle
    //  0.5f, -0.5f, 0.0f,  // bottom right
    // -0.5f, -0.5f, 0.0f,  // bottom left
    // -0.5f,  0.5f, 0.0f   // top left
    
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


std::array texCoord = {
        0.5f, 1.5f, //top
        1.0f, 0.0f, //right
        0.0f, 0.0f, //left

        
0.0f, 0.0f, //left
    0.5f, 1.0f, //top
    1.0f, 0.0f, //right



        
    

    
    // 0.0f, 0.0f,  // lower-left corner  
    // 0.5f, 0.0f,  // lower-right corner
    // 0.5f, 1.0f   // top-center corner
    // 0.5f, 1.0f,
    // 1.0f, 0.0f,
    // 0.0f, 0.0f
    //  0.5f,  0.5f, 0.0f,  // top right
    //  0.5f, -0.5f, 0.0f,  // bottom right
    // -0.5f,  0.5f, 0.0f,  // top left 
};

const char * vertex_shader = 
    "#version 460\n"
    "layout(location = 0) in vec3 vertex_position;"
    "layout(location = 1) in vec3 vertex_color;"
    "layout(location = 2) in vec2 texture_coord;"
    "out vec3 color;"
    "out vec2 texCoord;"
    "void main() {"
    "   color = vertex_color;"
    "   texCoord = texture_coord;"
    "   gl_Position = vec4(vertex_position, 1.0);"
    "}";

const char * fragment_shader = 
    "#version 460\n"
    "in vec3 color;"
    "in vec2 texCoord;"
    "out vec4 frag_color;"
    "uniform sampler2D tex;"
    "void main() {"
    "   frag_color = vec4(color, 1.0);"
    "}";


const char* fragment_shader_with_texture =
"#version 460\n"
"in vec3 color;"
"in vec2 texCoord;"
"out vec4 frag_color;"
"uniform sampler2D tex;"
"void main() {"
//"   frag_color = vec4(color, 1.0);"
    "frag_color = texture(tex, texCoord)  ;"
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


std::unique_ptr<std::string> g_path = nullptr;

GLint loadTexture(const std::string& texturePath){
    int channel = 0;
    int width = 0;
    int height = 0;

    GLuint textureID;

    stbi_set_flip_vertically_on_load(true);


    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//GL_LINEAR);
    unsigned char* pixels = stbi_load(std::string(*g_path + "\\" + texturePath).c_str(), &width, &height, &channel, 0);

    if (pixels){
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,GL_UNSIGNED_BYTE, pixels);
    } else {
        std::cerr << "Failed to load texture" << std::endl;
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(pixels);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureID;
}


GLuint loadTexture2(const std::string& texturePath)
{
    int channels = 0;
	int width = 0;
	int height = 0;

    const GLenum filter = GL_NEAREST;
    const GLenum wrapMode = GL_CLAMP_TO_EDGE;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* pixels = stbi_load(std::string(*g_path + "\\" + texturePath).c_str(), &width, &height, &channels, 0);
	
	if (!pixels) {
		std::cerr << "Can't load image: " << texturePath.c_str() << std::endl;
		return -1;
	}
    
    stbi_image_free(pixels);
    GLuint textureID;
    glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
		

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}

void setUniformInt(const std::string name, const GLuint shaderProgram, const GLint texId){
    glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), texId);
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

GLuint CreateVAOwithTexture(GLuint points_vbo, GLuint colors_vbo, GLuint texture_vbo){
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

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
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}





int main(int argc, char* argv[])
{
    std::string buf(argv[0]);
    g_path = std::make_unique<std::string>(buf.substr(0, buf.find_last_of("\\/")));
    buf.clear();


    


//    g_path = argv[0];
    std::cout<<g_path<<std::endl;
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

    GLuint textureID = 0;
   textureID = loadTexture2("res\\textures\\__map_16x16.png");
    GLuint shader_program = createGlProgram(vertex_shader, fragment_shader_with_texture);
 

    GLuint points_vbo = CreateVBO(point);
    GLuint colors_vbo = CreateVBO(colors);
    GLuint texture_vbo = CreateVBO(texCoord);

    // GLuint vao = CreateVAO(points_vbo, colors_vbo);
    GLuint vao = CreateVAOwithTexture(points_vbo, colors_vbo, texture_vbo);
 
    glUseProgram(shader_program);
    setUniformInt("tex", shader_program, 0 /*it's slot*/);
    glm::mat4 modelMatrix = glm::mat4(1.f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(100.f, 200.f, 0));
    glm::mat4 projectionMatrix = glm::ortho(0.0f, static_cast<float>(g_windowSizeX), 0.0f, static_cast<float>( g_windowSizeY), -100.f, 100.f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(pWindow))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(vao);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glDrawArrays(GL_TRIANGLES, 0, 3);


        glfwSwapBuffers(pWindow);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
