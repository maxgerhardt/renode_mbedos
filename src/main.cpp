#include <mbed.h>

/* PA2, PA3 on USART 2 */
Serial pc(USBTX,USBRX);
/* orange LED LD3 on PD13 */
DigitalOut led(LED1);

int main() {
        led = 1;
	pc.printf("Hello world!\n");
	while(true) {
		wait_ms(1000);
		//ThisThread::sleep_for(1000);
		pc.printf("Test\n");
		led = !led;
	}
	return 0;
}
