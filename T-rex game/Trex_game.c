/*
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */


#include <stdio.h>
#include "xparameters.h"
#include "platform.h"
#include "xil_printf.h"
#include "xgpio.h"
#include "myip.h"

int main()
{
	int status;
	XGpio btn;

	init_platform();

	print("Initializing Buttons & Switches\n");
	status = XGpio_Initialize(&btn, XPAR_AXI_GPIO_0_DEVICE_ID);
	if (status != XST_SUCCESS) { return XST_FAILURE; }
	// set the direction to input
	XGpio_SetDataDirection(&btn, 1, 0xFF); // Channel 1

	// Create a pointer to the base of the starting address of the buffer
	volatile uint32_t *objs = (volatile uint32_t*)(XPAR_MYIP_0_S00_AXI_BASEADDR);

	print("Starting...\n\r");

	uint8_t xdir = 0;
	uint16_t obj1x, obj2x, obj3x, obj4x, obj5x, obj6x, obj7x, obj8x = 0;
	uint16_t obj1y, obj2y, obj3y, obj4y, obj5y, obj6y, obj7y, obj8y = 0;
	uint32_t background = 0xFFFFFF;

	//trex1 location
	obj1x = 200;
	obj1y = 656;

	//trex2 location
	obj2x = 1300;
	obj2y = 325;

	//ölü
	obj3x = 1300;
	obj3y = 325;

	//kaktüs
	obj4x = 1200;
	obj4y = 656;

	//bulut
	obj5x = 1300;
	obj5y = 400;

	//kuþ
	obj6x = 1500;
	obj6y = 550;

	//kuþ2
	obj7x = 1300;
	obj7y = 325;

	//gameover
	obj8x = 1300;
	obj8y = 325;

	print("Hello World\n\r");
	int flag=0;
	int flag2=0;
	int flag3=0;
	int flag4=0;
	int fall=0;
	while(1) {

		u32 b = XGpio_DiscreteRead(&btn, 1);

		//zýplama
		if (1 == b && flag == 0 && 0==fall)
		{
			flag=1;
		}

		if(1==flag){
			obj1y -= 1;

			if(400 > obj1y){
				flag=0;
				fall=1;
			}
		}

		if(1==fall && flag == 0){
			obj1y += 1;

			if(656 < obj1y){
				flag=0;
				fall=0;
			}
		}

		//start
		if (2 == b && 0 == flag3)
		{
			flag2=1;
		}

		if(1==flag2){

			if(obj5x >= 5)
			{
				obj5x-=1;
			}
			else if(obj5x < 5)
			{
				obj5x = 1290;
			}

			if(obj6x >= 5)
			{
				obj6x-=1;
			}
			else if(obj6x < 5)
			{
				obj6x = 1270 + rand()%800;
			}

			if(obj4x >= 5)
			{
				obj4x-=1;
			}
			else if(obj4x < 5)
			{
				obj4x = 1270 + (rand()%10000)/6;
			}
		}

		//kaktüs çarpýþma
			if (obj4x < 260 && obj4x > 150 && obj1y > obj4y - 64 ) {

			flag3 = 1;
			flag2 = 0;
			}

		//kuþ çarpýþma
			if (obj6x > 136 && obj6x < 264 &&  obj1y > 486 &&  obj1y < 600) {

			flag3 = 1;
			flag2 = 0;
			}

		//stop
			if (flag3 == 1){
			obj1x = 1300;
			obj3x = 200;
			obj3y = 656;
			obj8x = 600;
			obj8y = 325;
			objs[6] = objs[6];
			objs[4] = objs[4];
			}

		// reset
		if (8 == b) {
			flag4=1;
		}

		if (flag4==1){
			//trex1 location
			obj1x = 200;
			obj1y = 656;

			//trex2 location
			obj2x = 1300;
			obj2y = 325;

			//ölü
			obj3x = 1300;
			obj3y = 325;

			//kaktüs
			obj4x = 1200;
			obj4y = 656;

			//bulut
			obj5x = 1300;
			obj5y = 400;

			//kuþ
			obj6x = 1500;
			obj6y = 550;

			//kuþ2
			obj7x = 1300;
			obj7y = 325;

			//gameover
			obj8x = 1300;
			obj8y = 325;

			flag3=0;
			flag2=0;
			flag=0;
			flag4=0;
		}

		// update objects
		objs[0] = background;
		objs[1] = (obj1x << 16 | obj1y);
		objs[2] = (obj2x << 16 | obj2y);
		objs[3] = (obj3x << 16 | obj3y);
		objs[4] = (obj4x << 16 | obj4y);
		objs[5] = (obj5x << 16 | obj5y);
		objs[6] = (obj6x << 16 | obj6y);
		objs[7] = (obj7x << 16 | obj7y);
		objs[8] = (obj8x << 16 | obj8y);

		for(int i=0; i<160000;i++);
	}

	cleanup_platform();
	return 0;
}
