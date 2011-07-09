#include "cannaui.h"

#include <iostream>
#include <cstdio>
#include <string>
#include <unicode/unistr.h>

// Utility functions
#define DEBUGM(...) fprintf(stderr, __VA_ARGS__)

/*
static void convertToUTF8(unsigned char* src, std::string& dst) {
	icu::UnicodeString ubuf(reinterpret_cast<char*>(src), "euc-jp");
	ubuf.toUTF8String(dst);
}
static void convertToUTF8(char* src, std::string& dst) {
	icu::UnicodeString ubuf(src, "euc-jp");
	ubuf.toUTF8String(dst);
}
*/

static void convertToUTF8(const cannawc* src, std::string& dst) {
	icu::UnicodeString ubuf((char*)src, "UTF32LE");
	ubuf.toUTF8String(dst);
}


// end

CannaUI::CannaUI(CannaUISettings* settings) {
	int ret = wcKanjiControl(0, KC_INITIALIZE, 0);
	if (ret < 0) {
		DEBUGM("KC_INITIALIZE: %d", ret);
	}
	
	wcKanjiStatusWithValue statusv;
	cannawc buf[8];
	statusv.ks = &kanjiStatus;
	statusv.buffer = buf;
	statusv.n_buffer = 8;
	statusv.val = CANNA_MODE_HenkanMode;
	wcKanjiControl(0, KC_CHANGEMODE, (char*)&statusv);
}

CannaUI::~CannaUI() {
	wcKanjiControl(0, KC_FINALIZE, 0);
}


IBusText* CannaUI::getKanjiList(void) {
	std::string s;
	convertToUTF8(kanjiStatus.gline.line, s);
	if (kanjiStatus.gline.length != 0) {
		IBusText* result = ibus_text_new_from_string(s.c_str());
//	ibus_text_append_atribute(result, IBUSXX, from, to);
		return result;
	} else {
		return 0;
	}
}

IBusText* CannaUI::getEcho(void) {
	std::string s;
	if (kanjiStatus.length > 0) {
		convertToUTF8(kanjiStatus.echoStr, s);
	}
	IBusText* result = ibus_text_new_from_string(s.c_str());
	DEBUGM("getEcho: %s\n", s.c_str());
//	ibus_text_append_atribute(result, IBUSXX, from, to);
	return result;
}

/**
 * @return  length of converted string bytes
 */
int CannaUI::sendKey(int key, std::string& converted) {
	cannawc buffer[BUF_SIZE];
	int ret = wcKanjiString(0, key, buffer, 1024, &kanjiStatus);
	DEBUGM("sendKey: key = %d, ret = %d\n", key, ret);
	if (ret != 0) {
		convertToUTF8(buffer, converted);
		return converted.length();
	} else {
		return 0;
	}
}

/*
void CannaUI::setHenkanMode() {
	jrKanjiStatusWithValue stv;
	unsigned char buf[1024];
	stv.ks = &status;
	stv.buffer = buf;
	stv.bytes_buffer = 1024;
	stv.val = CANNA_MODE_HenkanMode;
	jrKanjiControl(0, KC_CHANGEMODE, (char*)&stv);
}
*/

//
//
//
/*
void print_euc(char* str) {
	icu::UnicodeString ubuf(str, "euc-jp");
	std::string s;
	ubuf.toUTF8String(s);
	std::cout << s;
}

void print_eucw(cannawc* str) {
	icu::UnicodeString ubuf((UChar*)str);
	std::string s;
	ubuf.toUTF8String(s);
	std::cout << s;
}

int main____(void) {
	int ret;

	ret = jrKanjiControl(0, KC_INITIALIZE, 0);
	std::cout << ret << std::endl;

	jrKanjiStatus status;
	{
		jrKanjiStatusWithValue stv;
		unsigned char buf[1024];
		stv.ks = &status;
		stv.buffer = buf;
		stv.bytes_buffer = 1024;
		stv.val = CANNA_MODE_HenkanMode;
		jrKanjiControl(0, KC_CHANGEMODE, (char*)&stv);
	}

	std::string keys = "kyou   \ntesuto@@aaa\n";
	char buffer[1024];
	for (unsigned int i = 0; i < keys.length(); i++) {
		ret = jrKanjiString(0, keys[i], buffer, 1024, &status);
		std::cout << "echo: ";
		print_euc((char*)status.echoStr);
		std::cout << std::endl;
		std::cout << "cand: ";
		print_euc((char*)status.gline.line);
		std::cout << std::endl;
		buffer[ret] = '\0';
		std::cout << ret << ": ";
		print_euc(buffer);
		std::cout << std::endl;
		
		{
			wchar_t canbuf[1024];
			int ret = RkwGetKanjiList(0, canbuf, 1024);
			if (ret != -1) {
				std::cout << "kanji list: " << ret << " : ";
				canbuf[1023] = '\0';
				print_eucw(canbuf);
				std::cout << std::endl;
			}
		}
	}

	jrKanjiControl(0, KC_FINALIZE, 0);

	return 0;
}
*/
