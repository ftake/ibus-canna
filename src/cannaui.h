#ifndef _CANNAUI_H
#define _CANNAUI_H

#include <string>
#include <ibus.h>
//define CANNA_WCHAR16
#define CANNA_NEW_WCHAR_AWARE
#include <canna/jrkanji.h>


typedef
struct {
} CannaUISettings;

static const int BUF_SIZE = 1024;

class CannaUI {
public:
	CannaUI(CannaUISettings* settings);
	virtual ~CannaUI(void);
	IBusText* getKanjiList(void);
	IBusText* getEcho(void);
	int sendKey(int key, std::string& converted);
//	void setHenkanMode();
	
private:
	wcKanjiStatus kanjiStatus;
	unsigned char buf[BUF_SIZE];
};

#endif
