

#include <iostream>
#include <stdlib.h>  // for rand()
#include <time.h>    // for seeding rand()




#include "voxelmanager.h"

#define STB_PERLIN_IMPLEMENTATION //for noise functions
#include "stb_perlin.h"
#include <algorithm>

namespace voxelmanager {

    int cubeSize = 20;
    const int VOXELS_TO_SPAWN = cubeSize * cubeSize * cubeSize;
    //------------------------------------------------
    //------------------------------------------------
    unsigned int sharedVAO = 0;
    unsigned int sharedVBO = 0;
    unsigned int sharedEBO = 0;
    unsigned int VBOPositions = 0;
    unsigned int VBOColors = 0;

    float cellSize = 0.02f;
    float invScalar = 1.0f / cellSize;
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
    std::vector<Voxel> voxelsList;
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

        srand(time(NULL)); // Seed the random number generator

        const float UPPER_BOUND_RAND = 100.0f;

        //---------- init random positions
        //store grid positions, draw with world positions by multiplying by cellSize
        for (int i = 0; i < VOXELS_TO_SPAWN; ++i) {

            int x = i % cubeSize;
            int y = (i / cubeSize) % cubeSize;
            int z = i / (cubeSize * cubeSize);

            float rx = rand() / (float)RAND_MAX * UPPER_BOUND_RAND;
            float ry = rand() / (float)RAND_MAX * UPPER_BOUND_RAND;
            float rz = rand() / (float)RAND_MAX * UPPER_BOUND_RAND;

            bool insertEmpty = (rand() / (float)RAND_MAX * 2.0f) >= 1;
            // if(rx > 50 && ry < (rz+ry) && rz > (rx)){
            //     insertEmpty = true;
            // }

            // if( ry > (rz-ry) && rz + ry > (rx)){
            //     insertEmpty = true;
            // }

            // if(rz > 50 ){
            //     insertEmpty = true;
            // }

            Voxel newVoxel;
            newVoxel.gridPos = glm::vec3(x, y, z);
            newVoxel.position = glm::vec3(x, y, z) * cellSize;
            newVoxel.type = insertEmpty ? VoxelType::EMPTY : VoxelType::ALIVE;
            newVoxel.color = voxelColorsList[voxelColorsDist(mt)];

            //store voxel
            voxelsList.push_back(newVoxel);

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


        //generate the new positions and colors array
        //------------ init random colors for each voxel
        instancesPositions.clear();
        instancesColors.clear();
        //--------
        for (const Voxel& voxel : voxelsList) {
            if(voxel.type != VoxelType::EMPTY){
                instancesPositions.push_back(voxel.position);
                instancesColors.push_back(voxel.color);
            }
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



    //--------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------
    //----------------------------- convert to grid position (hacer helper)
    glm::vec3 WorldToGridPosition(glm::vec3 worldPos){

        glm::vec3 gridPos = worldPos * invScalar;
        return gridPos;
        //---------- multiplying or dividing the cell position by the cellSize
        //---------- gives you the coordinate in the grid

        //----------  FROM GRID COORDS                     ----  TO WORLD COORDS
        //---------- ( x = 3 , y = 8 , z = 11 ) *  cellSize ==> ( x = 0.06 , y = 0.16 , z = 0.22 )
        
        //----------  FROM WORLD COORDS                     ----  TO GRID COORDS
        //---------- ( x = 0.06 , y = 0.16 , z = 0.22 ) / cellSize ==> ( x = 3 , y = 8 , z = 11 )
    };


    //--------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------
    //--------------------------------------------------------------------------------------------
    //main simulation loop, runs every frame in main render loop
    void updateVoxelPositions(){

        std::cout << "creating new simulation frame..." << std::endl;

        // std::vector<Voxel> voxelToAdd; 
        //don't need to remove , just set to EMPTY to skip rendering

        for (Voxel& voxel : voxelsList) {
            
            int neighborCount = 0;
            int emptyNeighborCount = 0;

            //this would be the center of the 3x3x3 cube grid
            glm::vec3 currGridCenterPos = voxel.gridPos;

            //loop the 3x3x3 grid
            //for now we use the basic game of life rules
            for (int dx = -1; dx <= 1; ++dx) {
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dz = -1; dz <= 1; ++dz) {
                        
                        if (dx == 0 && dy == 0 && dz == 0) continue; // skip center
                        
                        int nx = currGridCenterPos.x + dx;
                        int ny = currGridCenterPos.y + dy;
                        int nz = currGridCenterPos.z + dz;

                        // process neighbor at (nx, ny, nz)
                        glm::ivec3 gridNeighbor = glm::vec3(nx, ny, nz);
                        auto it = std::find_if(voxelsList.begin(), voxelsList.end(),
                            [&](const Voxel& voxel) {
                                return voxel.gridPos == gridNeighbor;
                            });

                        if (it != voxelsList.end()) {
                            // Found the voxel
                            Voxel& foundVoxel = *it;
                            // Use foundVoxel
                            if(foundVoxel.type == VoxelType::EMPTY){
                                emptyNeighborCount += 1;
                            }else{
                                neighborCount += 1;
                            }
                        }
                        //  else {
                        //     // Not found
                        // }
                    }
                }
            }//--------------------- end of neighbours check


            //now that the neighbors are counted, apply the rules
            
            //cell dies by underpopulation
            if(neighborCount < 2){
                voxel.type = VoxelType::EMPTY;
            }else if(voxel.type == VoxelType::ALIVE && neighborCount > 1 && neighborCount < 4 ){
                //do nothing here, cell lives for another generation
            }else if(voxel.type == VoxelType::ALIVE && neighborCount > 3){
                //cell dies, as if by overpopulation.
                voxel.type = VoxelType::EMPTY;
            }else if(voxel.type == VoxelType::EMPTY && neighborCount > 3){
                //becomes a live cell, as if by reproduction.
                voxel.type = VoxelType::ALIVE;
            }

        }//--------- end of for loop through all voxels

        //after updating all the positions and states in the array, update gpu buffers
        updateInstacesData();
    };


};
