I2C-1 が使われている

    $ i2cdetect -l
    i2c-1   i2c             bcm2835 I2C adapter                     I2C adapte

I2C_1 のアドレスはこれで分かる。

    $ cat /sys/class/i2c-adapter/i2c-1/uevent 
    OF_NAME=i2c
    OF_FULLNAME=/soc/i2c@7e804000
    OF_COMPATIBLE_0=brcm,bcm2835-i2c
    OF_COMPATIBLE_N=1
    OF_ALIAS_0=i2c1

スレーブアドレスはこれで確認できる。

$ i2cdetect -y 1
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f
00:          -- -- -- -- -- -- -- -- -- -- -- -- -- 
10: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
20: -- -- -- -- -- -- -- -- -- -- 2a -- -- -- -- -- 
30: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
40: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
50: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
60: -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- 
70: -- -- -- -- -- -- -- --                         

しかし insmod しても prob が呼ばれてない。

    loading out-of-tree module taints kernel.
