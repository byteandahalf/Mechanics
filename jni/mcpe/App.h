#pragma once
#include "AppPlatformListener.h"
struct AppContext;
#include "util/Token.h"
#include <memory>
class AppPlatform;
#include <string>






class App : public AppPlatformListener {


public:
	App(int, char**);

	virtual ~App();

	void init(AppContext&);
	bool isInited();

	virtual AppPlatform* platform();

	void changeContext(AppContext&);

	virtual void audioEngineOn();
	virtual void audioEngineOff();

	virtual void muteAudio();
	virtual void unMuteAudio();

	virtual void destroy();

	const Token::List& getCommandLine();



	const Token* getCommandLineParameter(const std::string&);

	virtual void loadState(void*, int);
	virtual bool saveState(void**, int*);
	virtual bool useTouchscreen();
	virtual void setTextboxText(const std::string&);

	virtual void draw();
	virtual void update();
	virtual void setSize(int, int, float);

	virtual void quit();
	virtual bool wantToQuit();
	virtual bool handleBack(bool);
	virtual void handleBack();

protected:
	virtual void init();


private:
	bool _inited;
	bool _finished;
	AppContext* _context;
	Token::List commandLine;
};
