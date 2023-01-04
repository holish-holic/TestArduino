#include "M5Atom.h"

/* M5Atomが起動またはリセットされた後
setUp()関数内のプログラムが実行され、この部分は一度だけ実行されます。
在M5Atom 启动或者复位后，即会开始执行setup()，函数中的序，该部分只会执行一次 */
void look(int x, int y);

void setup()
{
    //Serial.begin(115200);
    M5.begin(true, false, true); //シリアルポートバッファをクリアし、シリアルポートのボーレートを115200に設定し、LEDマトリックスを初期化する。 清空串口缓区,设置串口波特率为115200；初始化led矩阵
    M5.dis.fillpix(0x000000); // 0xff0000をLEDドットマトリクス全体に塗りつぶす。 将 0xff0000 填充至整个LED点阵
    //delay(50);

    look(2,2);
}

const int EYE_COLOR_BG    = 0x000000;
const int EYE_COLOR_MAIN  = 0x000000;
const int EYE_COLOR_SUB   = 0xffffff;

void look(int x, int y)
{
  //  一度塗りつぶす
  M5.dis.fillpix(EYE_COLOR_BG);
  
  //  視点中心の上の行
  M5.dis.drawpix(x-1, y-1, EYE_COLOR_SUB);
  M5.dis.drawpix(x, y-1, EYE_COLOR_SUB);
  M5.dis.drawpix(x+1, y-1, EYE_COLOR_SUB);

  //  視点中心の行
  M5.dis.drawpix(x-1, y, EYE_COLOR_SUB);
  M5.dis.drawpix(x, y, EYE_COLOR_MAIN);
  M5.dis.drawpix(x+1, y, EYE_COLOR_SUB);

  //  視点中心の下の行
  M5.dis.drawpix(x-1, y+1, EYE_COLOR_SUB);
  M5.dis.drawpix(x, y+1, EYE_COLOR_SUB);
  M5.dis.drawpix(x+1, y+1, EYE_COLOR_SUB);
  
}
/* setup()のプログラムが実行された後、loop()のプログラムが実行されます。
loop()関数は、プログラムが繰り返し実行される無限ループです
在setup()函数中的位序执行完后，会接着执行loop()函数中的位序执行。
loop()函数是一个死循，其中的序会不断的重复运行 */
void loop()
{
    Serial.println('a');
    look(1, 1);
    delay(1000);

    Serial.println("bbbbb");
    look(2, 2);
    delay(1000);

    Serial.println("ccccc");
    look(3, 1);
    delay(1000);

    Serial.println("dd");
    look(1, 4);
    delay(1000);
}
