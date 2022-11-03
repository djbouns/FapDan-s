# FapDan-s V2

## Le filtre à papier 100% DIY pour l'aquariophilie

Bonjour,

Ce filtre a papier 3D a été dessiné par Dan's et j'ai écrit le programme de gestion du filtre.

### Pour la réalisation, il vous faudras :
Du PETG
1 alimentation
1 moteur
1 rouleau de papier
1 arduino nano
2 led + resistance 1k
1 relais/mosfet
1 bouton instantané

#### Fonctions :
> Temporisation du flotteur pour ne pas avoir de déclenchement du motoréducteur trop minime a répétition.  
> Led verte = système actif  
> Led rouge = système inactif  
> Le boitier active/désactive automatiquement le moteur en fonction du flotteur.  
> A la fin d'un rouleaux, cela génère des déclenchement répétitif du flotteur ou un trop long déclenchement, le boitier coupe le moteur / met le système inactif  
> Led verte allumé + flash de la led rouge = déclenchement répétitif survenu moins de 15 seconde après rotation du moteur ( le nombre de flash rouge correspondant au nombre de déclenchement intempestif consécutif, MAX 3 avant que le système soit mis inactif)  
> un appui sur le bouton supérieur a 1.5 seconde remet le système actif et met en marche le moteur. Lorsque vous relâcher le bouton, si le flotteur est bas, le moteur est coupé. Si le flotteur en haut, le moteur continue de tourner jusqu'au moment ou le flotteur soit en position basse.  

## Le tuto de montage est disponible ici :
http://www.recifal-france.fr/fabrication-du-fapdan-s-v2-filtre-a-papier-de-a-a-z-t37701.html

un GRAND MERCI a Dan's pour ce filtre 3D


## Vous êtes FAN de DIY ? ... 
# Venez découvrir l'AQUABOUNS, l'automate de gestion pour aquarium récifal
# www.aquabouns.fr

Téléchargement : https://github.com/djbouns

A bientôt
