I2C-1 が使われている

    $ i2cdetect -l
    i2c-1   i2c             bcm2835 I2C adapter                     I2C adapte

dts ファイルの aliases でアドレスが分かる。

    aliases {
      i2c0 = "/soc/i2c@7e205000";
      i2c1 = "/soc/i2c@7e804000";
      i2c2 = "/soc/i2c@7e805000";
    };

あるいはここにでも分かる( The Raspberry Pi, a temperature sensor and the device tree. https://bergzand.net/the-raspberry-pi-a-temperature-sensor-and-the-device-tree.html )

    $ cat /sys/class/i2c-adapter/i2c-1/uevent 
    OF_NAME=i2c
    OF_FULLNAME=/soc/i2c@7e804000
    OF_COMPATIBLE_0=brcm,bcm2835-i2c
    OF_COMPATIBLE_N=1
    OF_ALIAS_0=i2c1



しかし insmod しても prob が呼ばれてない。

    loading out-of-tree module taints kernel.
