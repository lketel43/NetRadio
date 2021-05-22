# NetRadio
Projet Réseaux S6

## Compilation

Pour compiler:
- Le projet en entier : `make`
- uniquement le code c : `make c`
- uniquement le code java : `make java`

Pour une compilation complète à neuf, il faut lancer `make clean all`

## Utilisation
### Diffuseur

* `java -jar Streamer.jar` permet de lancer un diffuseur par défaut avec les caractéristiques suivantes :
    * Id = STREAMER
    * Port de réception TCP = 4242
    * Adresse de multi diffusion = 225.0.0.0
    * Port de multi diffusion = 5001
    * Fréquence d'envoi des messages (en ms) = 1000
    * Ce diffuseur ne se connectera à aucun gestionnaire
    * Des messages par défaut sont diffusés
    * Des images par défaut sont diffusées
* `java -jar Streamer.jar ID PORT_TCP ADRESSE_MULTI PORT_MULTI FREQ`
   * Exemple : `java -jar Streamer.java myStream 4242 225.0.0.0 5001 5000`
   * Ce diffuseur ne se connectera à aucun gestionnaire
   * Des messages par défaut sont diffusés
* `java -jar Streamer.jar ID PORT_TCP ADRESSE_MULTI PORT_MULTI FREQ ADRESSE_GEST PORT_GEST`
   * Exemple : `java -jar Streamer.jar myStream 4242 225.0.0.0 5001 5000 localhost 4343`
   * Ce diffuseur se connecte au gestionnaire d'adresse `ADRESSE_GEST` tournant sur le port `PORT_GEST`
   * Des messages par défaut sont diffusés
* `java -jar Streamer.jar config messages`
   * `config` est un fichier de configuration de diffuseur. Son format doit être le suivant:
      * la première ligne contient l'id du diffuseur
      * la deuxième, son port de réception TCP
      * la troisième, l'addresse de multicast
      * la quatrième, le port de multicast
      * la cinquième, la fréquence d'envoi.
   * `messages` contient les messages que l'on souhaite diffuser.
      * La première ligne de ce fichier doit être l'id de l'auteur du message. Ensuite, chaque ligne contient un message.
* `java -jar Streamer.jar config messages manager`
   * `config` et `messages` sont comme précédemment.
   * `manager` est un fichier pour indiquer à quel gestionnaire le diffuseur doit se connecter
      * La première ligne du fichier contient l'adresse du gestionnaire
      * La deuxième contient son port
* `java -jar Streamer.jar config messages manager images`
   * `config`, `messages`, `manager` sont comme précédemment
   * `images` est un fichier contenant des chemins vers les images à diffuser
      * Un chemin par ligne


### Gestionnaire
Le gestionnaire se lance ainsi : `./manager [OPTION]... PORT [NB_DIFF]`
- `PORT` est le n° du port sur lequel le gestionnaire écoute les connexions entrantes
- `[OPTION]...` les options pour le gestionnaire
   - `-h` affiche l'aide
   - `-v` active le mode verbeux
   - `-d TEMPS` fais attendre le gestionnaire *TEMPS* secondes entre chaque envoi de *RUOK*; la valeur par défaut est 30 secondes.
   - `NB_DIFF` peut être précisé ou pas. S'il ne l'est pas le gestionnaire 
    gérera au plus 100 diffuseurs. S'il est précisé il gérera au plus NB_DIFF diffuseurs où NB_DIFF < 100.

Par exemple, la commande `./manager -v -d 5 5000 23` lance un gestionnaire en mode verbeux sur le port 5000, il testera la présence d'un diffuseur toutes les 5 secondes, et a au plus 23 diffuseur à gérer.  


### Clients
#### Client java
* `java -jar JavaClient.jar ADRESSE_MULTI PORT_MULTI ADRESSE_TCP PORT_TCP`
* `java -jar JavaClient.jar` lance un client par défaut. 
   * Équivalent à `java -jar JavaClient.jar 225.0.0.0 5001 localhost 4242` 

#### Client C 
On peut lancer le client C pour qu'il communique soit avec un gestionnaire, soit avec un diffuseur+multicast. Il n'y a pas de connexion par défaut. Le client doit connaître les adresses du diffuseur ou du gestionnaire avec lequel il veut communiquer.  
Communiquer avec un gestionnaire : ``./client adresse port`` où ``adresse`` et ``port`` sont ceux du gestionnaire en question.  
Communiquer avec un diffuseur et multidiffuseur : ``./client adresse_diff port_diff adresse_multicast port_multicast fichier`` où ``fichier`` est le fichier où sont affichés les messages diffusés par le multi-diffuseur. La bonne démarche est : Ouvrir un terminal, tapez ``tty`` et copier-coller le résultat affiché à la place de fichier. Ainsi les messages s'afficheront dans ce nouveau terminal.
 
 ## Architecture
 Dans `c/` :
 *  `manager.c` Gestionnaire, même fonctionnement qu'un serveur TCP threadé
 *  `message.c` Vérification et création des messages
 *  `message.h` Des constantes utiles pour les messages
 *  `client_keyboard.c` Client en C interactif
 *  `client.c` Manipulation d'un client via des fonctions. Utile lorsqu'on code un client "en dur"

Dans `java/` :
* `Streamer.java` qui est une classe représentant les diffuseurs
* `JavaClient.java` le client permettant d'envoyer des messages à un diffuseur et de recevoir les messages envoyés en multicast
* `JavaClientManager.java` le client permettant d'intéragir avec les gestionnaires
* D'autres classes permettant de gérer les messages, une petite interface graphique et de créer des threads pour les clients et diffuseurs qui sont multithreadés.

