# NetRadio
Projet Réseaux S6

## Compilation

Pour compiler:
- Le projet : `make`
- Le code c : `make c`
- Le code java : `make java`

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

### Gestionnaire
Le gestionnaire se lance ainsi : `./manager [OPTION]... PORT`
- `PORT` est le n° du port sur lequel le gestionnaire écoute les connexions entrantes
- `[OPTION]...` les options pour le gestionnaire
   - `-h` affiche l'aide
   - `-v` active le mode verbeux
   - `-d TEMPS` fais attendre le gestionnaire *TEMPS* secondes entre chaque envoi de *RUOK*; la valeur par défaut est 30 secondes.

Par exemple, la commande `./manager -v -d 5 5000` lance un gestionnaire en mode verbeux sur le port 5000, il testera la présence d'un diffuseur toutes les 5 secondes 


### Clients
#### Client java
* `java -jar JavaClient.jar ADRESSE_MULTI PORT_MULTI ADRESSE_TCP PORT_TCP`
* `java -jar JavaClient.jar` lance un client par défaut. 
   * Équivalent à `java -jar JavaClient.jar 225.0.0.0 5001 localhost 4242` 

#### Client C 
On peut lancer le client C pour qu'il communique soit avec un gestionnaire, soit avec un diffuseur+multicast. Il n'y a pas de connexion par défaut. Le client doit connaître les adresses du diffuseur ou du gestionnaire avec lequel il veut communiquer.  
Communiquer avec un gestionnaire : ``./client adresse port`` où ``adresse`` et ``port`` sont ceux du gestionnaire en question.  
Communiquer avec un diffuseur et multidiffuseur : ``./client adresse_diff port_diff adresse_multicast port_multicast fichier`` où ``fichier`` est le fichier où sont affichés les messages diffusés par le multi-diffuseur. La bonne démarche est : Ouvrir un terminal, tapez ``tty`` et copier-coller le résultat affiché à la place de fichier. Ainsi les messages s'afficheront dans ce nouveau terminal.
 
