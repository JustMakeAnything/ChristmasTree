#include <Arduino.h>
#include "constants.h"
#ifdef DEBUG
#include "debug.h"
#endif
#include "touch.h"
#include "patterns.h"
#include "patternBase.h"
#include "ledStrip.h"
#include "colorTools.h"
#include "fastMath.h"
#include "configuration.h"
#pragma GCC diagnostic ignored "-Wunused-parameter"

static uint16_t numLeds = getAmountOfLeds();
patternParams_S patternPDefault = {0,0,Single,false,0,0,0,0,255,false,MOver,NULL,0};
patternParams param; // Reusable to save memory


void fade(uint8_t intensity){
	param = patternPDefault;
	param.baseColor = getLedColor(0,0,0,0); 
	param.shape = AllOn;
	param.merge = MOver;
	param.size  = 65535;
	param.intensity = intensity;
	pattern(param);
}

void patternstretchOut();
void patternTrigger();
void patternSeesaw();
void patternSnowGreen();
void patternSnowNight();

// List of patterns to cycle through.  
// Each is defined as a separate function below.
// There is a limit of patterns in constants.h->MAXPATTERNS
SimplePatternList patternList = { 
	patternRainbowCallback,
	patternSnowNight,
	patternLeftRight,
	patternCloudySky,
	patternFire,
	patternRedWhite,
//	patternTrigger,
//	patternSeesaw,
//	patternstretchOut,
//	patternDrops,
//	patternLeftRightDouble,
//	patternstretchOut,
//	patternSunriseNew,
};

void executePattern(uint8_t patternNumber){
	setSeed(micros()%65536);
	patternList[patternNumber]();
}


#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
uint8_t getPatternCount(){
	return min(ARRAY_SIZE(patternList),MAXPATTERNS);
}

void patternTrigger(){
	allOff();
	param = patternPDefault;
	param.baseColor = getLedColor(255,0,getTriggerSensorValue(),0); 
	param.shape = Solid;
	param.merge = MMax;
	param.intensity = 255;
	param.speed = 0;
	param.size  = getTriggerSensorValue()*255;
	pattern(param);

}

void patternLeftRight(){
	fade(20);
	param = patternPDefault;
	param.baseColor = getLedColor(255,0,0,0); 
	param.shape = Single;
	param.merge = MMax;
	param.intensity = 255;
	param.speed = 1000;
	param.size  = 15000;
	param.pause =  15000;
	param.strip = 1;
	pattern(param);
	param.pause =  65535;
	param.pausestart = 65535;
	param.speed = -500;
	param.baseColor = getLedColor(0,0,255,0); 
	pattern(param);
}

void patternSnowGreen(){
	allOff();
	param = patternPDefault;
	param.baseColor = getLedColor(0,150,0,0); // Dark green
	param.shape = AllOn;
	param.merge = MMax;
	param.intensity = 255;
	pattern(param);

	param = patternPDefault;
	param.baseColor = getLedColor(255,0,0,255); 
	param.shape = Single;
	param.merge = MMax;
	param.intensity = 50;
	param.speed = -1500;
//	param.size  = 15000;
//	param.pause =  65535;
    param.strip = 1;
	pattern(param);
	param.speed = -700;
	param.baseColor = getLedColor(0,0,255,255); 
    param.strip = 2;
	pattern(param);
}

void patternSnowNight(){
	fade(50);
	param = patternPDefault;
	param.baseColor = getLedColor(0,255,0,0); 
	param.shape = AllOn;
	param.intensity = 18;
	param.merge = MMax;
	pattern(param);

	param = patternPDefault;
	param.shape = Single;
	param.merge = MOver;
	param.pause = 30000;
	param.size = 15000;
	param.intensity = 60;
	param.position = 65535;
	
	setSeed(micros()%65536);
	for (uint8_t i = 0; i < 4; i++){
		if(patternInitial() || patternPauses()>0){
			patternRandomize();
			patternClearPause();
		}
		param.baseColor = getLedColor(0,0,patternRandoms()/4,patternRandoms()/2+10);
		param.pausestart = i*5000+100;
		param.speed = (patternRandoms()*2+600-(patternPositions()>>8))*-1;
		param.strip = i+1;
		pattern(param);
	}
}


