#include "cannaui.h"

#include <iostream>
#include <cstdio>
#include <string>
#include <unicode/unistr.h>

// Utility functions
static void convertToUTF8(const std::string& src, std::string& dst) {
	icu::UnicodeString ubuf(const_cast<char*>(src.c_str()), "euc-jp");
	ubuf.toUTF8String(dst);
}

static void convertToUTF8(unsigned char* src, std::string& dst) {
	icu::UnicodeString ubuf(reinterpret_cast<char*>(src), "euc-jp");
	ubuf.toUTF8String(dst);
}

static void convertToUTF8(const char* src, std::string& dst) {
	icu::UnicodeString ubuf(const_cast<char*>(src), "euc-jp");
	ubuf.toUTF8String(dst);
}

/*
static void convertToUTF8(const cannawc* src, std::string& dst) {
	icu::UnicodeString ubuf((char*)src, "UCS-4-INTERNAL");
	ubuf.toUTF8String(dst);
}
*/

// end

CannaText::CannaText(const char* str, const int revPos,
					 const int revLen) {
	std::string beforeRevEUC(str, revPos);
	std::string reverseEUC(str + revPos, revLen);
	std::string afterRevEUC(str + revPos + revLen);

	convertToUTF8(beforeRevEUC, uText);
	std::string uRev;
	convertToUTF8(reverseEUC, uRev);
	std::string uAfterRev;
	convertToUTF8(afterRevEUC, uAfterRev);
	this->revPos = uText.size();
	this->revLen = uRev.size();
	uText.append(uRev);
	uText.append(uAfterRev);
}

CannaText::~CannaText(void) {

}

IBusText* CannaText::getIBusText() {
	IBusText* result = ibus_text_new_from_string(uText.c_str());
	ibus_text_append_attribute(result, IBUS_ATTR_TYPE_BACKGROUND, 0x00FFFF, revPos, revPos + revLen);
//	ibus_text_append_attribute(result, IBUS_ATTR_TYPE_FOREGROUND, 0xFFFFFF, revPos, revPos + revLen);
	return result;
}


CannaUI::CannaUI(CannaUISettings* settings) {
	int ret = jrKanjiControl(0, KC_INITIALIZE, 0);
	if (ret < 0) {
		DEBUGM("KC_INITIALIZE: %d", ret);
	}
	
	jrKanjiStatusWithValue statusv;
	unsigned char buf[8];
	statusv.ks = &kanjiStatus;
	statusv.buffer = buf;
	statusv.bytes_buffer = 8;
	statusv.val = CANNA_MODE_HenkanMode;
	jrKanjiControl(0, KC_CHANGEMODE, (char*)&statusv);
}

CannaUI::~CannaUI() {
	jrKanjiControl(0, KC_FINALIZE, 0);
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
	if (kanjiStatus.length == 0) {
		return ibus_text_new_from_string("");
	}
	/*
	int revPos = kanjiStatus.revPos;
	int revLen = kanjiStatus.revLen;
	std::string beforeRevEUC((char*)kanjiStatus.echoStr, revPos);
	std::string reverseEUC((char*)kanjiStatus.echoStr + revPos, revLen);
	std::string afterRevEUC((char*)kanjiStatus.echoStr + revPos + revLen);

	std::string beforeRev;
	convertToUTF8(beforeRevEUC.c_str(), beforeRev);
	std::string reverse;
	convertToUTF8(reverseEUC.c_str(), reverse);
	std::string afterRev;
	convertToUTF8(afterRevEUC.c_str(), afterRev);

	int u8RevPos = beforeRev.length();
	int u8RevLen = reverse.length();
	beforeRev.append(reverse);
	beforeRev.append(afterRev);

	IBusText* result = ibus_text_new_from_string(beforeRev.c_str());
	DEBUGM("getEcho: \"%s\"\n", beforeRev.c_str());
//	ibus_text_append_attribute(result, IBUS_ATTR_TYPE_UNDERLINE, 0, u8RevPos, u8RevPos + u8RevLen);
	ibus_text_append_attribute(result, IBUS_ATTR_TYPE_BACKGROUND, 0x00FFFF, u8RevPos, u8RevPos + u8RevLen);
	ibus_text_append_attribute(result, IBUS_ATTR_TYPE_BACKGROUND, 0x00FFFF, u8RevPos, u8RevPos + u8RevLen);
	*/
	CannaText ctext((char*)kanjiStatus.echoStr, kanjiStatus.revPos, kanjiStatus.revLen);
	return ctext.getIBusText();
}

/**
 * @return true: the key is handled by canna
 */
bool CannaUI::sendKey(int key, std::string& converted) {
	char buffer[BUF_SIZE];
	int ret = jrKanjiString(0, key, buffer, 1024, &kanjiStatus);

	if ((kanjiStatus.info & KanjiThroughInfo) != 0) {
		return false;
	}

	if (ret != 0) {
		convertToUTF8(buffer, converted);
	}
	return true;
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
