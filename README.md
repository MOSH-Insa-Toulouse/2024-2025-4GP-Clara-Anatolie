# 2024-2025 : Projet de capteur low-tech en graphite - Anatolie Blanc & Clara Jeandenans

## SOMMAIRE

- [SOMMAIRE](#sommaire)
- [Contexte](#contexte)
- [Livrables](#livrables)
- [Matériel nécessaire](#matériel-nécessaire)
- [Simulation électronique du capteur sous LTSpice](#simulation-électronique-du-capteur-sous-ltspice)
- [Design du PCB sous KiCad](#design-du-pcb-sous-kicad)
- [Réalisation du shield](#réalisation-du-shield)
- [Code Arduino](#code-arduino)
- [Application Android](#application-android)
- [Datasheet](#datasheet)
- [Conclusions et analyse critique du projet](#conclusions-et-analyse-critique-du-projet)
- [Contacts](#contacts)

## Contexte

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Dans le cadre de l'UF "Du capteur au banc de test" effectué en 4ème année au département du Génie Physique de l’INSA Toulouse, nous avons développé un dispositif permettant de mesurer la contrainte appliquée sur un capteur graphite. Ce dernier est basé sur l'article "Pencil Drawn Strain Gauges and Chemiresistors on Paper" (Cheng-Wei Lin*, Zhibo Zhao*, Jaemyung Kim & Jiaxing Huang). Il est simplement composé d'un bout de papier sur lequel on vient déposer du graphite à l'aide d'un crayon à papier. La couche de graphite est constituée de particules reliées entre elles. En déformant le papier, la disposition des particules varie. En fonction du sens de flexion du papier, la déformation peut être soit une traction soit une compression. Dans le cas d’une traction, les particules de graphique s’écartent, la résistance augmente. Dans le cas d’une compression, les particules se rapprochent et la résistance diminue. Ainsi donc, ce système granulaire subit une modification de résistance et de conductance selon la contrainte exercée. Grâce à cela, nous pouvons remonter à la déformation, telle une jauge de contrainte traditionnelle.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Afin de réaliser ce dispositif permettant la mesure de déformation à partir d'un capteur low-tech, il nous faut suivre différentes étapes : simulations électroniques, design du PCB, code arduino et réalisation de la datasheet.

Ici, nous faisons un point de tout ce qui a été fait dans le cadre de ce projet.

## Livrables

Plusieurs livrables sont attendus :

- **Un shield PCB connecté à une carte arduino UNO** avec différents composants : un capteur graphite, un amplificateur transimpédance, un module bluetooth, un écran OLED, un flexsensor commercial, un potentiomètre digital, un encodeur rotatoire. D'autres composants peuvent être implémentés. Nous avons opté pour l'ajout d'un servomotor ;
- **Un code arduino** qui gère les différents composants cités précédemments (mesures de contraintes, échanges bluetooth et OLED, potentiomètre digital et boutons) ;
- **Une application Android** (sous MIT App Inventor) interfaçant le PCB et le code Arduino correspondant;
- **Une datasheet du capteur** reprenant toutes ses caractéristiques ainsi que ses tests.

## Matériel nécessaire

Afin de réaliser notre dispositif électronique, nous avons eu besoin de :

*Pour le montage amplificateur transimpédance :*

- Résistances : une de 1 kΩ, une de 10 kΩ et deux de 100 kΩ - une troisième de 100 kΩ peut être prévue, mais peut aussi être substituée par une résistance variable (le potentiomètre digital), choix que l'on a fait ;
- Potentiomètre digital MCP41050 et son support ;
- Amplificateur opérationnel LTC1050 et son support ;
- Capacités : trois de 100 nF et une de 1 μF.

*Pour le reste du dispositif :*

- Arduino Uno et son câble d’alimentation ;
- Résistance : une de 47kΩ pour le flex sensor ;
- Module Bluetooth HC05 ;
- Ecran OLED de dimension 128*64 ;
- Flexsensor ;
- Servomotor ;
- Encodeur rotatoire ;
- Capteur graphite et 2 pinces crocodiles ;
- 20 sockets ;
- 35 headers.

## Simulation électronique du capteur sous LTSpice

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Notre capteur de graphite possède une résistance variable de l'ordre du gigaohm. Le courant généré, lorsque l'on applique une tension de 5V aux bornes du capteur, est très faible (de l'ordre de la dizaine de nanoampères). Ainsi, pour récupérer et pouvoir analyser ce signal, nous devons d’abord l'amplifier. C'est pourquoi nous avons utilisé un montage transimpédance, constitué d'un amplificateur opérationnel (AOP) pour fournir un signal en tension suffisant au convertisseur analogique-numérique (ADC) d'une carte Arduino UNO.

***Description du montage amplificateur transimpédance***

Nous avons testé ce montage sur le logiciel LTspice :

![montage_capteur_graphite](/Photos/Schema%20montage%20transimpedance.png)

Pour construire ce montage, nous nous sommes basées sur ce schéma ci :

![montage_transimpédance](/Photos/ampli%20transimpedance.png)

Concernant le choix de notre AOP, celui-ci devait être capable d'accepter en entrée un très faible courant. Il doit également avoir un offset de tension très faible afin de ne pas fausser les valeurs de tension transmises à l'ADC, puis analysées. Ainsi, nous avons choisi l’AOP nommé LTC 1050.

A ce circuit amplificateur, nous avons ajouté trois filtres pour améliorer notre signal :

- à l'entrée, un filtre passe-bas passif (R5,C1) ayant une fréquence de coupure de 16 Hz. Il permet de filtrer les bruits en courant sur le signal sortant du capteur ;
- un deuxième filtre passe-bas ayant une fréquence de coupure de 1.6 Hz (R3,C4) couplé à l'AOP. Ce filtre actif permet de filtrer la composante du bruit à 50 Hz provenant du réseau électrique ;
- à la sortie de l'amplificateur, un dernier filtre (R6,C2) ayant une fréquence de coupure de 1.6 kHz et permettant de filtrer le bruit lié au traitement opéré par l'amplificateur (bruits des alimentation, de l'horloge, etc...).

De plus, nous avons placé la capacité C3 de sorte à ce qu'elle filtre le bruit de l'alimentation. La résistance R5 en entrée, en plus de former avec la capacité C1 un filtre éliminant les bruits en tension, protège l'AOP contre les décharges électrostatiques. La résistance R1 sert de résistance de shunt entre la source de courant et la masse. La résistance R2 sera remplacé plus tard par un potentiomètre digital afin de régler le gain de l’AOP en fonction de nos besoins.

***Simulations réalisées sous LTSpice***

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pour savoir si nos filtres étaient efficients, nous avons testé notre circuit le logiciel LTSpice, en deux temps.
D'abord, nous avons effectué une première simulation en régime transitoire pour vérifier que l'amplification soit bien effective. Voici la réponse obtenue :

![Test_En_Transitoire](/Photos/verif1%20LTSPICE.png)

Comme on peut le constater sur le graphe, le signal est bien amplifié à 1V. L'Arduino pourra donc l'interpréter.

Ensuite, nous avons effectué une seconde simulation, afin d'observer la réponse à un courant alternatif et ainsi de vérifier que le signal est bien filtré.

![Test_En_AC](/Photos/verif2%20LTSPICE.png)

Comme on peut le constater ci-dessous, le signal paraît être filtré successivement par les 3 filtres. Le bruit est atténué d’environ 72dB à 50Hz. Le montage est performant.

## Design du PCB sous KiCad

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Afin de réaliser notre PCB, nous avons reproduit le circuit précédent sur Kicad 9.0. Nous avons remplacé la résistance R2 par un potentiomètre numérique afin de pouvoir faire varier le gain de notre AOP. De plus, nous avons rajouté divers composants afin de pouvoir mesurer efficacement la valeur de notre capteur graphite et comparer les résultats obtenus :

- un flexsensor servant de témoin, afin de pouvoir comparer nos mesures avec celle du capteur graphite
- un module bluetooth HC-05 afin de pouvoir communiquer avec notre circuit depuis notre téléphone depuis une application mobile que avons codée nous-même
- un écran OLED ainsi qu’un encodeur rotatoire pour pouvoir visualiser le résultat de nos mesures et pouvoir naviguer simplement dans les différents menus servant à réaliser diverses mesures
- un servomotor.

Tous nos composants ont été installés sur un shield d'Arduino UNO.

Nous avons débuté par la création d'une librairie de symboles comportant tous les différents composants. Puis, nous avons procédé à la réalisation de la schématique électrique de l'ensemble du montage sur Kicad. Voici le schéma électrique de l'ensemble de notre montage :

![Schema_elec_Kicad](/Photos/Schéma%20elec.PNG)

Nous avons ensuite réalisé les empreintes de nos composants en respectant certaines dimensions, en vue de l'impression du PCB (diamètre des trous, forme des empreintes, etc.).Nous sommes ensuite passées à la disposition des composants sur le PCB : une étape complexe car il faut faire des compromis afin d'obtenir le placement le plus efficace (en considérant les connexions entre composants, en évitant d'avoir recours aux vias, etc.)... L'image ci-dessous correspond au résultat final de notre PCB :

![PCB_Kicad](/Photos/PCB.PNG)

Nous avons aussi fait en sorte que les composants ne se touchent pas à l’aide du rendu 3D que voici :

![3D_PCB_Kicad](/Photos/vue%203D.PNG)

Les fichiers KiCad sont disponibles dans le dossier [KiCad](https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-Clara-Anatolie/tree/3c948c7d110ff542ae939aa06793f8daed436087/KiCad/projet_capteur_lebon%20-%20VERSION%203)

## Réalisation du shield

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Une fois notre PCB réalisé sur Kicad, nous avons pu passer à la réalisation physique de notre PCB. Voici les étapes de fabrication réalisées, avec l'aide de Cathy Crouzet (un grand merci à elle qui a pris le temps de tirer notre PCB et nous expliquer le processus de fabrication !) :

Étape 0 (importante !) : vérification du PCB sous KiCad ;
Étape 1 : édition du masque de gravure de notre PCB sous KiCad ;
Étape 2 : insolation UV d'une plaquette d'époxy recouverte d'une couche de cuivre et de résine photosensible ;
Étape 3 : immersion de la plaquette dans un révélateur afin d'éliminer la résine non-insolée ;
Étape 4 : immersion de la plaquette dans du perchlorure de fer pour graver les pistes ;
Étape 5 : nettoyage de la plaquette avec de l'acétone pour retirer la résine restante.

Nous avons ensuite pu réaliser le perçage de notre plaquette et y souder nos divers composants selon le schéma du PCB obtenu sur Kicad.

![Perçage_du_PCB](/Photos/Perçage_PCB.jpg)

Voici le résultat final de notre PCB :

![Plaquette_finale](/Photos/Montage%20final.jpg)

L’assemblage de la plaquette et l’Arduino a été plus compliqué que prévu. En effet, l'ensemble de nos trous étaient légèrement décalés les uns par rapport aux autres. Le problème semblait venir de l’étape de perçage.

De plus, le header connecté à la pin A0, qui devait servir pour le capteur de graphite, ne fonctionne pas. Nous avons donc décidé d'utiliser la pin A1, qui servira donc à récupérer les données du flex sensor et du capteur de graphite. Il nous suffit alors de brancher/débrancher les capteurs pour les changer. Malheureusement, le signal provenant du capteur de graphite ne sera donc pas filtré.

## Code Arduino

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Nous avons utilisé l'IDE Arduino. Plusieurs fichiers de test sont disponibles dans le [dossier Arduino](https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-Clara-Anatolie/tree/main/Code%20Arduino/Codes_Arduino_Tests).

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Pour le [fichier principal](https://github.com/MOSH-Insa-Toulouse/2024-2025-4GP-Clara-Anatolie/tree/main/Code%20Arduino/Projet_capteur), nous avions prévu de faire une calibration du potentiomètre digital en fonction de la valeur mesurée par le capteur graphite. Ensuite, le menu de l'écran OLED propose 3 options:
- Utiliser le flex sensor
- Utiliser le capteur graphite
- Utiliser le Servomoteur

![Menu](/Photos/Menu.jpg)

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Le choix se fait grâce à l'encodeur rotatoire. En tournant, on peut passer sur les différentes options puis en sélectionner une en appuyant sur l'encodeur. Cet appui sert également à sortir du menu selectionné.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;L'option concernant le flex sensor récupère sa valeur et l'écrit sur l'OLED. L'option Graphite Sensor récupère sa valeur de résistance et l'affiche sur l'OLED (normalement). Enfin, la dernière option déplace le servomoteur en fonction de la valeur du flex sensor.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Le calcul de la résistance du capteur en graphite se fait avec : $$Res=R1*(1+\frac{R3}{R2})*\frac{Vcc}{Vadc}-R1-R5$$ \

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Et dans notre cas, notre résistance R2 est variable : c'est celle du potentiomètre digital qui est calculé avec : $`R2=\frac{47500*pos}{256}+125`$ où notre $47500$ correspond à la valeur de résistance maximale de notre MCP41050 et $pos$ la position du potentiomètre parmi les 256 valeurs.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Le module bluetooth HC-05 reçoit également les valeurs du capteur graphite et affiche un graphe.

## Application Android

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Une application a été développée sous [MIT App Inventor](https://appinventor.mit.edu/) .

## Datasheet

La datasheet de notre capteur est disponible `[ici]()`.

## Conclusions et analyse critique du projet

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Ce projet a été très enrichissant. En menant un projet de bout en bout, des simulations à la datasheet, nous avons pu avoir un avant goût du travail d'ingénieur. Nous avons aussi pu avoir une vision des nombreuses compétences (simulation électronique, développement informatique, design d'un montage électronique, fabrication physique de ce dernier, etc.) indispensables à un ingénieur de nos jours. En ce sens, c'est un projet qui a donc été assez professionnalisant. De  plus, il est gratifiant de pouvoir fabriquer un produit dans son entièreté et de le voir fonctionner.

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;En ce qui concerne le capteur graphite fabriqué, il est fonctionnel et les tests nous permettent de dégager des tendances cohérentes avec la physique. Cependant, les conditions de tests étant rudimentaires et peu reproductibles, cela conduit à tout de même obtenir des résultats assez aléatoires. Il est donc nécessaire de prendre du recul.

## Contacts

Pour toute information complémentaire, vous pouvez nous contacter à ces adresses mails :

- Anatolie Blanc : <anatolie.blanc@insa-toulouse.fr>
- Clara Jeandenans : <clara.jeandenans@insa-toulouse.fr>

![Signature](/Photos/Signature.PNG)