int32_t speed=0;
void patternSeesaw(){
	fade(150);
	param = patternPDefault;
	param.baseColor = getLedColor(0,255,100,0); 
	param.shape = Single;
	param.merge = MMax;
	param.intensity = 255;
	int32_t distance = ((int32_t)patternPositions()-32768)>>7;
	speed += distance*-1;
	speed = max(0,abs(speed)-max(4,abs((speed+100)/250))+(int32_t)getTriggerSensorValue()/2) *sgn(speed);
//	Serial.print(speed);
//	Serial.print(" - ");
	param.speed = speed>>1;
//	Serial.println(param.speed);
	pattern(param);
}

void patternLeftRightDouble(){
	fade(20);
	param = patternPDefault;
	param.baseColor = getLedColor(255,0,0,0); 
	param.shape = Single;
	param.merge = MMax;
	param.intensity = 255;
	param.speed = 500;
	param.size  = 15000;
	param.pause =  65535;  // Length of the pause
	pattern(param);
	param.baseColor = getLedColor(0,255,0,0); 
	param.speed = -500;
	param.position =65535;
	pattern(param);

	param.pausestart = 65535; // pause at the beginning

	param.baseColor = getLedColor(255,255,0,0); 
	param.speed = 500;
	pattern(param);

	param.speed = -500;
	param.baseColor = getLedColor(0,0,255,0); 
	pattern(param);
}

uint16_t size;
void patternstretchOut(){
	fade(30);
	size+=500;
	param = patternPDefault;
	param.baseColor = getLedColor(255,0,0,0); 
	param.shape = SawRight;
	param.merge = MMax;
	param.intensity = 255;
	param.speed = 0;
	param.size  = size;
	pattern(param);

	initializeCurrent();
	param.baseColor = getLedColor(0,0,255,0); 
	param.shape = SawLeft;
	param.position =65535-size;
	pattern(param);


	param.baseColor = getLedColor(0,255,0,0); 
	param.position =65536/2;
	param.shape = Sine;
	param.speed = -125;
	param.size  = size/2;
	pattern(param);

}


void patternCloudySky(){
	allOff();
	param = patternPDefault;
	param.baseColor = getLedColor(0,0,150,0); // Dark blue sky
	param.shape = AllOn;
	param.merge = MMax;
	param.intensity = 255;
	pattern(param);

	// Big cloud, litte cyan
	param.baseColor = getLedColor(100,200,200,0); 
	param.shape = Sine;
	param.size = 40000;
	param.merge = MMax;
	param.speed = 50;
	param.strip = 1;
	pattern(param);

	// Medium cloud, white
	param.baseColor = getLedColor(128,128,128,0);
	param.size = 30000;
	param.speed = -40;
	param.strip = 2;
	pattern(param); 

	// 2 smaller clouds with white Leds
	param.baseColor = getLedColor(0,0,0,150);
	param.size = 20000;
	param.speed = 90;
	param.strip = 1;
	pattern(param);
	param.size = 16000;
	param.speed = -70;
	param.strip = 2;
	pattern(param);

}

#define riseSpeed 300
uint32_t risen = 0;
uint8_t patternSunriseNewHelp1(uint32_t offset){
	param.size = min(risen-offset,65500);
	param.speed = -riseSpeed>>1;
	if(param.size>=65500){
		param.speed =0;
	}
	return min(param.size/100,200);
}
void patternSunriseNewHelp2(uint32_t offset){
	if(risen>offset){
		pattern(param);
	}	
}
void patternSunriseNew(){
	allOff();
//	if(initial[currentPosition]==true){
	if(patternInitial()){
		risen = 0;
	}
	param = patternPDefault;
	param.shape = Sine;
	param.merge = MMax;
	param.position=32768;
	risen +=riseSpeed;
	param.baseColor = getLedColor(patternSunriseNewHelp1(0),0,0,0);
	patternSunriseNewHelp2(0);
	if(risen>=120000){
		risen=120000;
	}
	param.baseColor = getLedColor(0,patternSunriseNewHelp1(15000),0,0);
	patternSunriseNewHelp2(15000);

	param.baseColor = getLedColor(0,0,patternSunriseNewHelp1(35000),0);
	patternSunriseNewHelp2(35000);

	param.shape = AllOn;
	if(risen>70000){
		uint8_t bright = ((risen-70000)*255)/(120000-70000)/2;
		param.baseColor = getLedColorGamma(0,0,0,bright);
		pattern(param);
	}
}

