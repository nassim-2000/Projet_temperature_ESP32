/*====================================================
*  Programme permettant de transmettre une
*  température d'une pièce mesurée à distance
*  à l'aide d'un ESP32 et d'un serveur WEB
*====================================================*/

/*bibliothèques requises*/
#include <WiFiClientSecure.h>   //bibliothèque permettant l'utilisation de clées cryptées
#include <PubSubClient.h>   //bibliothèque fournit un client pour effectuer des messages de publication / abonnement simples avec un serveur qui prend en charge le MQTT
#include <WiFi.h>   //Cette bibliothèque permet la connexion internet WIFI

const char* ssid = "SSID";    //SSID WIFI
const char* password =  "MDP";    //MDP WIFI

const char* mqttServer = "farmer.cloudmqtt.com";  //Lien du serveur dans lequel on va envoyer les données

const int mqttPort = 20384;   // Transfert de données sur port protocole SSL
const char* mqttUser = "rdfcnpov";    //Authentification nom d'utilisateur
const char* mqttPassword = "xtMd5KxCPbcO";    //Authentification mot de passe

String temp_str;

char temp [50];

WiFiClientSecure net;

PubSubClient client(net);

/*--------------CLE PUBLIQUE----------------------------------*/
const char* Public_key = \

"-----BEGIN CERTIFICATE-----\n" \

"MIIEPzCCAyegAwIBAgIJAMxFBV3guX8ZMA0GCSqGSIb3DQEBCwUAMIG1MQswCQYD\n" \

"VQQGEwJmcjEWMBQGA1UECAwNaWxlIGRlIGZyYW5jZTEVMBMGA1UEBwwMdmlsbGV0\n" \

"YW5ldXNlMRwwGgYDVQQKDBN1bml2ZXJzaXRlIHBhcmlzIDEzMRkwFwYDVQQLDBBp\n" \

"dXQgdmlsbGV0YW5ldXNlMRQwEgYDVQQDDAtzZXJ2ZXIgbXF0dDEoMCYGCSqGSIb3\n" \

"DQEJARYZbWlsbGVmZXVpbGxlOTg3QGdtYWlsLmNvbTAeFw0yMDAyMDIyMDA4NTJa\n" \

"Fw0zMDAxMzAyMDA4NTJaMIG1MQswCQYDVQQGEwJmcjEWMBQGA1UECAwNaWxlIGRl\n" \

"IGZyYW5jZTEVMBMGA1UEBwwMdmlsbGV0YW5ldXNlMRwwGgYDVQQKDBN1bml2ZXJz\n" \

"aXRlIHBhcmlzIDEzMRkwFwYDVQQLDBBpdXQgdmlsbGV0YW5ldXNlMRQwEgYDVQQD\n" \

"DAtzZXJ2ZXIgbXF0dDEoMCYGCSqGSIb3DQEJARYZbWlsbGVmZXVpbGxlOTg3QGdt\n" \

"YWlsLmNvbTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMcxGCcu2fk2\n" \

"M8H+D8Eo1vyRg13UDVXlOww54o1HBVEMsRhDOl2q5YajRrQzuPy9JeeITUiXsbbF\n" \

"cHN7qYS5JRCrpHz5va86rFYvGejt7JpBdfO+RofWDLY2cj4UHZSRvY/vLrEXzXzy\n" \

"VbAD7WYpWyUcbdCSbl/S//4HHb/efqEONMQ72IlAPgQbEJG8iEyz9gjPHjtrTPmG\n" \

"Qp/Bl0kuDYYj+Qb5EzaBKnCdCiWTbbcsWF78dxCr5wOkh0Y3gOM7FDTzzBQav1MO\n" \

"beKSG/jbpBxpXHR9Diobaj/6cfKbh9gLI6Fb7Se+sDOBuKRj7plokqAfUVvqWx3x\n" \

