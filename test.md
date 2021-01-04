P2P_Reward arg1 arg2 arg3
-- arg1 == 아규먼트의 타입(1=명령줄 아규먼트, 2=파일 아규먼트)
-- arg2 == Master Wallet Address
-- arg3 == Master Wallet Privatekey

reward Program Build 
g++ *.cpp *.h -o Reward -I/usr/local/include -I/usr/include/rapidjson -lcurl

rapidjson = /usr/include/rapidjson
curl = /usr/local/include/

WSL 설치 후 셋팅 작업
1. ssh 설치 및 셋팅

2. net-tools 설치

3. sudo apt-get install build-essential

4. rapidjson 설치(sudo apt-get install rapidjson-dev)
   Location = /usr/include/rapidjson

6. openssl 설치
   wget https://www.openssl.org/source/openssl-1.1.1i.tar.gz
   tar xvfz openssl-1.1.1i.tar.gz
   cd openssl-1.1.1i
   ./config
   sudo make -j10 && sudo make install

5. curl 설치
   wget https://curl.se/download/curl-7.74.0.tar.bz2
   tar xvf curl-7.74.0.tar.bz2
   cd curl-7.74.0
   ./configure --with-ssl=/usr/local/ssl --prefix=/usr/local/curl
   sudo make -j10 && sudo make -j10 install
   curl --version (https가 목록에 있는지 확인)

6. visual studio 에서 P2P_Reward_Linux 배포

7. 리눅스에서 P2P_Reward_Linux Build
   g++ *.cpp *.h -o P2P_Reward_Linux -I/usr/local/include -I/usr/include/rapidjson -lcurl