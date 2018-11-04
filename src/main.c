#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>


const int WIDTH = 800;
const int HEIGHT = 600;

float RECTANGLE_VERTICES[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};

unsigned int RECTANGLE_INDICES[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
}; 

// called when user resizes window
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// called when we receive input
void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

unsigned int new_VAO() {
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    return VAO;
}

unsigned int new_VBO() {
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    return VBO;
}

unsigned int new_EBO() {
    unsigned int EBO;
    glGenBuffers(1, &EBO);

    return EBO;
}

void send_vertices_to_buffer(float vertices[], float size, int mode) {
    // lets make some memory for our vertices and copy that data to the
    // buffer's memory.
    glBufferData(mode, size * sizeof(float), vertices, GL_STATIC_DRAW);
    // GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
    // GL_DYNAMIC_DRAW: the data is likely to change a lot.
    // GL_STREAM_DRAW: the data will change every time it is drawn.
}

unsigned int compile_shaders() {
    const char* vertexShaderSource =
        "#version 330 core\n" // using 330 because we are using OpenGL 3.3
        "layout (location = 0) in vec3 aPos;\n"
        "void main()"
        "{\n"
        "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}";

    // we need a shader object referenced by an ID for the vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // since we are currently creating a vertex shader, we will pass in
    // GL_VERTEX_SHADER
    
    // attaching the source of the shader to the shader object 
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

    // compiling the shader
    glCompileShader(vertexShader);

    // we should probably check if the shader compilation was successful
    int  success;
    char infoLog[512]; // storage container for error messages
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Could not compile vertex shader");
        printf(infoLog);
    }

    const char* fragmentShaderSource = 
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n";

    // we need to compile the fragment shader as well,
    // but with GL_FRAGMENT_SHADER
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // lets check for compile errors for the frament shader as well.
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Could not compile fragment shader");
        printf(infoLog);
    }

    // To use the recently compiled shaders we have to link
    // them to a shader program object and then activate this shader program
    // when rendering objects.
    //
    // When linking the shaders into a program it links the outputs of each
    // shader to the inputs of the next shader. This is also where you'll get
    // linking errors if your outputs and inputs do not match.
    
    // lets create a program object
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram(); // returns ID reference to created obj

    // Now we need to attach the previously compiled shaders to the program
    // object and then link them.
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // lets check for errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Failed to link shader program");
        printf(infoLog);
    }

    // Our shaders are now trash, we dont need them anymore.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

void render_VAO(unsigned int VAO, unsigned int shaderProgram, int vertices, int mode) {
    // lets activate the program object, telling the graphics card to use it.
    glUseProgram(shaderProgram);

    // Bind VAO again
    glBindVertexArray(VAO);

    // render using wireframe mode
    // use GL_FILL to reset to normal
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    if (mode == 0)
        glDrawArrays(GL_TRIANGLES, 0, vertices);
    else
        glDrawElements(GL_TRIANGLES, vertices, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

int main(int argc, char* argv[]) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // only on MACOS
    #endif
    
    // creating the window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL App", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // hook on window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD");
        return -1;
    }
    
    printf("OpenGL %d.%d\n", GLVersion.major, GLVersion.minor);

    glViewport(0, 0, WIDTH, HEIGHT);

    unsigned int VAO = new_VAO();
    unsigned int VBO = new_VBO(); 
    unsigned int EBO = new_EBO();

    glBindVertexArray(VAO);

    // the buffer type of a vertex buffer object is GL_ARRAY_BUFFER
    // so lets use that one for this VBO.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // rectangle
    send_vertices_to_buffer(RECTANGLE_VERTICES, sizeof(RECTANGLE_VERTICES), GL_ARRAY_BUFFER);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    send_vertices_to_buffer(RECTANGLE_INDICES, sizeof(RECTANGLE_INDICES), GL_ELEMENT_ARRAY_BUFFER);

    // we have to tell OpenGL how to interpret the vertex data and how
    // to connect it to the shaders
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // first argument is `0` because location=0 in our vertexShader
    // second argument is `3` because the vertex attribute is vec3, 3 values.
    glEnableVertexAttribArray(0); // `0` as argument because location=0

    unsigned int shaderProgram = compile_shaders();

    // render loop
    while(!glfwWindowShouldClose(window)) {
        processInput(window);

        // render here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // rectangle
        render_VAO(VAO, shaderProgram, sizeof(RECTANGLE_INDICES) / sizeof(unsigned int), 1);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();
    return 0;
}
