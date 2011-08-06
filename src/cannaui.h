#ifndef _CANNAUI_H
#define _CANNAUI_H

#include <string>
#include <ibus.h>
//define CANNA_WCHAR16
#define CANNA_NEW_WCHAR_AWARE
#include <canna/jrkanji.h>

#define DEBUGM(...) fprintf(stderr, __VA_ARGS__)


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
	bool sendKey(int key, std::string& converted);
//	void setHenkanMode();
	
private:
	jrKanjiStatus kanjiStatus;
	unsigned char buf[BUF_SIZE];
};


class CannaText {
private:
	std::string uText;
	int revPos;
	int revLen;
public:
	CannaText(const char* str, const int revPos, const int revLen);
	virtual ~CannaText(void);
	IBusText* getIBusText(void);
};

#endif
