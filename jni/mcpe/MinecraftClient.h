/*
* @Credits to MCMrARM for this header
*/


#pragma once

class MinecraftClient;

#include <thread>
#include <mutex>
#include <memory>

#include "../mcpe.h"
#include "Minecraft.h"

class Gui;
class ScreenChooser;
class GameRenderer;
class PixelCalc;
class LocalPlayer;
class SoundEngine;
class PingedCompatibleServer;
class TextureAtlas;
class LevelRenderer;
class ParticleEngine;
class BuildActionIntention;
class ExternalServerFile;
class MouseHandler;
class IInputHolder;
class Options
{
public:
	class Option;
};

class MinecraftClient : public Minecraft {

public:

        static const int INTERACTION_FREQ_MS;
        MinecraftClient(int, char**);
        ~MinecraftClient();

        virtual void init();
        virtual void teardown();

        virtual void startFrame();
        virtual void updateGraphics();
        virtual void endFrame();

        virtual bool handleBack(bool);
        virtual void handleBack();

        virtual void onLowMemory();

        virtual void play(const std::string&, float, float, float, float, float);
        virtual void playUI(const std::string&, float, float);

        void setScreen(Screen*);
        std::unique_ptr<ScreenChooser> screenChooser;

        void handleBackNoReturn();

        virtual bool useTouchscreen();
        bool supportNonTouchScreen();
        virtual void setSize(int, int, float);

        void optionUpdated(const Options::Option*, bool);

        void optionUpdated(const Options::Option*, float);

        void optionUpdated(const Options::Option*, int);

        void _reloadFancy(bool);

        virtual LevelStorageState selectLevel(const std::string&, const std::string&, const LevelSettings&);
        Options& getOptions();
        virtual void setTextboxText(const std::string&);

        void onUpdatedClient(int, int, int, int);

        void rebuildLevel();
        bool isKindleFire(int);

        bool transformResolution(int*, int*);

        virtual void onAppFocusLost();
        virtual void onAppFocusGained();

        virtual void onAppSuspended();

        virtual void onAppResumed();

        virtual void onInternetUpdate();

        virtual void onPlayerLoaded(Player&);

        void updateScheduledScreen();

        virtual void leaveGame(bool, bool);

        virtual void setLevel(std::unique_ptr<Level>, const std::string&, Player*);


        std::shared_ptr<Screen>& getScreen();



        GameRenderer& getGameRenderer();



        Gui& getGui();

        bool joinMultiplayer(const PingedCompatibleServer&, bool);
        virtual void vibrate(int);

        void locateMultiplayer();
        virtual bool isServerVisible();
        virtual Player* getPlayer();
        virtual LocalPlayer* getLocalPlayer();
        static void resetInput();
        bool useController();
        PixelCalc& getPixelCalc();
        PixelCalc& getPixelCalcUI();

        void lockForControl();

        void unlockControl();


        void initGLStates();
        void restartServer();

        void updateStats();








private:
        bool _verbose;
        int _frames;
        int _lastTickMs;

        static bool _hasInitedStatics;

        static std::shared_ptr<TextureAtlas> _terrainTextureAtlas;
        static std::shared_ptr<TextureAtlas> _itemsTextureAtlas;

        std::vector<bool> _handleBackEvent;

        virtual void audioEngineOn();
        virtual void audioEngineOff();
        virtual void muteAudio();
        virtual void unMuteAudio();

        virtual void tick(int, int);
        void tickInput();

        void handleMouseDown(int, bool);
        void handleMouseClick(int);

        void _reloadInput();
        void handleBuildAction(BuildActionIntention*);

        virtual void sendLocalMessage(const std::string&, const std::string&);
        void grabMouse();
        void releaseMouse();

        void initFoliageAndTileTextureTessellator();
        void _loadMaterials();
public:
        std::unique_ptr<LevelRenderer> levelRenderer;
        std::unique_ptr<GameRenderer> gameRenderer;
        std::unique_ptr<ParticleEngine> particleEngine;
        std::unique_ptr<ExternalServerFile> _externalServer;
        std::unique_ptr<Textures> textures;
        std::unique_ptr<Font> font;
        std::unique_ptr<MouseHandler> mouseHandler;
        std::unique_ptr<IInputHolder> inputHolder;
        Mob* cameraTargetPlayer;
private:
        LocalPlayer* player;
        std::unique_ptr<Gui> gui;
        std::unique_ptr<Options> _options;
        std::unique_ptr<SoundEngine> soundEngine;

        bool hasFocus;

        int missTime;
        int lastInteractionTime;

        bool screenIsTicking;
        bool hasScheduledScreen;
        Screen* scheduledScreen;

        bool _supportsNonTouchscreen;
        std::unique_ptr<PixelCalc> pixelCalc;
        std::unique_ptr<PixelCalc> pixelCalcUi;
        std::mutex controllerLock;
        bool hasRunTickingTextureTicks;
};