"KDQcYWD+kOcCAwEAAaNQME4wHQYDVR0OBBYEFN6q2ZLl3W4JNafkkAkCLQuozjCO\n" \

"MB8GA1UdIwQYMBaAFN6q2ZLl3W4JNafkkAkCLQuozjCOMAwGA1UdEwQFMAMBAf8w\n" \

"DQYJKoZIhvcNAQELBQADggEBAHI/l6T3oUCebie4UViqTJtwlOaF3sxdYTVCe4YH\n" \

"Cr5R65y2LH4A1h2WKGBsUDABfAFQMSVrIMpylWuUX1LF/lWoCQY4j04GXLPEjU7H\n" \

"cOTPXWtJWqsEvTAoBA8Z5DJqzeoXrWyNf3QYo81+4xdl/I85RHx4Dzsy7P/sA18L\n" \

"LXxCk2dxVHEnWtFnS6xY4coDvUfgf/ed1FlbNAoRWC9DrmYqMIZu15xDmVBwiO3Q\n" \

"3smo0PoP4sC8tMqCG5eqgveMNgw+4qsb/pb3L1Z5QsgyP4iAg14w9MLo0yzl0y+k\n" \

"sEox9TDYBNYYzhwAtRk7DQdaZnwy3Yd6Ue77eg8SyVzf+p4=\n" \

"-----END CERTIFICATE-----\n" ;


/*--------------CLE PRIVEE----------------------------------*/
const char* Private_key = \

"-----BEGIN PRIVATE KEY-----\n" \

"MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDHMRgnLtn5NjPB\n" \

"/g/BKNb8kYNd1A1V5TsMOeKNRwVRDLEYQzpdquWGo0a0M7j8vSXniE1Il7G2xXBz\n" \

"e6mEuSUQq6R8+b2vOqxWLxno7eyaQXXzvkaH1gy2NnI+FB2Ukb2P7y6xF8188lWw\n" \

"A+1mKVslHG3Qkm5f0v/+Bx2/3n6hDjTEO9iJQD4EGxCRvIhMs/YIzx47a0z5hkKf\n" \

"wZdJLg2GI/kG+RM2gSpwnQolk223LFhe/HcQq+cDpIdGN4DjOxQ088wUGr9TDm3i\n" \

"khv426QcaVx0fQ4qG2o/+nHym4fYCyOhW+0nvrAzgbikY+6ZaJKgH1Fb6lsd8Sg0\n" \

"HGFg/pDnAgMBAAECggEBAIrToK3ObYTPTgtqcriBKjUlhZUjcZfoB9thjJBCa2Yt\n" \

"nk/2qvsVXJsVuPVnBwq/GWfhC1wIvYdQ3CrgiSlDkJ2pwx6HQLib//ihI0HAcSKh\n" \

"dP/IIYgSmxFzqPOzOVqOqLtkS/qJSGJI04e1dQO61xdqVO8E34BRorD9bLKI82Bz\n" \

"NuiFw+1H6lEbX+n4c8d9MkhDIZI27eM8oN6JsbT7ABBRnwW8SLI1ihkrfKMv/fsI\n" \

"XEdeNCb6De4+w5VGHFVMLhhLcCbhYchQzUIW+v30ubFP4CG2T6kkdMBxMpV5LoY7\n" \

"Vs5x8xxPH3JbQ+ICYKNBoDeolcj+tHgvQ2QOcY8uZgECgYEA750+Do2HTpyPO+Qm\n" \

"FDIdo5P+j7QcyjMVUz0poNlkU5nNNj7Z9tkCo9v8kqc1+jUv7QrJN9k+pO2ygbCg\n" \

"a5+94L8EC8YbWxtffSAWuOefPfoapRnrgKjFRN+ilF9Q0dMHbn0XpgVsMsH7gLgO\n" \

"pI6GxUFSuyLRrp0FkhmtkaVKz6cCgYEA1NA0WpE77ipsU6DI11RmVgNBcorYlmbg\n" \