void patternRedWhite(){
	allOff();
	param = patternPDefault;
	param.shape = Sine;
	param.repeat = true;
	param.baseColor = getLedColor(255,0,0,0);
	param.merge = MMax;
	param.size = 65536*2/(numLeds/2+1);
	param.speed = 50;
	pattern(param);

	param.gamma = true; // Gamma of white is different from red
	param.baseColor = getLedColor(0,0,0,155);
	param.position = 65536*2/(numLeds-1);
	pattern(param);	
}

uint32_t callbackRainbow(uint8_t ledNumber,uint16_t relativePos){
	return getLedColorHSV(relativePos,255,255);
}

void patternRainbowCallback(){
	allOff();
	param = patternPDefault;
	param.shape = Callback;
	param.merge = MOver;
	param.callback = callbackRainbow;
	param.size = 65535;
	param.speed = 200;
	pattern(param);	
}

// Pattern examples
/*
void patternGenMulti(){
	allOff();
	pattern(getLedColor(255,255,0,0),100,Single  ,false,    0,0,0,MOver,0);
	// pattern(getLedColor(0,0,255,0), -300,SawRight,false,15000,0,0);
	// pattern(getLedColor(255,0,0,0),  200,SawLeft ,false, 5000,0,0);
}

void patternGenPause(){
	allOff();
	pattern(getLedColor(255,0,0,0), 1000,Single,false,0,65535,0,MOver,0);
	pattern(getLedColor(0,0,255,0),-1000,Single,false,0,65535,65535,MOver,0);
	par = patternPDefault;
	par.baseColor = getLedColor(0,255,0,0);
	par.shape = SawRight;
	par.size = 10000;
	par.speed = 500;
	pattern(par);
}
*/


void sunriseSingle(uint8_t bar,float h){

	h = max(h,0);
	float hsqrt = ( float(sqrt( h )));
	int red   = ( hsqrt * hsqrt );//* float(16.0) );
//	int downred = ( hsqrt * 16.0 );
	int green = ( h * hsqrt ) / 256 * 16.0;
	int blue  = ( h * h ) / 256.0;
//	int white = blue;
	int white = 0;
	if(h>130){
		red -= (h-130);
		green -= (h-130);
		blue -= (h-130);
	}

	setBar(bar,getLedColor(red,green,blue,white));
//	show();
}
void patternDrops(){
	fade(100);
	param = patternPDefault;
	param.shape = Single;
	param.merge = MMax;
	param.pause = 10000;
	param.intensity = 255;
	
	setSeed(micros()%65536);
	for (uint8_t i = 0; i < 4; i++){
		if(patternInitial() || patternPauses()>0){
			patternRandomize();
			patternClearPause();
		}
		if(patternRandoms()>127){
			param.baseColor = getLedColorHSV(uint16_t(patternRandoms())*64+256*96,255,255);
		}else{
			param.baseColor = getLedColorHSV(uint16_t(patternRandoms())*64,255,255);
		}
		param.pausestart = i*5000+100;
		param.speed = patternRandoms()+40+(patternPositions()>>7);
		pattern(param);	
	}
}
void patternFire(){
	fade(20);
	param = patternPDefault;
	param.shape = Single;
	param.merge = MMax;
	param.pause = 10000;
	param.intensity = 255;
	
	for (uint8_t i = 0; i < MAXPOSITIONS-1; i++){
		if(patternInitial() || patternPauses()>0){
			patternRandomize();
			patternClearPause();
		}
		int16_t random = min(patternRandoms(),220);
		random += getTriggerSensorValue()/2;
		param.baseColor = getLedColorHSV(min(random,260)*30,255,255);
		param.intensity = max(0,255-((int)(patternPositions()>>7)-((int)random/2)));
		param.pausestart = i*3000+100;
		param.speed = random+100+(patternPositions()>>4);
		param.strip = (i % 2) + 1;
		pattern(param);	
	}
}
