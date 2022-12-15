#include "input.hpp"
#include "callbacks.hpp"
#include "system.hpp"
#include <gccore.h>
#include <math.h>

static unsigned int kBuffer = 0;
static unsigned int kDown = 0;
static unsigned int kDownPrev = 0;

int inp_Init()
{
	PAD_Init();

	return 0;
}

void inp_Exit()
{

}

static void _scanGamecube(u16 down)
{
	s8 stickDeadZone = 32;
	u8 triggerDeadZone = 128;

	if (PAD_StickY(0) > stickDeadZone  || (down & PAD_BUTTON_UP)) 	 kBuffer |= BTN_UP;
	if (PAD_StickY(0) < -stickDeadZone || (down & PAD_BUTTON_DOWN))  kBuffer |= BTN_DOWN;
	if (PAD_StickX(0) < -stickDeadZone || (down & PAD_BUTTON_LEFT))  kBuffer |= BTN_LEFT;
	if (PAD_StickX(0) > stickDeadZone  || (down & PAD_BUTTON_RIGHT)) kBuffer |= BTN_RIGHT;

	if (down & PAD_BUTTON_A) 	  kBuffer |= BTN_JUMP;
	if (down & PAD_BUTTON_B) 	  kBuffer |= BTN_ATTACK;
	if (down & PAD_BUTTON_X) 	  kBuffer |= BTN_WEAPON;
	if (down & PAD_BUTTON_START)  kBuffer |= BTN_SELECT;
	if (down & PAD_BUTTON_Y)	  kBuffer |= BTN_START;
	if (down & PAD_BUTTON_A) 	  kBuffer |= BTN_ACCEPT;
	if (down & PAD_BUTTON_B) 	  kBuffer |= BTN_DECLINE;
	if (down & PAD_TRIGGER_Z)	  kBuffer |= BTN_R;

	if (PAD_TriggerL(0) > triggerDeadZone || (down & PAD_TRIGGER_L)) kBuffer |= BTN_L;
	if (PAD_TriggerR(0) > triggerDeadZone || (down & PAD_TRIGGER_R)) kBuffer |= BTN_R;
}


void inp_Scan()
{
	PAD_ScanPads();
	u16 gDown = PAD_ButtonsHeld(0);

	//
	kBuffer = 0;

	_scanGamecube(gDown);

	//disable opposites
	if ( (kBuffer & BTN_UP) && (kBuffer & BTN_DOWN) )
		kBuffer ^= BTN_UP | BTN_DOWN;

	if ( (kBuffer & BTN_LEFT) && (kBuffer & BTN_RIGHT) )
		kBuffer ^= BTN_LEFT | BTN_RIGHT;
}

void inp_Update()
{
	kDownPrev = kDown;
	kDown = kBuffer;
}

bool inp_ButtonHeld(unsigned int btn)
{
	if (kDown & btn)
		return true;
	
	return false;
}

bool inp_ButtonPressed(unsigned int btn)
{
	return !(kDownPrev & btn) && (kDown & btn);
}

bool inp_ButtonReleased(unsigned int btn)
{
	return (kDownPrev & btn) && !(kDown & btn);
}
