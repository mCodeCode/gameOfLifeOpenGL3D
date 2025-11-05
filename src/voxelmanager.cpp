

#include "voxelmanager.h"

#include <iostream>

namespace voxelmanager {

    int cubeSize = 10;
    const int VOXELS_TO_SPAWN = cubeSize * cubeSize * cubeSize;
    //------------------------------------------------
    //------------------------------------------------
    unsigned int sharedVAO = 0;
    unsigned int sharedVBO = 0;
    unsigned int sharedEBO = 0;
    unsigned int VBOPositions = 0;
    unsigned int VBOColors = 0;

    float cellSize = 0.02f;
    //------------------------------------------------
    //------------------------------------------------
    const int WORLD_MIN_X = 0;
    const int WORLD_MAX_X = 1000;
    const int WORLD_MIN_Y = 0;
    const int WORLD_MAX_Y = 500;

    size_t lastPositionSize = 0;
    size_t lastColorSize = 0;
    
    //------------------------------------------------
    //------------------------------------------------
    std::vector<glm::vec3> instancesPositions;
    std::vector<glm::vec3> instancesColors;
    
    //-------------------------------------------------
    //-------------------------------------------------
    std::random_device rd;
    std::mt19937 mt(rd());

    std::vector<glm::vec3> voxelColorsList = {
        {0.00f, 1.00f, 1.00f}, // Cyan neon
        {1.00f, 0.00f, 1.00f}, // Magenta neon
        {0.00f, 0.75f, 1.00f}, // Electric blue
        {0.50f, 0.00f, 1.00f}, // Vivid violet
        {0.00f, 1.00f, 0.50f}, // Aqua green
        {1.00f, 0.50f, 0.00f}, // Cyber orange
        {0.90f, 0.00f, 0.50f}, // Hot pink
        {0.00f, 0.50f, 1.00f}, // Digital sky
        {0.30f, 1.00f, 0.90f}, // Mint pulse
        {0.80f, 0.00f, 0.80f}, // Neon purple
        {0.00f, 0.90f, 0.70f}, // Teal glow
        {0.60f, 0.00f, 0.90f}, // Ultraviolet
        {0.00f, 0.80f, 1.00f}, // Holographic blue
        {1.00f, 0.20f, 0.60f}, // Raspberry beam
        {0.20f, 1.00f, 0.80f}, // Neon jade
        {0.90f, 0.40f, 1.00f}, // Plasma pink
        {0.10f, 0.90f, 1.00f}, // Ice circuit
        {1.00f, 0.00f, 0.30f}, // Laser red
        {0.00f, 0.60f, 1.00f}, // Blue flame
        {0.70f, 0.00f, 0.70f}, // Deep magenta
        {0.00f, 1.00f, 0.00f}, // Neon green
        {1.00f, 0.00f, 0.00f}, // Neon red
        {0.00f, 0.00f, 1.00f}, // Neon blue
        {1.00f, 1.00f, 0.00f}, // Neon yellow
        {0.50f, 1.00f, 0.00f}, // Lime flash
        {1.00f, 0.00f, 0.80f}, // Infra pink
        {0.00f, 0.40f, 1.00f}, // Deep tech blue
        {0.80f, 1.00f, 0.00f}, // Volt yellow
        {0.00f, 0.90f, 0.30f}, // Emerald neon
        {0.90f, 0.00f, 0.90f}, // Hyper purple
        {0.10f, 0.70f, 1.00f}, // Sky pulse
        {1.00f, 0.60f, 0.00f}  // Amber blaze
    };


    std::uniform_int_distribution<int> voxelColorsDist(0, voxelColorsList.size() - 1);

    // std::uniform_real_distribution<float> randomXPos(WORLD_MIN_X, (WORLD_MAX_X - 1.0f));
    // std::uniform_real_distribution<float> randomYPos(WORLD_MIN_Y, (WORLD_MAX_Y - 1.0f));
    std::uniform_real_distribution<float> randomXPos(0, 7.0f);
    std::uniform_real_distribution<float> randomYPos(0, 7.0f);


    //-------------------------------------------------
    //-------------------------------------------------


    //-------------------------------------------------
    //-------------------------------------------------



    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------

    //--------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------
    // setup VAO, VBOs and EBO for shared use
    void setupSharedCubeGeometry() {

        float cubeVertices[] = {
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f
        };

        unsigned int cubeIndices[] = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            0, 4, 7, 7, 3, 0,
            1, 5, 6, 6, 2, 1,
            3, 2, 6, 6, 7, 3,
            0, 1, 5, 5, 4, 0
        };

        std::cout  << "  Generating shared VAOs and buffers...\n";

        GLenum err = glGetError();

        glGenVertexArrays(1, &sharedVAO);
        if (err != GL_NO_ERROR) {
            std::cout  << " setupSharedCubeGeometry sharedVAO failed: GL error " << err << "\n";
            return;
        }

        glGenBuffers(1, &sharedVBO);
        if (err != GL_NO_ERROR) {
            std::cout  << " setupSharedCubeGeometry sharedVBO failed: GL error " << err << "\n";
            return;
        }

