# NetRadio
Projet Réseaux S6

## Compiler le projet

Pour compiler le projet il suffit d'exécuter la commande `make all` à la racine du projet. Il est également possible de compiler uniquement le code source java en faisant `make classes`, ou le code source C avec `make exec`.

## Lancer un diffuseur

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