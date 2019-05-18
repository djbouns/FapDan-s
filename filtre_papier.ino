
/********************************************************************************************************************************************************************************************************
****                                                                                          FapDan's v2.0                                                                                          ****
****                                                                                       écrit par Djbouns                                                                                         ****
****                                                   Gestion du FapDan's, filtre a papier réalisé en impression 3D avec les plans dessinés par Dan's                                               ****
****                                                                                           18/05/2019                                                                                            ****
********************************************************************************************************************************************************************************************************/

/* FapDan's, Gestion du filtre a papier réalisé en impression 3D avec les plans dessinés par Dan's
   Copyright (C) 2019 par dj bouns

   Ce programme est un logiciel libre: vous pouvez le redistribuer
   et/ou le modifier selon les termes de la "GNU General Public
   License", tels que publiés par la "Free Software Foundation"; soit
   la version 2 de cette licence ou (à votre choix) toute version
   ultérieure.

   Ce programme est distribué dans l'espoir qu'il sera utile, mais
   SANS AUCUNE GARANTIE, ni explicite ni implicite; sans même les
   garanties de commercialisation ou d'adaptation dans un but spécifique.

   Se référer à la "GNU General Public License" pour plus de détails.
   Vous la trouverez dans l'onglet GNU_General_Public_License.h

   Vous devriez avoir reçu une copie de la "GNU General Public License"
   en même temps que ce programme; sinon, écrivez a la "Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA".*/

