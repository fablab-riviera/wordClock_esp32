# wordClock

**Version pour le matériel esp32Wroom**

**Cette version est un exemple possible de wordClock**

Il peut être adapté librement pour le besoin de chacun

## Fonctions des principaux modules:
- Le module `wordClock.cpp` déterminine tout le contenu qui sera affiché par les leds
- Le module `webServerWC.cpp` est un utilitaire qui permet de changer les paramètres de l'affichage (couleur, luminosité, l'heure affichée)
- Le module `wifiConnect.cpp` est un utilitaire qui connecte la wordClock à un réseau wifi. Si aucune connexion wifi ne peut être ouverte au démarrage de la wordClock, la wordClock se met en mode Access Point (AP) et affiche sur les leds sa propre adresse IP.
- Le  module `timeSync.cpp` est un utilitaire en charge de synchroniser l'horloge avec l'heure Internet
- Le module `flashEeprom.cpp` est un utilitaire en charge de sauvegarder les paramètres dans eeprom du esp32 (réseau wifi, mot de passe wifi, luminosité et couleur)
- Le module `neoMatrix.cpp` est un utilitaire en charge de piloter les leds et de permettre de les adresser sous forme de coordonnées (x,y) 

## Les liens entre les modules
![Les modules de la wordClock](/doc/modulesWordClock.jpg)

## Capture d'écran de la page web du webserveur
![Capture d'écran de la page web du webserveur](/doc/ecranWebServeur.jpg)

