#ifndef _CANNAUI_H
#define _CANNAUI_H

typedef
struct {
} CannaUISettings;

class CannaUI {
public:
	CannaUI(CannaUISettings* settings);
	virtual ~CannaUI(void);
};

#endif
