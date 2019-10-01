#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

static bool wireframe = true;

struct ShaderSource{
    string vertexSource;
    string fragmentSource;
};

unsigned int compileShader(unsigned int type,const string& src){
    const char* source = src.c_str();
    unsigned int id = glCreateShader(type);
    glShaderSource(id,1,&source,nullptr);
    glCompileShader(id);

    int result;
    char infoLog[512];
    glGetShaderiv(id,GL_COMPILE_STATUS,&result);
    if(!result){
        glGetShaderInfoLog(id, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog <<endl;
    }

    return id;
}

unsigned int createShader(const string& vertexSrc,const string& fragmentSrc){
    unsigned int vs= compileShader(GL_VERTEX_SHADER,vertexSrc);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER,fragmentSrc);

    unsigned int program = glCreateProgram();
    
    glAttachShader(program,vs);
    glAttachShader(program,fs);

    glLinkProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

ShaderSource parseShader(const string& location){
    enum class ShaderType{
    VERTEX = 0, FRAGMENT = 1, NONE =-1
    };

    ifstream file(location);
    stringstream ss[2];
    string line;
    ShaderType type = ShaderType::NONE;
    while(getline(file,line)){
        if(line.find("#shader") != string::npos){
            if(line.find("vertex") != string::npos){
                type = ShaderType::VERTEX;
            }
            else if(line.find("fragment")){
                type = ShaderType::FRAGMENT;
            }
        }
        else{
            ss[(int)type]<<line<<"\n";
        }
    }
    return {ss[0].str(),ss[1].str()};
}


void framebuffer_size_callback(GLFWwindow* window,int width, int height)
{
    glViewport(0,0,width,height);
}

void processInput(GLFWwindow* window)
{
    if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS){
        glfwSetWindowShouldClose(window,true);
    }
}

int main(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800,600,"self",NULL,NULL);

    if(window == NULL){
        cout<<"Glfw window error"<<endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glewInit();

    glViewport(0,0,800,600);

    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

    //make a vertex array object
    unsigned int vao;
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    //define verticies as an arrary
    float verticies[] = {
        0.5f, -0.5f, 0.0f,       1.0f, 0.0f, 0.0f, 
        -0.5f, -0.5f, 0.0f,      0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f,        0.0f, 0.0f, 1.0f
    };

    //make a vertex buffer
    unsigned int vertexBuffer;
    glGenBuffers(1,&vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(verticies),verticies,GL_STATIC_DRAW);

    //make a index buffer
    // unsigned int indecies[]{
    //     1,2,0
    // };

    // unsigned int ebo;
    // glGenBuffers(1,&ebo);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER,3*sizeof(unsigned int),indecies,GL_STATIC_DRAW);

    //make a shader
    ShaderSource source = parseShader("res/Basic.shader");
    string vertexSrc = source.vertexSource;
    string fragmentSrc = source.fragmentSource;

    unsigned int shader = createShader(vertexSrc,fragmentSrc);
    glUseProgram(shader);

    //make a vertex attribute pointer
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glUseProgram(0);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader);
        glBindVertexArray(vao);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);

        //glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,nullptr);
        glDrawArrays(GL_TRIANGLES,0,3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}