        glGenBuffers(1, &sharedEBO);
        if (err != GL_NO_ERROR) {
            std::cout  << " setupSharedCubeGeometry sharedEBO failed: GL error " << err << "\n";
            return;
        }


        glGenBuffers(1, &VBOPositions);
        if (err != GL_NO_ERROR) {
            std::cout << "VBOPositions failed: GL error " << err << "\n";
            return;
        }

        glGenBuffers(1, &VBOColors);
        if (err != GL_NO_ERROR) {
            std::cout << "VBOColors failed: GL error " << err << "\n";
            return;
        }



        std::cout  << "  bind VAO ...\n";
        glBindVertexArray(sharedVAO);
        if (err != GL_NO_ERROR) {
            std::cout  << " setupSharedCubeGeometry glBindVertexArray(sharedVAO); failed: GL error " << err << "\n";
            return;
        }

        std::cout  << " and setup buffers...\n";

        //--------------------------------------- vertex buffer (location =  0)
        glBindBuffer(GL_ARRAY_BUFFER, sharedVBO);
        if (err != GL_NO_ERROR) {
            std::cout  << " setupSharedCubeGeometry glBindBuffer(GL_ARRAY_BUFFER, sharedVBO); failed: GL error " << err << "\n";
            return;
        }

        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
        if (err != GL_NO_ERROR) {
            std::cout  << " setupSharedCubeGeometry glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW); failed: GL error " << err << "\n";
            return;
        }

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        if (err != GL_NO_ERROR) {
            std::cout  << " setupSharedCubeGeometry glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); failed: GL error " << err << "\n";
            return;
        }

        glEnableVertexAttribArray(0);
        if (err != GL_NO_ERROR) {
            std::cout  << " setupSharedCubeGeometry glEnableVertexAttribArray(0); failed: GL error " << err << "\n";
            return;
        }


        //------------------------------------- vertex buffer for positions (location =  1)
        glBindBuffer(GL_ARRAY_BUFFER, VBOPositions);
        if (err != GL_NO_ERROR) {
            std::cout << " glBindBuffer(GL_ARRAY_BUFFER, vboPositions);  failed" << err << "\n";
            return;
        }

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        if (err != GL_NO_ERROR) {
            std::cout << " glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); failed" << err << "\n";
            return;
        }

        glEnableVertexAttribArray(1);
        if (err != GL_NO_ERROR) {
            std::cout << "7) glEnableVertexAttribArray(1); failed" << err << "\n";
            return;
        }

        glVertexAttribDivisor(1, 1);
        if (err != GL_NO_ERROR) {
            std::cout << "8) glVertexAttribDivisor(1, 1);  failed" << err << "\n";
            return;
        }


        //------------------------------------------ vertex buffer for colors (location =  2)
        glBindBuffer(GL_ARRAY_BUFFER, VBOColors);
        if (err != GL_NO_ERROR) {
            std::cout << "9) glBindBuffer(GL_ARRAY_BUFFER, vboColors); failed" << err << "\n";
            return;
        }

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        if (err != GL_NO_ERROR) {
            std::cout << "10)  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); failed" << err << "\n";
            return;
        }

        glEnableVertexAttribArray(2);
        if (err != GL_NO_ERROR) {
            std::cout << "11) glEnableVertexAttribArray(2);  failed" << err << "\n";
            return;
        }

        glVertexAttribDivisor(2, 1);
        if (err != GL_NO_ERROR) {
            std::cout << "12) glVertexAttribDivisor(2, 1); failed" << err << "\n";
            return;
        }


