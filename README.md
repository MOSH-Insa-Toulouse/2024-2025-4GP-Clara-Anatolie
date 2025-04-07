# 2024-2025 : Capteur low-tech Graphite - Anatolie Blanc & Clara Jeandenans

## SOMMAIRE

- [SOMMAIRE](#sommaire)
- [Contexte](#contexte)
- [Livrables](#livrables)
- [Matériel nécessaire](#matériel-nécessaire)
- [Simulation électronique du capteur sous LTSpice](#simulation-électronique-du-capteur-sous-ltspice)
- [Design du PCB sous KiCad](#design-du-pcb-sous-kicad)
- [Réalisation du shield](#réalisation-du-shield)
- [Datasheet](#datasheet)
- [Contact](#contact)

## Contexte

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Dans le cadre de l'UF "Du capteur au banc de test" effectué en 4ème année au département du Génie Physique de l’INSA Toulouse, nous avons développé un dispositif permettant de mesurer la contrainte appliquée sur un capteur graphite. Ce dernier est basé sur l'article "Pencil Drawn Strain Gauges and Chemiresistors on Paper" (Cheng-Wei Lin*, Zhibo Zhao*, Jaemyung Kim & Jiaxing Huang). Il est simplement composé d'un bout de papier sur lequel on vient déposer du graphite à l'aide d'un crayon à papier. La couche de graphite est constituée de particules reliées entre elles. En déformant le papier, la disposition des particules varie. En fonction du sens de flexion du papier, la déformation peut être soit une traction soit une compression. Dans le cas d’une traction, les particules de graphique s’écartent, la résistance augmente. Dans le cas d’une compression, les particules se rapprochent et la résistance diminue. Ainsi donc, ce système granulaire subit une modification de résistance et de conductance selon la contrainte exercée. Grâce à cela, nous pouvons remonter à la déformation, telle une jauge de contrainte traditionnelle.
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pour pouvoir réaliser ce dispositif permettant la mesure de déformation à partir d'un capteur low-tech, il nous faut suivre différentes étapes : simulations électroniques, design du PCB, code arduino et réalisation de la datasheet.

## Livrables

- Un Shield PCB relié à une carte arduino UNO avec différents composants : un capteur graphite, un amplificateur transimpédance, un module bluetooth, un écran OLED, un flexsensor commercial, un potentiomètre digital, un encodeur rotatoire. Il peut également un servomotor, des boutons ou tout autres composants de notre choix.
- Un code arduino qui gère les différents composants cités précédemments (mesures de contraintes, échanges bluetooth et OLED, potentiomètre digital et boutons).
- Une application Android (sous MIT App Inventor) interfaçant le PCB et le code arduino.
- Une datasheet sur le capteur de contrainte.

Nous avons opté pour l'ajout d'un servomotor.

## Matériel nécessaire

Pour réaliser notre dispositif électronique, voici la liste des composants nécessaires :

- Résistances : 1 de 1 kΩ, 1 de 10kΩ, 2 de 100kΩ pour l'amplificateur transimpédance et 1 de 47kΩ pour le flex sensor.
- Arduino Uno et son câble d’alimentation
- Amplificateur opérationnel LTC1050 et son support
- Potentiomètre digital MCP41050 et son support
- Module Bluetooth HC05
- Ecran OLED 128x64
- Flexsensor
- Servomotor
- Encodeur rotatoire
- Capteur graphite et 2 pinces crocodiles
- 20 sockets
- 35 hearders.

## Simulation électronique du capteur sous LTSpice

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Notre capteur de graphite possède une résistance variable de l'ordre du gigaohm. Le courant alors généré lorsque l'on applique une tension de 5V aux bornes du capteur est très faible (de l'ordre de la dizaine de nanoampères). Ainsi, pour récupérer et pouvoir analyser ce signal, nous devons d’abord l'amplifier. C'est pourquoi nous avons utilisé un montage transimpédance, constitué d'un amplificateur opérationnel (AOP) pour fournir un signal en tension suffisant au convertisseur analogique-numérique (ADC) d'une carte Arduino UNO.
Nous avons testé ce montage sur le logiciel LTspice :

![montage_capteur_graphite](/Photos/Schema%20montage%20transimpedance.png)

Pour construire ce montage, nous nous sommes basées sur ce schéma :

![montage_transimpédance](/Photos/ampli%20transimpedance.png)

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Concernant le choix de notre AOP, celui-ci devait être capable d'accepter en entrée un très faible courant. Il doit également avoir un offset de tension très faible afin de ne pas fausser les valeurs de tension transmises à l'ADC, puis analysées. Ainsi, nous avons choisi l’AOP nommé LTC 1050.

A ce circuit amplificateur, nous avons ajouté trois filtres pour améliorer notre signal :

- à l'entrée, un filtre passe-bas passif (R5,C1) ayant une fréquence de coupure de 16 Hz. Il permet de filtrer les bruits en courant sur le signal d'entrée
- un deuxième filtre passe-bas ayant une fréquence de coupure de 1.6 Hz (R3,C4) couplé à l'AOP. Ce filtre actif permet de filtrer la composante du bruit à 50 Hz provenant du réseau électrique
- à la sortie de l'amplificateur, un dernier filtre (R6,C2) ayant une fréquence de coupure de 1.6 kHz et permettant de retirer le bruit créé au cours du traitement (bruits des alimentation, de l'horloge, etc...).

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;De plus, nous avons placé la capacité C3 de sorte à ce qu'elle filtre le bruit de l'alimentation. La résistance R5 en entrée, en plus de former avec la capacité C1 un filtre éliminant les bruits en tension, protège l'AOP contre les décharges électrostatiques. La résistance R1 sert de résistance de shunt entre la source de courant et la masse. La résistance R2 sera remplacé plus tard par un potentiomètre digital afin de régler le gain de l’AOP en fonction de nos besoins.

Pour savoir si nos filtres étaient efficients, nous avons testé notre circuit. Voici la réponse obtenue :

![Test_En_Transitoire](/Photos/verif1%20LTSPICE.png)

Le signal est amplifié à 1V donc l'arduino UNO pourra le mesurer.

Enfin, voici la réponse lorsque l'on simule un courant alternatif pour vérifier que le bruit est bien filtré.

![Test_En_AC](/Photos/verif2%20LTSPICE.png)

On remarque que le bruit est bien atténué d’environ 72dB à 50Hz. Le filtre est performant.

## Design du PCB sous KiCad

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Afin de réaliser notre PCB, nous avons reproduit le circuit précédent sur Kicad 9.0. Nous avons remplacé la résistance R2 par un potentiomètre numérique afin de pouvoir faire varier le gain de notre AOP. De plus, nous avons rajouté divers composants afin de pouvoir mesurer efficacement la valeur de notre capteur graphite et comparer les résultats obtenus :

- un flexsensor servant de témoin, afin de pouvoir comparer nos mesures avec celle du capteur graphite
- un module bluetooth HC-05 afin de pouvoir communiquer avec notre circuit depuis notre téléphone depuis une application mobile que avons codée nous-même
- un écran OLED ainsi qu’un encodeur rotatoire pour pouvoir visualiser le résultat de nos mesures et pouvoir naviguer simplement dans les différents menus servant à réaliser diverses mesures
- un servomotor.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Tous nos composants ont été installés sur un shield d'Arduino UNO.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Nous avons commencé par réaliser les symboles des différents composants et reproduire le schéma électrique complet sur Kicad. Voici le schéma électrique de l'ensemble de notre montage :

![Schema_elec_Kicad](/Photos/Schéma%20elec.PNG)

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Nous avons ensuite réalisé les empreintes de nos composants afin de les placer sur notre PCB. Notre difficulté principale a été de placer les composants de sorte qu'il n'y ait pas de via, notamment pour le GND. Une seule via est présente sur notre PCB. Voici le résultat final :

![PCB_Kicad](/Photos/PCB.PNG)

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Nous avons aussi fait en sorte que les composants ne se touchaient pas à l’aide du rendu 3D que voici :

![3D_PCB_Kicad](/Photos/vue%203D.PNG)

## Réalisation du shield

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Grâce à notre Kicad, nous avons pu éditer un masque de gravure de notre PCB. Nous avons envoyé notre projet Kicad à Cathy qui s'est chargée de le vérifier et l'imprimer. Elle a ensuite insolé aux UV une plaque d'Epoxy recouverte d'une fine couche de cuivre et d'une résine photosensible. Elle a plongé la plaquette insolée dans du révélateur afin de retirer la résine non insolée, puis dans du perchlorure de fer afin de graver les pistes. Enfin, elle a nettoyé notre plaquette avec de l'acétone afin de retirer la résine restante. Un grand merci à Cathy qui a pris le temps de tirer notre PCB et nous expliquer le processus de fabrication !

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Nous avons ensuite pu réaliser le perçage de notre plaquette et y souder nos divers composants selon le schéma du PCB obtenu sur Kicad.

![Perçage_du_PCB]()

![Soudage_du_PCB]()

Voici le résultat final :

![Plaquette_finale]()

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;L’assemblage de la plaquette et l’Arduino a été plus compliqué que prévu. En effet, l'ensemble de nos trous étaient légèrement décalés les uns par rapport aux autres. Le problème semblait venir de l’étape de perçage.

## Datasheet

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Vous pouvez retrouver la datasheet du produit `[ici]()`

## Contact

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pour toute information complémentaire, vous pouvez nous contacter à ces adresses mails :

- Anatolie Blanc : <anatolie.blanc@insa-toulouse.fr>
- Clara Jeandenans : <clara.jeandenans@insa-toulouse.fr>
