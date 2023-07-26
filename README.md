# Projet_Parking2023
Partie de la communication entre l'ESP maitre et ses ESP slave ainsi que l'envoie des données vers un broker.

## a)	Explication de la partie (objectif):

Le but de ma partie est de permettre à l' utilisateur de savoir si une place du parking est prise ou non via un capteur situé au-dessus de chaque place de parking.
De plus, une led située à côté du capteur permet de signaler si la place est prise ou non avec une led verte si elle est libre et rouge si elle est occupée.

●	Détecter état place parking et Indiquer état place parking: il faut élaborer  un prototype simple à configurer, simple à installer et consommant peu d’énergie. Ces systèmes récupéreront et transmettront les informations avec un courtier de type MQTT.

○	Capteur des places du parking. Chaque capteur est composé d’un ESP8266, d’un capteur à ultrason, d’une led RGB et d’une fiche modBus RS485.

○	Indication de l’état de la place. L’indication des places se fait via la led RGB se trouvant sur chacun des capteurs. Par exemple si la place d’un capteur est libre la led sera verte à contrario si la place est prise la led sera rouge.

●	Transmettre les données en modBus. La transmission des données de si oui ou non la place est prise se fait via le modBus RS485. Le RS485 à une distance de transmission de 1200 mètres avec une vitesse de 9600 baud.

 
Tableau de la distance par rapport à la vitesse pour le protocole modBus RS485:
Vitesse (baud)	distance (m)
1	1200
10	1200
100	1200
600	1200
1200	1200
1800	1200
2400	1200
4800	1200
9600	1200
10000	1200
19200	1200
28800	1200
38400	1200
57600	1200
76800	1200
83333	1200
84000	1190
100000	1000
115200	868

●	Configurer les matériels : cette tâche est importante, elle conditionne une installation et une maintenance aisée. On proposera une application qui communiquera avec les détecteurs et indicateurs du parking.

 
## b)	Présentation du matériel :
Les matériels utilisés dans le projet comprennent :
1. Deux ESP8266 : ils jouent les rôles du maître et de l'esclave dans le système.
2. Une liaison Modbus RS485 : utilisée pour envoyer les données des esclaves vers le maître.
3. Une Raspberry Pi4 : joue le rôle de serveur SQL et de broker.

## c)	Description des diférents modules :

### Capteur et indication d’une place:

La première tâche réalisée concerne le capteur qui permet de détecter la présence d'un véhicule sur la place. Voici un fonctionnement simplifié du capteur : 

Le capteur émet des ondes qui permettent de déterminer la présence ou l'absence d'un véhicule sur la place. Pour éviter de considérer une place comme occupée alors qu'il n'y a personne, on utilise la formule suivante qui détermine la distance maximale à laquelle les ondes sont émises, en prenant en compte la distance du sol.

De plus, l'indication de l'état de la place est réalisée par le biais de la LED RGB qui est positionnée juste au-dessus de la place. Cette LED est contrôlée par le capteur, en particulier en fonction de la distance à laquelle un objet est détecté ou non.

Elle est gérée au moyen d'une boucle qui utilise la distance mesurée par l'onde. Si la distance mesurée est inférieure à une valeur prédéfinie, la LED sera allumée en rouge. En revanche, si la distance reste constante, la LED sera allumée en vert.

On met en œuvre un système de mesure de distance utilisant un capteur ultrasonique et le contrôle d'illumination de LEDs en fonction de la distance mesurée. Voici un résumé des différentes parties du code :

Tout d'abord, les broches nécessaires sont définies, notamment celles des LEDs (rouge, verte et bleue), ainsi que celles du capteur ultrasonique (broche de déclenchement et broche de réception). Un seuil de distance est également déterminé pour la détermination de l'état des LEDs.

Ensuite, dans la fonction de configuration et d'initialisation (setup), la communication série est initialisée et les broches correspondant aux LEDs et au capteur ultrasonique sont configurées en sortie et en entrée respectivement.

La boucle principale (loop) est ensuite exécutée en continu. Dans cette boucle, la mesure de distance est réalisée en utilisant le capteur ultrasonique. Une impulsion de déclenchement est envoyée au capteur en modifiant l'état de la broche correspondante, puis la durée de l'impulsion envoyée par le capteur est mesurée à l'aide de la fonction pulseIn(). La distance à l'objet est ensuite calculée en fonction de cette durée.

Si la distance mesurée est inférieure au seuil défini, la LED rouge est allumée en modifiant l'état de la broche correspondante. Dans le cas contraire, c'est la LED verte qui est allumée. L'état de la LED est également affiché sur le port série pour un suivi visuel.


### Transmission des données (Esclave):

L'objectif est de créer un dispositif qui peut communiquer avec un maître Modbus et échanger des données via des registres.

