#include"cannaui.h"

#include<iostream>
#include<string>
#include<canna/jrkanji.h>
#include<unicode/unistr.h>
#include<canna/RK.h>

CannaUI::CannaUI(CannaUISettings* settings) {

}

CannaUI::~CannaUI() {

}


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
