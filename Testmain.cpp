#include "UART.h"
#include <iostream>
using namespace std;

int main(){
	Uart Test("/dev/pts/4");
	Test.sendUart('x');
	int x;
	cin >> x;
	Test.readUart();
	return 0;
}
