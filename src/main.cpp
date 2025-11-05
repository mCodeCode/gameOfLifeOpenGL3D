#include <glad/glad.h>
#include <GLFW/glfw3.h>



#include <filesystem>
#include <iostream>
#include <thread>

#include "customcamera.h"
#include "shaderhandler.h"
#include "voxelmanager.h" //includes glm::vec3 vec4 and such from imports



//glfw callbacks for mouse and keyboard input
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);





// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 600;



//delta time 
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame


//cam settings
Camera mainCamera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));

// process all input: query GLFW whether relevant keys are pressed/released this frame
// and react accordingly
//---------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //camera controls
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        mainCamera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
    }
         
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        mainCamera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
    }
         
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        mainCamera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
    }
         
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        mainCamera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
    }

    
         
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

//----------------------------------------------------------------------------------------------
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn){

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        mainCamera.LastX = xpos;
        mainCamera.LastY = ypos;
        firstMouse = false;
    }
    mainCamera.ProcessMouseMovement(xpos,ypos);
}


//---------------------------------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    mainCamera.ProcessMouseScroll(yoffset);
}




//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "game of life 3D", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //-----------------------------------------------
    //-----------------------------------------------
    glfwMakeContextCurrent(window);
     
    if (!glfwGetCurrentContext()) {
        std::cout  << "OpenGL context is not current!\n";
    }
    
    // Print main thread ID for comparison
    std::cout  << "Main thread ID: " << std::this_thread::get_id() << "\n";

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //enable mouse callback 
    glfwSetCursorPosCallback(window, mouse_callback); 
    //enable mouse scroll callback 
    glfwSetScrollCallback(window, scroll_callback); 
    //disable mouse cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
   

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    //---- more debug info
    int profile;
    glGetIntegerv(GL_CONTEXT_PROFILE_MASK, &profile);
    if (profile & GL_CONTEXT_CORE_PROFILE_BIT) {
        std::cout  << "Context profile: CORE\n";
    } else if (profile & GL_CONTEXT_COMPATIBILITY_PROFILE_BIT) {
        std::cout  << "Context profile: COMPATIBILITY\n";
    } else {
        std::cout  << "Context profile: UNKNOWN\n";
    }

    std::cout  << "OpenGL version: " << glGetString(GL_VERSION) << "\n";
    std::cout  << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
    std::cout  << "Renderer: " << glGetString(GL_RENDERER) << "\n";
    std::cout  << "Vendor: " << glGetString(GL_VENDOR) << "\n";


    //----------------------------------------

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------



    // build and compile our shader program
    // ------------------------------------
    Shader worldShader("../res/shaders/main_sh_1.vs", "../res/shaders/main_sh_1.fs"); 
    //voxel size uniform
    worldShader.use();
    worldShader.setFloat("voxelSize", 0.02);

    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //--------     setting up VAO VBO and EBO for instance rendering
    try {
        voxelmanager::setupSharedCubeGeometry();
        std::cout  << " setupSharedCubeGeometry() completed\n";
    } catch (const std::exception& e) {
        std::cout  << " CRASH in setupSharedCubeGeometry: " << e.what() << "\n";
    }


    //----------  create the initial positions and colors for all instances
    try {
        voxelmanager::initInstacesData();
        std::cout  << " initInstacesData() completed\n";
    } catch (const std::exception& e) {
        std::cout  << " CRASH in setupSharedCubeGeometry: " << e.what() << "\n";
    }



    

    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------


    // pass projection matrix to shader (note that in this case it could change every frame)
    worldShader.use();
    
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    worldShader.setMat4("projection", projection);
    
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------

    













    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------
    //------------------------------------------------------------------------

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
        //-----------------
        processInput(window);


        // update simulation
        //------------------


        // render
        //------------------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // activate shader
        worldShader.use();

        //set view matrix
        glm::mat4 view = mainCamera.GetViewMatrix();
        worldShader.setMat4("view", view);

        //---- update buffers
        voxelmanager::updateInstacesData();


        //bind VAO before draw call
        glBindVertexArray(voxelmanager::sharedVAO);

        GLint boundEBO = 0;
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundEBO);
        if (boundEBO == 0) {
            std::cout  << "CRITICAL: No EBO bound before draw — will crash!\n\n";
        }

        int instancesCount = voxelmanager::instancesPositions.size();
        std::cout << "instancesCount : " << instancesCount << "\n";
        if (instancesCount == 0) continue;


        glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, instancesCount);
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cout << "OpenGL Error after draw: " << err << "\n";
        }


        glBindVertexArray(0);



        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1,  &voxelmanager::sharedVAO);
    glDeleteBuffers(1,  &voxelmanager::sharedVBO);
    glDeleteBuffers(1,  &voxelmanager::sharedEBO);
    glDeleteProgram(worldShader.ID);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

