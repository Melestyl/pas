# 🏢 <u>Desklink</u> [Programmation Système] 🏢

This project was created during our "Programmation Système" course at school.

It's written in C and uses shared memory, mutex, mailboxes, and more. 

## 📝 Description 📝

<u>Desklink</u> is a system programming project that allows users to manage and
book areas such as offices and meeting rooms.

It includes an admin mode for additional functionalities
(creating and removing areas). 

## 📂 Project Structure 📂

The project is divided into several parts:

- 📂 `desklink` Project
  - 📂 `Common` Common files and linked list functions
    - 📜 common.c
    - 📚 common.h
    - 📜 linked_list.c
    - 📚 linked_list.h
    - 📄 `config.txt` Previously used before switching to .h
    - 🛠️ Makefile
  - 📂 `Server` Server files with areas management
    - 📜 areas.c
    - 📚 areas.h
    - 📜 server.c
    - 📚 server.h
    - 🛠️ Makefile
  - 📂 `User` User files with booking management
    - 📜 user.c
    - 📚 user.h
    - 🛠️ Makefile
  - 📄 README.md
  - 📄 .gitignore

## 🚀 Getting Started 🚀

To run the project:

### From project directory, compile the project in
`Server` and `User` folders.
```shell
cd Server && make
cd -
cd User && make
```

### Launch a server, and then launch a user.
```shell
cd Server && ./server
```
```shell
cd User && ./user
```

Multiple users can be started at the same time.

### Follow the instructions in the terminal 🎉.
Using the menu, you can book areas, create areas, and more.

Be sure to be admin to create areas... 🤫

### Starting as admin
There is a secret password that is told from generation to generation...
```shell
cd User && ./user 1234
```

## 📚 Version 📚

1.0

## 👥 Authors 👥

- DELANNOY Anaël
- TELLIER--CALOONE Tom

We hope you find this project interesting and useful! 😊