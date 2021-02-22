# pacman
Pacman 3D Computer Grafik Abschlussarbeit
    Frederik Lau, 813538
    Marvin Frede, 823083


## GAME 
Target of the game is to collect all points which are spread on the map!
You lose if you get caught by one of the ghosts running around the map!

Ziel des Spiels ist es, alle Punkte zu sammeln, die auf der Karte verteilt sind!
Das Spiel ist verloren, wenn Pacman von einem der Geister erwischt wird, die auf der Karte herumlaufen!


## CONTROLS

- Spacebar			- Start / Pause / Start New Game
- W / Arrow-Up 		- Run forward
- A / Arrow-Left	  	- Turn left
- D / Arrow-Right  	- Turn right
- G 				- Switch between First- and Third-Person

## PRINTS

- Winning: *GAME OVER* WIN! ALL POINTS COLLECTED!
- Losing: *GAME OVER* LOSE! CATCHED BY GHOST!
- Switching to First Person: SWITCHED TO FIRST PERSON!
- Switching to Third Person: SWITCHED TO THIRD PERSON!
- Playing: PLAYING!
- Paused: PAUSE!


## REFERENZEN

Geist-Model
https://clara.io/view/6043d076-ef36-456c-b3bd-d1b8de19d105
    - den Vorstellungen entsprechender Geist wurde entnommen und einzeln als Body und als Extremitäten gespeichert

Pacman-Model
https://sketchfab.com/3d-models/pacman-17800bd2ae224ee281e43efcbc498e43
    - Das Model wurde als Body gespeichert und zusätzlich Augen modelliert

Portal-Textur
https://cartoongoodies.com/png_images/rick-and-morty-portal/

Pfeil-Model
https://clara.io/view/942deb6f-a75a-4d08-bf20-4bd8dad0bcbe

Wand-Textur
https://cc0textures.com/view?id=MetalPlates004

Recherche zu Partikeleffekt
https://levelup.gitconnected.com/how-to-create-instanced-particles-in-opengl-24cb089911e2


## BESONDERE FUNKTIONALITÄTEN

### AUFBAU
Der grundlegende Aufbau ist folgender:

 - Application
    - Game
        - Pacman (Gamecharacter)
        - Liste von Geistern (Gamecharacter)
        - Feld
            - Liste von Wänden (Wall)
            - Liste von Punkten (Point)
            - Liste von Portalen (Portal)
    
### GAME

    - Das Game hält die gesamte Spiellogik fest
    - Das Game entscheidet über den Gamemode, welcher aktuell aus FirstPerson, ThirdPerson und Debug (für die Entwickler) besteht
        - Im FirstPerson Modus wird Pacman nicht gerendert, sondern in Egoperspektive durch die Spielwelt navigiert, dabei wird die Kamera an die Stelle von Pacman gesetzt
        - Der Spieler hat im FirstPerson Modus eine weiße Taschenlampe, die sich vom gelben Pacman in der ThirdPerson unterscheidet
        - Zur Hilfestellung des orientierungslosen FirstPerson Spielers schwebt ein Pfeil im oberen Bildschirmrand, der auf den am nahegelegensten Punkt zeigt
        - Im FirstPerson Modus sind die Wände höher, um dem Spiel aus Egoperspektive einen stärkeren Charakter zu verleihen
        - Außerdem sind die Geister leicht höhergelegen, da sie schweben und vermitteln so einen fürchtenden Eindruck
        - Im ThirdPerson Modus sind die Wände kleiner, um die Punkte sichtbarer zu machen und dem Spieler mehr Orientierung zu verleihen
        - Wiederum fehlt der Pfeil zur Navigation
        - Pacman wird im ThirdPerson Mode mit einem eigenen Model gerendert und leuchtet gelb
        - Der Debugmodus ist wie der ThirdPerson Modus mit freier Kamerasteuerung, zusätzlich wird der Pfeil groß über Pacman gerendert
    - Das Game prüft nach Tastatureingaben und entscheidet so über Änderung der Spielmodi
    - Bei Eingaben, die die Bewegung von Pacman beeinflussen, wird diese Prüfung an Pacman delegiert
    - Das Game prüft außerdem den aktuellen Spielstatus, ob der Spieler gewonnen, verloren oder pausiert hat
        - Das Game selbst prüft, ob Pacman mit einem Geist kollidiert, delegiert die Prüfung der Punkte jedoch an das Feld (aufgrund dessen breiterem Wissen)

