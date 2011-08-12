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

	using namespace icu;
	UnicodeString u32BeforeRev(
		const_cast<char*>(beforeRevEUC.c_str()), "euc-jp");
	UnicodeString u32Rev(
		const_cast<char*>(reverseEUC.c_str()), "euc-jp");
	UnicodeString u32AfterRev(
		const_cast<char*>(afterRevEUC.c_str()), "euc-jp");

	this->revPos = u32BeforeRev.countChar32();
	this->revLen = u32Rev.countChar32();
	
	this->length = this->revPos + this->revLen + u32AfterRev.countChar32();

	u32BeforeRev += u32Rev;
	u32BeforeRev += u32AfterRev;
	u32BeforeRev.toUTF8String(uText);
	DEBUGM("%s\n", uText.c_str());
}

CannaText::~CannaText(void) {
}

IBusText* CannaText::getIBusText() {
	IBusText* result = ibus_text_new_from_string(uText.c_str());
	ibus_text_append_attribute(result, IBUS_ATTR_TYPE_BACKGROUND, 0x00FFFF, revPos, revPos + revLen);
//	ibus_text_append_attribute(result, IBUS_ATTR_TYPE_FOREGROUND, 0xFFFFFF, revPos, revPos + revLen);
	return result;
}

int CannaText::getLength() {
	return this->length;
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
	
	echo = 0;
	kanjiList = 0;
}

CannaUI::~CannaUI() {
	jrKanjiControl(0, KC_FINALIZE, 0);
	delete this->echo;
	delete this->kanjiList;
}


IBusText* CannaUI::getKanjiList(void) {
	if (kanjiList != 0) {
		return kanjiList->getIBusText();
	} else {
		return 0;
	}
}

IBusText* CannaUI::getEcho(void) {
	if (this->echo != 0) {
		return echo->getIBusText();
	} else {
		return ibus_text_new_from_static_string("");
	}
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

	// ** update echo **
	// delete previous echo
	delete this->echo;
	if (kanjiStatus.length == 0) {
		this->echo = 0;
	} else {
		this->echo = new CannaText(
			reinterpret_cast<char*>(kanjiStatus.echoStr),
			kanjiStatus.revPos, kanjiStatus.revLen);
	}

	// ** update kanjiList **
	if ((kanjiStatus.info & KanjiGLineInfo) != 0) {
		delete this->kanjiList;
		if (kanjiStatus.gline.length == 0) {
			this->kanjiList = 0;
		} else {
			this->kanjiList = new CannaText(
				reinterpret_cast<char*>(kanjiStatus.gline.line),
				kanjiStatus.gline.revPos, kanjiStatus.gline.revLen);
		}
	}

	if (ret != 0) {
		convertToUTF8(buffer, converted);
	}
	return true;
}

