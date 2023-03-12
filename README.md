## 環境作成方法

### 作成環境

* Windows 10 Home
* wsl:Ubuntu-18.04

### crosstool-ngのインストール方法

```
> mkdir -p ~/cross/rpi
> mkdir ~/cross/src
> cd ~/cross/src
> sudo git clone https://github.com/crosstool-ng/crosstool-ng
> sudo chown -R {user_name} crosstool-ng
> cd ~/cross/src/crosstool-ng
> ./bootstrap
> sudo ./configure --prefix=/home/{user_name}/cross/
> sudo make
> sudo make install
```

### automakeとautoconf

上記の環境作成時にautomakeとautoconfが最新でないため、エラーになってしまうことがある。
この場合は`apt-get`でインストール済の2つをアンインストールして、最新版を入れなおす。
具体的には以下のとおり
```
sudo apt-get remove autoconf
wget http://ftp.gnu.org/gnu/autoconf/autoconf-2.71.tar.gz
gunzip autoconf-2.71.tar.gz
tar xvf autoconf-2.71.tar
./configure
make
make install
```
```
sudo apt-get remove automake
wget https://ftp.gnu.org/gnu/automake/automake-1.16.5.tar.gz
gunzip automake-1.16.5.tar.gz
tar xvf automake-1.16.5.tar
./configure
make
make install
```