L'inclusion des bibliothèques nécessaires, notamment "SoftwareSerial" pour la communication série logicielle et "ModbusRTUSlave" pour la gestion du protocole Modbus RTU.

Ensuite, les broches de communication sont définies, à savoir la broche de réception (rxPin), la broche d'émission (txPin) et la broche de validation du driver de lignes de transmission (dePin).

Les constantes telles que l'adresse de l'esclave Modbus (id), la vitesse de communication en baud (baud) et la taille du tampon de réception des trames Modbus (bufSize).

Des variables globales sont déclarées, notamment le tampon de réception (buf), le nombre de registres de maintien (numHoldingRegisters) et le nombre de bobines (numCoils).

Ensuite, des fonctions sont définies pour la lecture et l'écriture des registres de maintien, ainsi que pour la lecture et l'écriture des bobines.

La fonction de configuration et d'initialisation (setup) est utilisée pour configurer les broches, initialiser les objets de communication (SoftwareSerial) et configurer la bibliothèque ModbusRTUSlave en spécifiant le nombre de registres de maintien et en associant les fonctions de lecture et d'écriture appropriées.

Dans la boucle principale (loop), la fonction poll() de la bibliothèque ModbusRTUSlave est appelée pour gérer la communication Modbus. De plus, une mise à jour périodique des registres de maintien est effectuée, où la valeur d'un registre est incrémentée et modifiée toutes les 1000 millisecondes.


### Transmission des données (Maître):

La mise en place d'un maître Modbus RTU qui communique avec des esclaves Modbus RTU en utilisant les bibliothèques "ModbusMaster" et "SoftwareSerial". L'objectif est de permettre au maître de lire des données à partir des esclaves via une liaison RS485.

Ensuite, les broches de communication RS485 sont configurées, à savoir la broche de contrôle de direction (DE_PIN), la broche de transmission (TXD_PIN) et la broche de réception (RXD_PIN).

Une constante est définie pour spécifier la vitesse de communication série (SERIAL_BAUD), puis une variable (result) est utilisée pour stocker le résultat des opérations Modbus.

Une instance de ModbusMaster (node) est créée pour gérer la communication Modbus, tandis qu'une instance de SoftwareSerial (mySerial) est utilisée pour la communication série via les broches RXD_PIN et TXD_PIN.

Des fonctions de rappel sont définies pour la transmission Modbus. La fonction preTransmission est exécutée avant la transmission pour activer la broche de contrôle de direction (DE_PIN), et la fonction postTransmission est exécutée après la transmission pour désactiver cette broche.

Dans la fonction de configuration et d'initialisation (setup), la communication série est initialisée pour la liaison RS485, la broche DE_PIN est configurée en sortie et la transmission est désactivée. Les fonctions de rappel sont attachées à l'instance ModbusMaster, puis la communication Modbus est démarrée en spécifiant l'adresse de l'esclave et l'objet SoftwareSerial.

Dans la boucle principale (loop), le maître effectue une lecture des données à partir de l'esclave Modbus. Le registre de maintien d'adresse 0 est lu à l'aide de la méthode readHoldingRegisters(). Le résultat est vérifié et affiché sur le port série. Si la lecture est réussie, la valeur du registre est affichée en hexadécimal. En cas d'erreur, le code d'erreur est affiché en hexadécimal.


### Envoie des données au broker:

La communication MQTT (Message Queuing Telemetry Transport) utilisera l'aide de la bibliothèques et "ModbusCommunication.h". Voici un résumé des différentes parties du code :

La classe CPub est conçue pour simplifier l'utilisation de la communication MQTT. Elle est dotée d'un constructeur par défaut qui initialise un objet mqttClient de la classe PubSubClient, utilisant un objet wifiClient de la classe WiFiClient.

La fonction de configuration et d'initialisation, appelée "setup", permet de paramétrer la connexion Wi-Fi en fournissant les informations relatives au réseau (SSID et mot de passe) ainsi que l'adresse du serveur MQTT. La connexion Wi-Fi est établie en utilisant la fonction WiFi.begin(), et une boucle while est mise en place pour attendre que la connexion soit établie avec succès en vérifiant l'état de la connexion via WiFi.status(). Ensuite, la fonction mqttClient.setServer() est utilisée pour définir l'adresse du serveur MQTT et le port de communication.

La boucle principale, nommée "loop", est exécutée en continu. Si la connexion MQTT n'est pas établie (mqttClient.connected() renvoie false), une tentative de connexion est effectuée en utilisant mqttClient.connect(). Si la connexion réussit, un abonnement au sujet "TestModBus" est réalisé via mqttClient.subscribe(). Enfin, la fonction mqttClient.loop() est appelée pour maintenir la communication avec le serveur MQTT.

La classe CPub offre également une fonction de publication de messages, appelée "publish", qui permet d'envoyer un message à un sujet MQTT spécifié. Cette fonction utilise mqttClient.publish() pour transmettre le message.
