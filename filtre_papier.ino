
/********************************************************************************************************************************************************************************************************
****                                                                                          FapDan's v2.0                                                                                          ****
****                                                                                       écrit par Djbouns                                                                                         ****
****                                                   Gestion du FapDan's, filtre a papier réalisé en impression 3D avec les plans dessinés par Dan's                                               ****
****                                                                                           05/05/2019                                                                                            ****
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

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Déclarations $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
const byte relais = A0; // pin du relais
const byte ledRouge = 3; // pin de la led rouge
const byte ledVerte = 2; // pin de la led verte
const byte flotteur = 4; // pin du flotteur
const byte bouton = 5; // pin du bouton
const uint16_t delaisFlotteurHaut = 500; // delais avant mise en route du moteur
const uint16_t delaisMaxFlotteurHaut = 3000; // delais max avant de stoper moteur
uint32_t compteurFlotteur = 0; // pour stocker le compteur flotteur
boolean compteurFlotteurOn = false; // pour mettre on off le compteur flotteur
boolean enFonction = true; // pour mettre on off le moteur
const uint16_t delaisBouton = 3000; // delais dactivation du bouton
uint32_t compteurBouton = 0; // pour stocker le compteur bouton
boolean compteurBoutonOn = true; // pour mettre on off le compteur bouton

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
}

//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ Loop $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void loop() {
  if (digitalRead (bouton) == LOW) { // si bouton active
    DebugPrintLN("bouton appuyer"); // debug
    if (compteurBoutonOn == true) { // si compteur bouton off
      compteurBoutonOn = !compteurBoutonOn; // compteur bouton on
      compteurBouton = millis(); // stock le timer
      DebugPrintLN("démarre le compteur du bouton"); // debug
    }
    if (delaisBouton < (millis() - compteurBouton)) { // si bouton active plus longtemps que delaisBouton
      enFonction = true; // moteur on
      compteurBoutonOn = false; // compteur off
      digitalWrite(relais, HIGH); // relais on = moteur on
      DebugPrintLN("réactive le fonctionnement, LED verte et rouleau ON manuelement"); // debug
    }
  }
  else { // si bouton pas active
    compteurBoutonOn = true; // compteur off
    if (enFonction == true) { // si en fonction
      DebugPrintLN("systeme activé, LED verte"); // debug
      if (digitalRead (flotteur) == LOW) { // si flotteur haut
        DebugPrintLN("flotteur Haut"); // debug
        if (compteurFlotteurOn == false) { // si compteur off
          compteurFlotteurOn = !compteurFlotteurOn; // on met le compteur on
          compteurFlotteur = millis(); // stock le timer
          DebugPrintLN("démarre le compteur de flotteur haut"); // debug
        }
        if (delaisMaxFlotteurHaut < millis() - compteurFlotteur) { // si flotteur declencher plus longtemps que le delaismaxflotteurhaut
          digitalWrite(relais, LOW); // relais off
          digitalWrite(ledRouge, HIGH); // led rouge on
          digitalWrite(ledVerte, LOW); // led verte off
          enFonction = false; // on desactive
          DebugPrintLN("fin de rouleaux, on desactive et LED rouge"); // debug
        }
        else if (delaisFlotteurHaut < millis() - compteurFlotteur) { // si flotteur declencher plus longtemps que delaisflotteurhaut
          digitalWrite(relais, HIGH); // relais on = moteur on
          DebugPrintLN("le rouleau tourne"); // debug
        }
      }
      else { // sinon (flotteur bas)
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
#ifdef DEBUG // si debug actif
  delay(500); // delais pour lire debug plus facilement
#endif
}
