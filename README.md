# Arduino-Syaberueno

Jenkinsが喋れるように、コンソールから引数で文字列を与えてそれを喋るようにした！
  
ハードウェアはここを参考に作りました！  
http://blog-yama.a-quest.com/?eid=970151  

# 使い方

作ったハードウェアに、どうにかして喋らせたいコマンドを送信する必要があります

## シリアルでテキストを送る

nodeにserialportをnpmで入れておけば、Arduino-SyaberuenoをUSBで繋ぐのが楽です。  
環境変数 *SYABERUNO\_PATH* にデバイスのパスを設定してください。  

使い方の例  
 export SYABERUNO\_PATH=/dev/ttyXX  
 node driver.js konnitiwa  

環境変数を適切に設定したうえで、driver.jsを呼び出してください。

## HTTPのGETで送る

GETパラメータに対応しました。LANでなんとかつないで喋らせたい文字列をぶちこめば喋ってくれます。

