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

Il y a deux manières de lancer un diffuseur. Soit par ligne de commande, soit par fichiers. Par les lignes de commandes, nous disposons de trois manières:
* ``java Streamer`` permet de lancer un diffuseur par défaut avec les caractéristiques suivantes:
    * ``id = STREAMER``
    * ``port de réception = 4242``
    *  ``adresse de multi diffusion = 225.0.0.0``
    * ``port de multi diffusion = 5001``
    * ``fréquence = 1000``
    * Ce diffuseur ne se connectera à aucun gestionnaire
* ``java Streamer`` avec cinq options: l'id du diffuseur, son port de réception, son addresse de multicast, son port de multicast, sa fréquence d'envoi (exemple: ``java Streamer myStream 4242 225.0.0.0 5001 5000``).
* ``java Streamer``avec sept options: les mêmes que précedemment plus une adresse sur laquelle tourne un gestionnaire et son port de réception (exemple: ``java Streamer myStream 4242 225.0.0.0 5001 5000 localhost 4343``).

Des messages par défaut sont diffusés par le diffuseur créé.

Pour lancer un diffuseur grâce à des fichiers faire ``java Streamer file1 file2``où ``file1`` est une fichier contenant des informations sur le diffuseur. Son format doit être le suivant:
* la première ligne contient l'id du diffuseur
* la deuxième, son port de réception
* la troisième, l'addresse de multicast
* la quatrième, le port de multicast
* la cinquième, la fréquence d'envoi.

``file2`` contient les messages que l'on souhaite diffuser. La première ligne de ce fichier doit être l'id de l'auteur du message. Ensuite, chaque ligne contient un message.

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
Il y a deux manières de lancer un client. Soit lancer ``java JavaClient`` : cela lancera un client par défaut qui se connectera à l'adresse de multi diffusion ``225.0.0.0``, au port de multi diffusion ``5001``, à l'adresse ``localhost`` et au port ``4242``.
Sinon, lancer ``java JavaClient`` avec les options, adresse de multi diffusion, port multi diffusion, adresse pour mode connecté, port pout le mode connecté.

### Client C 
On peut lancer le client C pour qu'il communique soit avec un gestionnaire, soit avec un diffuseur+multicast. Il n'y a pas de connexion par défaut. Le client doit connaître les adresses du diffuseur ou du gestionnaire avec lequel il veut communiquer.  
Communiquer avec un gestionnaire : ``./client adresse port`` où ``adresse`` et ``port`` sont ceux du gestionnaire en question.  
Communiquer avec un diffuseur et multidiffuseur : ``./client adresse_diff port_diff adresse_multicast port_multicast fichier`` où ``fichier`` est le fichier où sont affichés les messages diffusés par le multi-diffuseur. La bonne démarche est : Ouvrir un terminal, tapez ``tty`` et copier-coller le résultat affiché à la place de fichier. Ainsi les messages s'afficheront dans ce nouveau terminal.
 
