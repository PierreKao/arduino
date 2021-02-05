
#ifndef LD3320_H
#define LD3320_H

#include <Arduino.h>
#include <stdio.h>    
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

class VoiceRecognition
{

public:
	VoiceRecognition();
	// void reset();
	void init();
	// unsigned char start();
	int addCommand(char *pass, int num);
	int read();
	void start();
	
private:
};

#endif