### FELD
Das Feld trägt die Verantwortung für Spielfläche, Wände und Punkte

    - Die Spielfläche ist in einem Schachbrettmuster mit den Maßen 30x33 angelegt, sodass Punkten und Wänden genaue Positionen vermitteln kann, die sie auf dem Spielfeld haben
    - Der Aufbau des Spielfelds ist dem original Pacman-Spiel nachempfunden. Punkte und Wände befinden sich an den exakt gleichen Positionen.
    - Da die Wände als zusammenhängende TriangleBoxModels umgesetzt wurden und verschiedene Größen annehmen können, gibt es nicht ein TriangleBoxModel pro Feld, das eine Wand ist
    - TriangleBoxModel Wände strecken sich über mehrere Felder
    - Aufgrund verschieden großer Wände wurden die Texturkoordinaten der TriangleBoxModel so angepasst, dass sich die Textur wiederholt und nicht streckt
    - Der Spielflächenaufbau erfolgt in folgender Reihenfolge:
        - zunächst wird eine Map mit Wänden erstellt, gemappt wird die x,z-Position der oberen linken Ecke der Wand auf dem Feld (0-30/0-33) (Key) zu den Ausmaßen der Wand (Value)
        - Die Ausmaße sind length und depth
        - dann wird die Map iteriert und für jede Position eine Wand als TriangleBoxModel mit den gegebenen Ausmaßen erstellt
        - Der Konstruktor der Klasse Wall sorgt dann dafür, dass sich die Wand mit gegebenen Spielfeldkoordinaten die korrekte globale Position gibt
        - als nächstes werden alle Felder auf denen keine Wand steht mit Punkten aufgefüllt, dabei gibt es beim original Pacman-Spiel jedoch Felder auf denen es keine Punkte gibt (diese werden zusätzlich berücksichtigt)
        - So kann man jederzeit beliebige Wände erstellen, bzw. den Spielfeld Aufbau verändern, ohne das Spielfeldprinzip zu verändern
    - Das Enum FieldType wird dazu genutzt, dass das Feld zurückgeben kann, welchen Feldtyp unter welcher x,z-Koordinate zu erwarten ist.
    - Die Feldtypen sind
        - OutOfField
        - Point (wird zu Free nach aufsammeln)
        - Wall
        - Free
        - Portal

### MODEL

    - ein angelegtes Model muss nicht von der Oberklasse an die korrekte Stelle geschoben werden, sondern kann sich anhand der x,z-Koordinate an der richtigen Stelle erzeugen
    - Die Oberklassen der Models übernehmen auch das Update der Modelle als Unterklassen
    - Die Models erzeugen alle Sub-Models und Lichter selbst

### NORMAL MAPPING

    - Dem TriangleBoxModel wurden Tangenten und Bitangenten hinzugefügt, um Normalmapping zu ermöglichen

### GAMECHARACTER

    - Die Geister und Pacman erben von der Klasse Gamecharacter und teilen sich somit vielseitige Eigenschaften
    - Den Gamecharacter zeichnet aus:
        - Er bewegt sich auf dem Feld
        - Er hat eine Position (Matrix transform) und eine Position auf dem Feld (Konvertierung der globalen Position zu Position auf dem Feld)
        - potentielle Punkt- und Spotlichtquelle
        - potentielle Models aus denen er sich zusammensetzt
        - Er kennt seine Ausrichtung (Orientation) in eine der vier Himmelsrichtungen (NORD = Blickrichtung entlang z-Achse in positiver Richtung)

#### BEWEGUNGSABLAUF
Schon zu Beginn der Programmierung hatteb wir eine feste Vorstellung zum Bewegungsablauf

    - Der Gamecharacter besitzt eine Aktionspipeline
        - Es wird festgehalten, ob der Gamecharacter noch eine Rotation oder eine Bewegung oder beides auszuführen hat, dabei wird die Rotation bevorzugt
    - Der Gamecharacter nimmt keine neuen Aktionen an, bis die erwarteten Aktionen ausgeführt wurden.
    - Eine Fortbewegungsaktion ist die Bewegung, um genau ein Feld auf dem Spielfeld nach vorne
    - Eine Rotationsaktion ist eine Rotation, um die eigene Achse, um genau 90°
    - Pacman kann aufgrund der Eingaben nur eine Aktion festhalten und auszuführen, werden Fortbewegung und Rotation vom Nutzer eingegeben, wird die Rotation bevorzugt und die Alternative ignoriert
    - Die Geister können Fortbewegung und Rotation gleichzeitig festlegen und so hintereinander ausführen, dadurch wird die KI unterstützt, um sinnvoll um Kurven zu gehen und eine flüssigere Fortbewegung der Geister zu ermöglichen
    - Pacman kann nicht rückwärts laufen oder sich mit einem Klick um 180° drehen, dies ist mit der Horrorthematik des Spiels verbunden
    - Pacman läuft ein wenig schneller als die Geister und rotiert deutlich schneller als die Geister (aber einen Tick langsamer als dem Spieler in manchen Momenten lieb ist)

### PARTIKEL
Das Einsammeln der Punkte soll Partikel emittieren

    - Ein umfassender Partikelemitter im Feld managed Ort und Bewegung der Partikel, die vom Spielfeld emittiert werden sollen
    - Der Emitter geht ressorucensparend vor, indem eine vordefinierte Liste an Partikeln gehalten wird
    - Einzelne Partikel werden bei Bedarf aktiviert und nach Ablauf deaktiviert, anstatt regelmäßig neu initialisiert zu werden
    - Nachteil ist, dass nur eine begrenzte Menge an Partikeln zur Verfügung steht
    - Vorteil ist, dass dadurch nicht dynamisch Partikel erzeugt werden müssen, sondern statisch zur Verfügung stehen
    - Dadurch, dass der Emitter dem Feld zur Verfügung steht kann dieser leicht bedient werden
    - Das Feld muss dem Emitter nur den Ort des zuletzt aufgesammelten Punktes übergeben
    - Jedes erzeugte Partikel erstellt sich dann selbst einen zufällig generierten Richtungsvektor, in den er translatiert wird
    - Außerdem besitzt ein Partikel eine zufällige Lebenszeit (restriktiert), sowie den boolean ob es am Leben ist und eine zufällige Rotation
    - Die Partikel sind je nach aufgerufenen Konstruktor entweder eine Partikeltextur auf einem LinePlaneModel oder ein nach einem bestimmten Farbwert zufällig gefärbtes LinePlaneModel