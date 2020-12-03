/*****************************************************************
 *  File : ESP32_React-JSX_Sample00.ino
 *  ESP32を使ったWEBサーバ
 *  2020/12/03 木 JST
 *  
 *  何をするもの？
 *  Hello Word!と現在時刻からのカウントの表示
 *  
 *  使用したもの：
 *  html,
 *  React(CDN参照),
 *  JSX(CDN参照),
 *  
 *  *************************************************************/


//#include <Arduino.h>
//#include <stdio.h>          // sprintf関数を使うため
#include <WiFi.h>           //Wi-Fiの使用に必要
#include <WebServer.h>      //Wi-Fiの使用に必要
#include <WiFiMulti.h>      //複数のアクセスポイント接続用
#include <ESPmDNS.h>        //mDNS設定用
//#include <Update.h>       //OTAWebUpdaterに必要
//#include <WiFiClient.h>   //OTAWebUpdaterに必要

WiFiMulti wifiMulti;        //複数のアクセスポイント接続時必要
/*  Arduino IEDでは 
 *   #define 非推奨　
 *   const 型 データ名 推奨のためコメントアウト
 * #define SPI_SPEED   115200  // SPI通信速度
 * ******************************************************************/
const int HTTP_PORT  = 80;
const int SPI_SPEED  = 115200;

// パスフレーズは平文ではなくハッシュ化したものを使用したかった

const char *host   = "esp32";         //http://esp32.local
const char *ssid_1 = "ssid1*"; 
const char *pass_1 = "pass1*";                   
const char *ssid_2 = "ssid2**"; 
const char *pass_2 = "pass2**";
     

/* Function Prototype */
void doInitialize();        //初期化
void connectToWifi();       //WiFi Connect
void handleRoot();          //ルートアクセス時の応答関数


/* html 記述には C++ 生の文字リテラル
 *        という定義方法を使用
const String データ名 = R"rawliteral(
　　　　～～　HTML文　～～
)rawliteral"; */
/**************************< html記述 StrReactSample >****************************/
const String StrReactSample = R"rawliteral(
<!DOCTYPE html>
<html lang="ja">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <meta http-equiv="X-UA-Compatible" content="ie=edge">
  <title>Reactの練習</title>
</head>
<body>
<div id="root"></div>
<script src="https://unpkg.com/react@16/umd/react.development.js" crossorigin></script>
<script src="https://unpkg.com/react-dom@16/umd/react-dom.development.js" crossorigin></script>
<!-- react.development.js     ->> react使用に必要 -->
<!-- react-dom.development.js ->> react使用に必要 -->
<script src="https://unpkg.com/babel-standalone@6/babel.min.js"></script>
<!-- babel.min.js             ->> babel使用に必要 -->
<script type="text/babel">
  function tick() {
  const root = document.getElementById('root');
  const element = (
    <div>
      <h1>Hello, world!</h1>
      <h2>It is {new Date().toLocaleTimeString()}.</h2>
    </div>
  );
  ReactDOM.render(element, root);
}
setInterval(tick, 1000);
</script>
</body>
</html>

)rawliteral";

WebServer server(HTTP_PORT);
/*****************************************************************************
 *                               Main Processing                             *
 *****************************************************************************/

void setup() {
  
  doInitialize();                     // 初期化
  connectToWifi();                    // WiFi Connect

  server.on("/", handleRoot);         // ルートアクセス時の応答関数を設定
//server.onNotFound(handleNotFound);  // 不正アクセス時の応答関数を設定
  server.begin();                     // ウェブサーバ開始
}

void loop() {
  server.handleClient();              // クライアントからの要求を処理する
  
}

/**************************< Initialize functions >****************************/
/* 初期化処理
 *  関数名: doInitialize
 *  引数  : なし
 *  戻り値: なし 
 */
void doInitialize(){
  Serial.begin(SPI_SPEED);
  delay(500);
  
}
/****************************< Connect functions >****************************/
/* Wi-Fi接続 & mDNS設定 処理
 *  関数名: connectToWifi
 *  引数  : なし
 *  戻り値: なし
 */
void connectToWifi(){
    wifiMulti.addAP(ssid_1, pass_1);
    wifiMulti.addAP(ssid_2, pass_2);
    Serial.println("Connecting Wifi...");
    
    // Wait for connection
    while (wifiMulti.run() != WL_CONNECTED) {
      delay(200);
      Serial.print(".");
    }
    Serial.println("");
    // モニターにローカルIPアドレスを表示する
    Serial.println("WiFi Connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    /* use mdns for host name resolution*/
    if (!MDNS.begin(host)) { //http://esp32.local
      Serial.println("Error setting up MDNS responder!");
      while (1) {
       delay(1000);
      }
    }
    Serial.println("mDNS responder started");
}

/**************************< handleReact functions >**************************/
/* 表示するhtmlの処理
 *  関数名: handleRoot
 *  引数  : なし
 *  戻り値: なし
 */
void handleRoot(){
  server.send(200, "text/html", StrReactSample);
  
}
