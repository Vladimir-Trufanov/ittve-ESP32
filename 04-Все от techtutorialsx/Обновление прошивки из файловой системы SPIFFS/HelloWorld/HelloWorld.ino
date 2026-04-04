// HelloWorld.ino

// "C:\Users\Евгеньевич\AppData\Local\Arduino15\packages\esp32\tools\esptool_py\5.1.0/esptool.exe" --chip esp32 --port "COM12" --baud 460800  --before default-reset --after hard-reset write-flash  -z --flash-mode keep --flash-freq keep --flash-size keep 0x1000 "C:\Users\Евгеньевич\AppData\Local\arduino\sketches\DF937A1419FDC5968171B16799C2EFFF/HelloWorld.ino.bootloader.bin" 0x8000 "C:\Users\Евгеньевич\AppData\Local\arduino\sketches\DF937A1419FDC5968171B16799C2EFFF/HelloWorld.ino.partitions.bin" 0xe000 "C:\Users\Евгеньевич\AppData\Local\Arduino15\packages\esp32\hardware\esp32\3.3.5/tools/partitions/boot_app0.bin" 0x10000 "C:\Users\Евгеньевич\AppData\Local\arduino\sketches\DF937A1419FDC5968171B16799C2EFFF/HelloWorld.ino.bin" 

// HelloWorld.ino.bootloader.bin   0x1000
// HelloWorld.ino.partitions.bin   0x8000 
// boot_app0.bin                   0xe000 
// HelloWorld.ino.bin              0x10000

void setup() 
{
  Serial.begin(115200);
}
 
void loop() 
{
  Serial.println("Hello и привет worldic");
  delay(1000);
}