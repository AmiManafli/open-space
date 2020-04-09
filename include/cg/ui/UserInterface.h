#ifndef CG1_PROJECT_USERINTERFACE_H
#define CG1_PROJECT_USERINTERFACE_H

#include <cg/GLHeader.h>
#include <cg/GLContext.h>
#include <cg/terrain/Terrain.h>

class UserInterface {
public:
    explicit UserInterface(EntityManager *entityManager, GLContext *context);

    void render();

    void onUpdateTerrain(Terrain *terrain, std::function<bool(Terrain *, TerrainSettings& settings)> updateTerrain);

    TerrainSettings settings;

private:
    EntityManager *entityManager;
    GLContext *context;

    bool showDemoWindow = false;
    bool showTerrainGeneratorWindow = true;

    char* currentView = nullptr;
    std::vector<const char *> views;

    ImVec2 cameraWindowSize;

    Terrain *terrain;
    std::function<bool(Terrain *, TerrainSettings& settings)> updateTerrain = nullptr;
    std::string generateTerrainButtonText;

    void renderMainMenu();
    void renderSceneInfoWindow();
    void renderCameraInfoWindow();
    void renderTerrainGeneratorWindow();
};

#endif //CG1_PROJECT_USERINTERFACE_H