"05zpszuMXDdur3XZN8T3fuXuS2z8qa1O8lYXhIGe3aXeuz5TqSniSmoz8lSUhHf3\n" \

"RuvX4XtHQGoJ7WWEhQ5wi3Gpea9LUPtAeqQd42FZ7zXgCbX/8tMGMNuOHrjbtX/v\n" \

"cdscqPZIXMECgYEA117EWpxHVBdQfwqQV3K8RT1Of6fqgKvtrQW6lwMPND13wBw6\n" \

"Ga/wuYuPt9lnoN+RlkQtnG896UxlOECj0FiieJH4dwS3reoqp/LhuY0nCrP0oxVr\n" \

"IheQSFmibi5GHHLLqTnyuZ0CJZVKE+Zb8vjQlhgi3mVrhsFIMou2AMVLOt0CgYBl\n" \

"2wzjuBVuBWH5mL/nx95/FmnRr8DAI5dB0R1NipfAX0ubsRT5C03Fr7kg4pX/hBKA\n" \

"DFKPOhbWURhMmCUklwZWxYP9XRwEVwLEMAcQRZexrc74j5kgseuN4iXSNuMebFLN\n" \

"IBWqCaMoyalkHHbXOscAxWS9MY7+zK/G169c8xtpQQKBgFz72pGIqwnajtdb9a4v\n" \

"/Y12Pi92IJYQv4T0RYk1YgePVoE6sJDm2+nysjwveHTAzfxAADny8Oo1oMGvAkHz\n" \

"M9AE4ms7UABemO7ioI80l9wWT8Rc+1lh9PEWUzMmS0/bPE70F3RH7G10I1bvn76V\n" \

"XAwNEvsp+x3FZF0wTeBlw4QZ\n" \

"-----END PRIVATE KEY-----\n" ;

/*connection wifi*/
void setup() {
  Serial.begin(115200);   //vitesse de connexion
  WiFi.begin(ssid, password);   //Authentification avec nom du réseau+mot de passe
  /*boucle si connexion non établie*/
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");   //affichage du chargement WiFi
  }
  Serial.println("Connected to the WiFi network");    //affichage wifi connecté

  /*connexion au serveur*/
  client.setServer(mqttServer, mqttPort);
  net.setCertificate(Public_key);   //ajout de la clé PUBLIQUE
  net.setPrivateKey(Private_key);   //ajout de la clé privée

  /*boucle si client n'est pas connecté*/
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");    //affichage chargement connexion client
    /*si client connecté*/
    if (client.connect("ESP32Client", mqttUser, mqttPassword )) {
      Serial.println("connected");    //affichage message client connecté
    /*si client n'est pas connecté*/
    }else{
      Serial.print("failed with state ");   //affichage message d'échec de connexion
      Serial.print(client.state());
      delay(2000);
    }
  }
}
void loop() {
  int potValeur = 0;
  float volt=0;
  float degr=0;

  potValeur =analogRead(potPin);    //Lecture valeur du potentiomètre
  volt=(3.3/4095)*potValeur;    //conversion valeur résistance --> volt
  degr=(100/3.3)*volt;    //conversion valeur volt --> degré

  /*affichage sur le moniteur série de la résistance, de la tension et des degrés*/
  Serial.println("ValeurPot:");
  Serial.println(potValeur);
  Serial.println("Tension:");
  Serial.print(volt);
  Serial.print("V");
  Serial.println("\t");
  Serial.println("Degrès:");
  Serial.print(degr);
  Serial.print("°");
  Serial.println("\t");


  temp_str = String(degr); //conversion variable décimale --> chaine de caractère
  temp_str.toCharArray(temp, temp_str.length() + 1); //Ajout de la chaine dans un tableau
  client.publish("Température :",temp);   //envoi du tableau sur le serveur
  delay(5000);
  client.loop();
}
