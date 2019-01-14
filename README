# bme680_i2c

Raspberry Pi 3 Model BでBME680を利用するためのサンプルコードです。

# 接続方法
BME680をI2Cバス1のアドレス0x77で認識できる状態にする。

# 利用方法

```
// 本リポジトリをクローンする。
git clone https://github.com/nyoki/bme680_i2c.git
cd bme680_i2c

// 公式ドライバ(https://github.com/BoschSensortec/BME680_driver)をクローンし、以下の3ファイルをsrcフォルダ以下にコピーする。
// bme680.c, bme680.h, bme680_defs.h
git clone https://github.com/BoschSensortec/BME680_driver.git
copy BME680_driver/{bme680.c,bme680.h,bme680_defs.h} src/

//コンパイルし、実行
gcc -c bme680*.c && gcc -o bme680_i2c bme680_i2c.o bme680_pi.o bme680.o
./bme680_i2c
```