#include <avr/wdt.h>

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Déclarations $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
const byte relais = A0; // pin du relais
const byte ledVerte = 2; // pin de la led verte
const byte ledRouge = 3; // pin de la led rouge
const byte flotteur = 4; // pin du flotteur
const byte bouton = 5; // pin du bouton
const uint16_t delaisFlotteurHaut = 500; // delais avant mise en route du moteur 1/2 sec
const uint16_t delaismoteurOn = 1000; // delais avant mise en route du moteur 1/2 sec
const uint16_t delaisMaxFlotteurHaut = delaisFlotteurHaut + 3500; // delais max avant de stoper moteur 3.5 sec + delais flotteur haut
uint32_t compteurFlotteur = 0; // pour stocker le compteur flotteur
boolean compteurFlotteurOn = false; // pour mettre on off le compteur flotteur
boolean enFonction = true; // pour mettre on off le moteur
const uint16_t delaisBouton = 1500; // delais dactivation du bouton 1.5 sec
const uint16_t delaisFinDeRlx = 15000; // delais durant le quel les declanchement son comptés , pour detécter la fin de rouleau 15 sec
const uint8_t declenchementMaxFinDeRlx = 3; // 3 declenchements
uint8_t declenchementRlx =  0; // pour stocker les declenchements
uint32_t compteurDeclenchement = 0; // pour stocker le compteur declenchement
uint32_t compteurBouton = 0; // pour stocker le compteur bouton
boolean compteurBoutonOn = false; // pour mettre on off le compteur bouton

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Debug $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//#define DEBUG // à commenter pour enlever le debug
#ifdef DEBUG
#define DebugPrintLN(x)  {Serial.println(x);}
#else
#define DebugPrintLN(x)
#endif

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Setup $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void setup() {
  Serial.begin(9600);
  pinMode(ledRouge, OUTPUT); // configure le pin en sortie
  pinMode(ledVerte, OUTPUT); // configure le pin en sortie
  pinMode(relais, OUTPUT); // configure le pin en sortie
  pinMode(flotteur, INPUT_PULLUP); // configure le pin en entre en mode pullup
  pinMode(bouton, INPUT_PULLUP); // configure le pin en entre en mode pullup
  digitalWrite(ledVerte, HIGH); // led verte on
  wdt_enable(WDTO_8S); // Si plus de remise a zero du compteur "reboot" (fait dans le loop) au bout de 8 secondes = bug de l'arduino = reboot automatique !
}

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Loop $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void loop() {
  if (digitalRead (bouton) == LOW) { // si bouton active
    DebugPrintLN("bouton appuyer"); // debug
    if (compteurBoutonOn == false) { // si compteur bouton off
      compteurBoutonOn = true; // compteur bouton on
      compteurBouton = millis(); // stock le timer
      DebugPrintLN("démarre le compteur du bouton"); // debug
    }
    if (delaisBouton < (millis() - compteurBouton)) { // si bouton active plus longtemps que delaisBouton
      enFonction = true; // moteur on
      digitalWrite(ledVerte, HIGH); // led verte on
      digitalWrite(ledRouge, LOW); // led rouge off
      digitalWrite(relais, HIGH); // relais on = moteur on
      declenchementRlx = 0; // remet le compteur de declenchement a zero
      compteurFlotteurOn = false; // remet le compteur flotteur en OFF
      DebugPrintLN("réactive le fonctionnement, LED verte et rouleau ON manuelement"); // debug
      while (digitalRead (flotteur) == LOW) { // boucle temps que le flotteur n'est pas bas
      }
    }
  }
  else { // si bouton pas active
    compteurBoutonOn = false; // compteur off
    if (enFonction == true) { // si en fonction
      DebugPrintLN("systeme activé, LED verte"); // debug
      if (digitalRead (flotteur) == LOW) { // si flotteur haut
        DebugPrintLN("flotteur Haut"); // debug
        if (compteurFlotteurOn == false) { // si compteur off
          compteurFlotteurOn = true; // on met le compteur on
          compteurFlotteur = millis(); // stock le timer
          DebugPrintLN("démarre le compteur de flotteur haut"); // debug
        }
        if (millis() - compteurDeclenchement < delaisFinDeRlx) { // si le flotteur est de nouveau en position haute pendant
          declenchementRlx++; // incremente le compteur de declenchement
          for (uint8_t cligonote = 0; cligonote < declenchementRlx; cligonote++) {
            digitalWrite(ledRouge, HIGH); // led rouge on
            delay(500);
            digitalWrite(ledRouge, LOW); // led rouge off
          }
        }
        else {
          declenchementRlx = 0; // remet le compteur de declenchement a zero
        }
        if (delaisMaxFlotteurHaut < millis() - compteurFlotteur || declenchementRlx >= declenchementMaxFinDeRlx) { // si flotteur declencher plus longtemps que le delaismaxflotteurhaut873
          digitalWrite(relais, LOW); // relais off
          digitalWrite(ledRouge, HIGH); // led rouge on
          digitalWrite(ledVerte, LOW); // led verte off
          enFonction = false; // on desactive
          DebugPrintLN("fin de rouleaux / trop de declenchement consecutif, SYSTEM DESACTIVE, LED rouge ON"); // debug
        }
        else if (delaisFlotteurHaut < millis() - compteurFlotteur ) { // si flotteur declencher plus longtemps que delaisflotteurhaut
          digitalWrite(relais, HIGH); // relais on = moteur on
          compteurDeclenchement = millis(); // stock le timer
          DebugPrintLN("le rouleau tourne"); // debug
        }
      }
      else { // sinon (flotteur bas)
        if (compteurFlotteurOn) { // si compteur est ON
          delay (delaismoteurOn); // delay pour que le moteur tourne encore unpeu apres la remise en bas du flotteur
        }
        digitalWrite(relais, LOW); // relais off
        digitalWrite(ledRouge, LOW); // led rouge off
        digitalWrite(ledVerte, HIGH); // led verte on
        compteurFlotteurOn = false; // compteur off
        DebugPrintLN("flotteur bas, rouleau Off et LED verte"); // debug
      }
    }
    else {
      digitalWrite(relais, LOW); // relais off
      digitalWrite(ledRouge, HIGH); // led rouge off
      DebugPrintLN("systeme desactivé, LED rouge"); // debug
    }
  }
  wdt_reset(); // indique que le loop est OK, pas de bug, remise a zero du compteur "reboot" du watchdog

#ifdef DEBUG // si debug actif
  delay(500); // delais pour lire debug plus facilement
#endif
}
