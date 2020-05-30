#ifndef CG1_PROJECT_USERINTERFACE_H
#define CG1_PROJECT_USERINTERFACE_H

#include <cg/GLHeader.h>
#include <cg/GLContext.h>
#include <cg/terrain/Terrain.h>
#include "EntityWindow.h"
#include "PlanetEditor.h"
#include "SkyboxEditor.h"

class UserInterface {
public:
    static const uint16_t MAX_PROFILE_LENGTH = 32;

    explicit UserInterface(EntityManager *entityManager, GLContext *context);
    ~UserInterface();

    void init();

    void render();

    void onUpdateTerrain(Terrain *terrain, std::function<bool(Terrain *, TerrainSettings& settings)> updateTerrain);

    void setupTheme(bool styleDark, float alpha);

    TerrainSettings settings;

private:
    EntityManager *entityManager;
    GLContext *context;

    PlanetEditor *planetEditor;
    EntityWindow *entityWindow;
    SkyboxEditor *skyboxEditor;

    bool showSpaceDisplay = true;
    bool showDemoWindow = false;
    bool showTerrainGeneratorWindow = false;
    bool showPlanetEditor = false;
    bool showWireframe = false;
    bool showEntityWindow = false;
    bool showSkyboxEditor = false;

    char* currentView = nullptr;
    std::vector<const char *> views;

    char currentProfile[MAX_PROFILE_LENGTH];
    std::vector<std::string> availableProfiles;

    char* currentNoise = nullptr;
    std::vector<const char *> noiseFunctions;

    ImVec2 cameraWindowSize;

    Terrain *terrain;
    std::function<bool(Terrain *, TerrainSettings& settings)> updateTerrainFunc = nullptr;
    std::string generateTerrainButtonText;

    bool showEntityNames = true;
    void renderEntityNames();

    void updateTerrain(Terrain *terrain, TerrainSettings& settings);

    void renderSpaceDisplay();
    void renderAppInfoDisplay();
    void renderMainMenu();
    void renderSceneInfoWindow();
    void renderCameraInfoWindow();
    void renderTerrainGeneratorWindow();

    std::string settingsFilename;
    char terrainProfileName[MAX_PROFILE_LENGTH];

    void loadTerrainProfiles();
    void loadTerrainSettings();
    void saveTerrainSettings();
};

#endif //CG1_PROJECT_USERINTERFACE_H
