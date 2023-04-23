
#include "M5Atom.h "//をインクルードします。
/* M5Atomが起動またはリセットされた後
setUp()関数内のプログラムが実行され、この部分は一度だけ実行されます。
在M5Atom 启动或者复位后，即会开始执行setup()，函数中的序，该部分只会执行一次 */
void setup(){
    M5.begin(true, false, true); //シリアルポートバッファをクリアし、シリアルポートのボーレートを115200に設定し、LEDマトリックスを初期化する。 清空串口缓区,设置串口波特率为115200；初始化led矩阵
    delay(50);
    M5.dis.fillpix(0xff0000); // 0xff0000をLEDドットマトリクス全体に塗りつぶす。 将 0xff0000 填充至整个LED点阵
}
/* setup()のプログラムが実行された後、loop()のプログラムが実行されます。
loop()関数は、プログラムが繰り返し実行される無限ループです
在setup()函数中的位序执行完后，会接着执行loop()函数中的位序执行。
loop()函数是一个死循，其中的序会不断的重复运行 */
void loop(){
    delay(50);
    M5.dis.fillpix(0xff0000);

    delay(50);
    M5.dis.fillpix(0x00ff00);

    delay(50);
    M5.dis.fillpix(0x0000ff);
}
