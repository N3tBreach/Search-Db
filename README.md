# 🔍 Recherche Multi-thread dans les Fichiers

Ce projet est un programme de **recherche multi-thread** qui scanne des fichiers texte dans un répertoire spécifié à la recherche d’un terme donné. Il utilise des **threads POSIX (pthreads)** pour exécuter les recherches en parallèle, ce qui le rend efficace pour explorer de grands répertoires.

## 📝 Fonctionnalités

- **Multi-thread** : Jusqu'à 4 threads peuvent chercher simultanément dans différents fichiers, améliorant ainsi les performances.
- **Types de fichiers pris en charge** : Le programme recherche dans les fichiers `.txt`, `.csv` et `.sql`.
- **Résultats limités** : La recherche s'arrête après avoir trouvé 10 correspondances, évitant ainsi de surcharger les résultats.
- **Recherche en temps réel** : L'utilisateur peut entrer un nouveau terme de recherche sans redémarrer le programme.
- **Recherche sécurisée** : Des mutex assurent un accès sécurisé au compteur de résultats partagé entre les threads.

## 🛠️ Prérequis

- **Système compatible POSIX** (Linux, macOS, ou Windows avec Cygwin/WSL)
- **GCC** (GNU Compiler Collection) ou tout autre compilateur C compatible
- **Bibliothèque pthreads** (déjà incluse dans la plupart des systèmes POSIX)

## 🧑‍💻 Compilation

Pour compiler le programme, utilisez la commande suivante dans votre terminal :

```bash
gcc -o search_database main.c -lpthread
```

- `-lpthread` : Lie la bibliothèque pthreads.

## ▶️ Utilisation

Une fois compilé, lancez le programme avec :

```bash
./search_database.exe
```

Le programme vous demandera d'entrer un terme de recherche.

Exemple :
```bash
Entrez un terme de recherche (ou 'q' pour quitter) : exemple
```

Vous pouvez entrer de nouveaux termes jusqu'à ce que vous tapiez `q` pour quitter.

## 📁 Chemin du Répertoire

Par défaut, le programme recherche les fichiers dans le répertoire suivant :
```bash
C:/Users/xdabase/Desktop/db
```

Pour changer le chemin du répertoire, modifiez la variable `directory_path` à **la ligne 107** du code :
```c
char directory_path[256] = "C:/Users/xdatabase/Desktop/db";  // Modifiez ce chemin si nécessaire
```

## ⚙️ Personnalisation

- **MAX_RESULTS** : Vous pouvez modifier le nombre maximal de résultats en changeant cette ligne dans le code :
  ```c
  #define MAX_RESULTS 10
  ```
- **MAX_THREADS** : Modifiez le nombre maximum de threads en ajustant cette ligne :
  ```c
  #define MAX_THREADS 4
  ```

## 🛡️ Sécurité des Threads

- Le programme utilise un **mutex** (`pthread_mutex_t`) pour protéger les données partagées (comme le compteur de résultats) entre les threads. Cela garantit la sécurité des threads lorsque plusieurs d'entre eux tentent de mettre à jour la même variable.

## 🤖 Remarques

- Le programme ne scanne que les fichiers se terminant par `.txt`, `.csv` ou `.sql`.
- Il ignore les dossiers spéciaux comme `.` et `..` pour éviter les boucles infinies ou les erreurs.
- Si plus de 4 fichiers sont présents, le programme attend que certains threads se terminent avant d'en créer de nouveaux, empêchant une surcharge des ressources.
  
## 🏗️ Améliorations Futures

- Ajouter la possibilité de spécifier dynamiquement le chemin du répertoire depuis la ligne de commande.
- Améliorer le programme pour rechercher de manière récursive dans les sous-répertoires.
- Prendre en charge d'autres formats de fichiers comme `.log` ou `.json`.
