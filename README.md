
組み込みLinuxデバイスドライバの作り方 (1) - Qiita
https://qiita.com/take-iwiw/items/1fdd2e0faaaa868a2db2

# 1回目: ビルド環境準備と、簡単なカーネルモジュールの作成

最低限のモジュールの作り方

ソースコード test.c

    #include <linux/module.h>
    
    static int test_init(void)
    {
        printk("Hello my module\n");
        return 0;
    }
    
    static void test_exit(void)
    {
        printk("Bye bye my module\n");
    }
    
    module_init(test_init);
    module_exit(test_exit);

Makefile

    obj-m := test.o    # 最終的に test.c というソースファイルから test.ko が作られる
    
    all:
        make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
    clean:
        make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean

インストール

    insmod test.ko

削除

    rmmod test

# 3回目: システムコールハンドラとドライバの登録(動的な方法)

キャラクタデバイスに登録するときは cdev を作り登録する。

デバイスドライバに open, close, read, write で呼ばれる関数を定義し、それらの関数のアドレスを cdev のメンバ変数に割り当てる。

udev の仕組みを使うといちいちデバイスファイルを手で作る必要がない。


# 4回目: read/writeの実装とメモリのお話

* カーネルモジュールはカーネル空間にある
* ユーザー空間とカーネル空間の間でデータをやり取りするときは以下の関数を使う

    copy_to_user
    copy_from_user

# 5回目: ラズパイ用のGPIOデバドラの実装

* Raspberry Pi 3 B のレジスタの(CPUから見た)物理アドレスは 0x3F000000
* GPIO レジスタのアドレスは 0x3F000000 をベースにして存在する
* レジスタに 0 や 1 を書くと GPIO が設定できたり、読むこともできる

物理アドレスには直接さわれないので、レジスタを使うときは仮想アドレスへ変換してから使う

* ユーザー空間の場合は /dev/mem を open して使う。mmap してバッファに割り当てるとアクセスしやすい
* カーネル空間(モジュール)の場合は ioremap_nocache を使う


# 6回目: ioctlの実装

read, write 以外にも、任意のコマンドとパラメータを渡すことができる。

# 8回目: debugfs用インタフェース

モジュールのデバッグのときに使ったりする？

モジュールの賞味の API として read, write があるけどそれ以外の目的のために使う？

好き勝手に使っていいらしい。

https://www.kernel.org/doc/Documentation/filesystems/debugfs.txt

# 9回目: 他のカーネルモジュールの関数を呼ぶ / GPIO制御関数を使う

## 他のカーネルモジュールの関数を呼ぶ

カーネルモジュールはすべてカーネル空間にあるので、リンクせずに他モジュールの関数を呼べる。らしい。

* 呼ばれる側は関数を EXPORT_SYMBOL(関数名) しておく
* 呼ぶ側は普通に関数を呼ぶ(extern して呼べる)

呼ばれる側のモジュールを先に insmod しておくこと

## GPIO制御関数を使う

いくつか関数がある。ピン番号だけでアクセスできるので、物理アドレスや仮想アドレスを意識せずに済む。

GPIO とレジスタアドレスを誰が結びつけてるんだ？

https://www.kernel.org/doc/Documentation/gpio/gpio.txt

# 10回目: I2Cを使ったデバイスドライバを作る

I2C はバスの構造になっているので作法がある。

以下引用

|階層|役割|
|:---|:--|
|I2C Client Drivers |各I2C機器のデバイスドライバ、i2c-dev|
|i2c-core |I2C通信のメイン処理|
|i2c-algo-XXX |通信用アルゴリズム|
|I2C Adapters |I2Cそのものの制御をするところ。レジスタをいじったり、チップ依存の処理|
|I2C Bus |例えば、I2C_0、I2C_1|
|I2C Client |各I2C機器 (例えば|

デバイスドライバ開発者は I2C Client Drivers の部分を作ることとなる。

デバイスID の構造体を i2c のバスに登録。

    static struct i2c_device_id mydevice_i2c_idtable[] = {
        {"MyI2CDevice", 0},
        { }
    };
    MODULE_DEVICE_TABLE(i2c, mydevice_i2c_idtable);

デバイスドライバの構造体を定義し

    static struct i2c_driver mydevice_driver = {
        .driver = {
            .name   = DRIVER_NAME,
            .owner = THIS_MODULE,
        },
        .id_table       = mydevice_i2c_idtable,     // このデバイスドライバがサポートするI2Cデバイス
        .probe          = mydevice_i2c_probe,       // 対象とするI2Cデバイスが認識されたときに呼ばれる処理
        .remove         = mydevice_i2c_remove,      // 対象とするI2Cデバイスが取り外されたときに呼ばれる処理
    };

init のときに登録

    i2c_add_driver(&mydevice_driver);

exit のときに削除

     i2c_del_driver(&mydevice_driver);

# 11回目: デバイスツリーにI2Cデバイスを追加する

デバイスドライバが乱立してきたので整理するためにデバイスツリーという仕組みが作られた。

デバイスのパラメータをデバイスツリーに括りだすことができる。デバイスドライバに決め打ちせずに済む。

モジュールがロードされたときにパラメータとして渡される。

デバイスツリー(バイナリファイル)は /boot/ にある

デバイスツリーファイル(テキスト)を書いておき、コンパイルして、バイナリを /boot/ に設置する。

コンパイルにはデバイスツリー用コンパイラが必要なのでインストールする。

    sudo apt-get install device-tree-compiler

逆コンパイル

    dtc -I dtb -O dts /boot/bcm2709-rpi-2-b.dtb > dis_bcm2709-rpi-2-b.dts

コンパイル

    dtc -O dtb -o bcm2709-rpi-2-b.dtb dis_bcm2709-rpi-2-b.dts

出来た bdb を /boot/ へ設置して、reboot する。

起動するとたとえば /proc/device-tree/soc/i2c@7e804000/mydevice@18/ というディレクトリが出来ている

デバイスドライバ側ではデバイスツリー構造体を登録しておく

    static const struct of_device_id mydevice_of_match_table[] = {
        {.compatible = "mycompany,myoriginaldevice",},
        { },
    };
    MODULE_DEVICE_TABLE(of, mydevice_of_match_table);

これはいつもどおり。

    static struct i2c_device_id mydevice_i2c_idtable[] = {
        {"MyI2CDevice", 0},
        { }
    };
    MODULE_DEVICE_TABLE(i2c, mydevice_i2c_idtable);

insmod すると prob 関数が呼ばれる。らしいんだが、手元のカラーセンサーで試しても prob が呼ばれない。

# 12回目: 作成したデバイスドライバを起動時にロードする

.ko を設置して depmod する。


    sudo cp MyDeviceModule.ko /lib/modules/$(uname -r)/kernel/drivers/.
    sudo depmod -ae

設定ファイルに書いておくとシステム起動時にロードされる

    sudo bash -c 'echo MyDeviceModule >> /etc/modules-load.d/modules.conf'
