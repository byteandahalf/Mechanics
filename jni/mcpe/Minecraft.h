/*
* @Credits to MCMrARM for this header
*/

#pragma once

#include <string>
#include <memory>
#include "App.h"

#include "LevelStorageState.h"
#include "App.h"
#include "../mcpe.h"

class LevelStorageSource;
class ICreator;
class Timer;
class User;
class MojangConnector;
class GameMode;
class IRakNetInstance;
class NetEventCallback;
class LevelSettings;

class Screen
{
public:
	char filler[104];
	int width;
	int height;
};

class Minecraft : public App {

public:
        Minecraft(int, char**);
        virtual ~Minecraft();

        virtual void teardown();

        virtual void init();
        void init(const std::string&);

        bool supportsNonTouchScreen();
        void toogleDimension();
        bool isCreativeMode();
        void setIsCreativeMode(bool);

        virtual LevelStorageState selectLevel(const std::string&, const std::string&, const LevelSettings&);
        virtual void setLevel(std::unique_ptr<Level>, const std::string&, Player*);

        LevelStorageSource* getLevelSource();

        bool isLookingForMultiplayer;
        void locateMultiplayer();
        void cancelLocateMultiplayer();
        void hostMultiplayer(int);
        void doActuallyRespawnPlayer();

        virtual void update();

        virtual void startFrame();
        virtual void updateGraphics();
        virtual void endFrame();

        virtual void tick();

        void tickInput();

        bool isOnlineClient();
        bool isOnline();

        virtual void leaveGame();

        double getLastFrameDuration();



        ICreator* getCreator();
        ServerCommandParser getCommandParser();

        bool isPowerVR();
        bool isKindleFire(int);
        bool transformResolution(int*, int*);
        void lockForControl();
        void unlockControl();
        void rebuildLevel();

        virtual void play(const std::string&, float, float, float, float, float);
        virtual void playUI(const std::string&, float, float);
        virtual void updateSoundLocation(Mob*, float);

        void resetInput();

        virtual bool isServerVisible();

        std::string getServerName();
        virtual void sendLocalMessage(const std::string&, const std::string&);

        virtual Player* getPlayer();
        virtual void onInternetUpdate();

        virtual void onPlayerLoaded(Player&);

private:
        void* prepareLevel_tspawn(void*);

public:
        int width;
        int height;


        int commandPort;
        int reserved_d1, reserved_d2;
        float reserved_f1, reserved_f2;

        volatile bool pause;

        GameMode* gameMode;

        std::shared_ptr<MojangConnector> mojangConnector;

        std::unique_ptr<IRakNetInstance> raknetInstance;
        std::unique_ptr<NetEventCallback> netCallback;

        int lastTime;
        int lastTickTime;
        float ticksSinceLastUpdate;
        int clientRandomId;

        User* user;
        std::unique_ptr<Level> level;

        std::shared_ptr<Screen> screen;
        int customDebugId;

        const int CDI_NONE = 0;
        const int CDI_GRAPHICS = 1;

        bool mouseGrabbed;



        std::string externalStoragePath;
        std::string externalCacheStoragePath;

        void setLeaveGame();
        void removeAllPlayers();

        void onUpdatedClient(int, int, int, int);

        virtual void vibrate(int);

protected:

        std::unique_ptr<Timer> timer;

        LevelStorageSource* storageSource;
        bool _running;
        bool _powerVr;

        std::string homeFolder;


        int ticks;

        bool _isCreativeMode;

        bool _shouldLeaveGame;


        std::unique_ptr<ServerCommandParser> commandParser;

        double frameDuration, lastFrameStart;
};