        //------------------------------------------- index buffer 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sharedEBO);
        if (err != GL_NO_ERROR) {
            std::cout  << " setupSharedCubeGeometry glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sharedEBO); failed: GL error " << err << "\n";
            return;
        }

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);
        if (err != GL_NO_ERROR) {
            std::cout  << " glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW); failed: GL error " << err << "\n";
            return;
        }


        //------------ end of setup
        glBindVertexArray(0);
        if (err != GL_NO_ERROR) {
            std::cout  << " glBindVertexArray(0); 2 failed: GL error " << err << "\n";
            return;
        }
    
    };


    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    
    
    
    
    
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    // initializes positions and colors for all instances
    void initInstacesData(){

        
        //---------- init random positions
        //store grid positions, draw with world positions by multiplying by cellSize
        for (int i = 0; i < VOXELS_TO_SPAWN; ++i) {

            int x = i % cubeSize;
            int y = (i / cubeSize) % cubeSize;
            int z = i / (cubeSize * cubeSize);



            glm::vec3 newWorldPos = glm::vec3(x, y, z) * cellSize;

            //store world positions
            instancesPositions.push_back(newWorldPos);
            
        }



        //------------ init random colors for each voxel
        for (int i = 0; i < VOXELS_TO_SPAWN; ++i) {
            glm::vec3 randColor = voxelColorsList[voxelColorsDist(mt)];
            instancesColors.push_back(randColor);
        }

    };



    //--------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------
    // updates colors and positions of all the instances (voxels) that are going to be rendered
    void updateInstacesData(){


        if (!glfwGetCurrentContext()) {
            std::cerr << "ERROR: OpenGL context is not current — uploadInstanceData() called from wrong thread\n";
            assert(false && "OpenGL context is not current");
            return;
        }


        if (instancesPositions.empty()) return;


        //----------------------------------------------     for error log
        GLenum err = glGetError();

        //----------------------------------------------    bind VAO before setup
        //----------------------------------------------
        glBindVertexArray(sharedVAO);
        if (err != GL_NO_ERROR) {
            std::cerr << "14) glBindVertexArray(vao); failed" << err << "\n";
            return;
        }

        //----------------------------------------------
        //----------------------------------------------
        //----------------------------------------------
        //----------------------------------------------

        //-------------------------- to keep track of memory usage
        static size_t totalBufferBytes = 0;
        size_t thisUploadSize = 0;

        
        if((instancesPositions.size() * sizeof(glm::vec3) > 0)){
            if(lastPositionSize <= 0){
                thisUploadSize += instancesPositions.size() * sizeof(glm::vec3);
            }
        }

        if((instancesColors.size() * sizeof(glm::vec3) > 0)){
            if(lastColorSize <= 0){
                thisUploadSize += instancesColors.size() * sizeof(glm::vec3);
            }
        }

        
        //---------------------------------------------- POSITIONS BUFFER UPDATE

        //----------------------------------------------  check if size of positions has changed
        //----------------------------------------------  (that is , if more voxels were created)
        size_t newPositionSize = instancesPositions.size() * sizeof(glm::vec3);
        glBindBuffer(GL_ARRAY_BUFFER, VBOPositions);
        if (err != GL_NO_ERROR) {
            std::cerr << "14) glBindBuffer(GL_ARRAY_BUFFER, vboPositions); failed" << err << "\n";
            return;
        }

        //---------------------------------  if size increased, set the whole buffer again (resize)
        if (newPositionSize != lastPositionSize) {
            glBufferData(GL_ARRAY_BUFFER, newPositionSize, instancesPositions.data(), GL_DYNAMIC_DRAW);
            if (err != GL_NO_ERROR) {
                std::cerr << "15) glBufferData(GL_ARRAY_BUFFER, newPositionSize, positions.data(), GL_DYNAMIC_DRAW); failed" << err << "\n";
                return;
            }

            //update size with the current value
            lastPositionSize = newPositionSize;
            thisUploadSize += newPositionSize;

        } else {
            //-------------------------------------  if data changed, but size stays the same
            //-------------------------------------  update the buffer data (no resize of the buffer)
            glBufferSubData(GL_ARRAY_BUFFER, 0, newPositionSize, instancesPositions.data());
            if (err != GL_NO_ERROR) {
                std::cerr << "16)  glBufferSubData(GL_ARRAY_BUFFER, 0, newPositionSize, positions.data()); failed" << err << "\n";
                return;
            }

            thisUploadSize += instancesPositions.size() * sizeof(glm::vec3);
        }



        //----------------------------------------------
        //----------------------------------------------
        //----------------------------------------------
        //----------------------------------------------
        
        //---------------------------------------------- COLORS BUFFER UPDATE
        
        
        //---------------------------------------- same logic as positions, check size,
        //---------------------------------------- and update or resize the buffer
        size_t newColorSize = instancesColors.size() * sizeof(glm::vec3);
        glBindBuffer(GL_ARRAY_BUFFER, VBOColors);
        if (err != GL_NO_ERROR) {
            std::cerr << "17)   glBindBuffer(GL_ARRAY_BUFFER, vboColors); failed" << err << "\n";
            return;
        }

        if (newColorSize != lastColorSize) {
            glBufferData(GL_ARRAY_BUFFER, newColorSize, instancesColors.data(), GL_DYNAMIC_DRAW);
            if (err != GL_NO_ERROR) {
                std::cerr << "18)  glBufferData(GL_ARRAY_BUFFER, newColorSize, colors.data(), GL_DYNAMIC_DRAW); failed" << err << "\n";
                return;
            }

            //update size with the current value
            lastColorSize = newColorSize;
            thisUploadSize += newColorSize;

        } else {
            glBufferSubData(GL_ARRAY_BUFFER, 0, newColorSize, instancesColors.data());
            if (err != GL_NO_ERROR) {
                std::cerr << "19)  glBufferSubData(GL_ARRAY_BUFFER, 0, newColorSize, colors.data()); failed" << err << "\n";
                return;
            }

            thisUploadSize += instancesColors.size() * sizeof(glm::vec3);
        }



        //----------------------------------------  done using the vertex array
        glBindVertexArray(0);
        if (err != GL_NO_ERROR) {
            std::cerr << "20) glBindVertexArray(0); failed" << err << "\n";
            return;
        }
        
        
        
        
        //---------------------------------------- To keep track of memory usage
        totalBufferBytes = thisUploadSize; // overwrite instead of accumulate
        std::cout << "Total buffer memory: " << totalBufferBytes / 1024 << " KB\n";

    };


};
