# Projet-C-L3-STRI
Projet de développement en C réalisé dans le cadre de la L3-STRI

RFC

This document specifies conceptual implementation of a simple client-server application.

Introduction
This application will implement a directory service on top of a standard TCP/IP stack for networking. This service is text-based and closely resembles IETF’s standards.

Directory
Each server will manage zero or more directories independents from each other. They will contain records representing users. The first name, name and email fields are mandatory for each recorded user, there is no theoretical limit to the number of fields. Two records in the same directory must have the same fields. Empty fields should not be authorized. Each directory is created and administered by a unique user. The administrator has authority over all the directory.

PDU description
Two fields are used for the PDU :
Code : the type of the exchange between both machines (client and server). According to the code, we can determine if the PDU is sent by the client or the server. The different values are:
Request : Additional parameters to the ‘Code’ field.

Code
Request syntax
Comments
AUTH
login password
Request to authenticate a user
A_C


Request to create an account
A_M
oldUserName newUserName oldPassword newPassword
Request to modify an account
A_D
login password usernameToDelete
Request to delete an account
D_C
username recordIndex 
Request to create a directory
D_D


Request to delete a directory
D_P


Request to print a directory
D_A
username targetedUser
Request to authorize a user to access a directory 
D_R
username targetedUser
Request to remove the right of the targeted user to read the directory
R_C


Request to create a record
R_M
Username name firstname phone address email birthdate comments
Request to modify a record
R_D


Request to delete a record
R_P


Request to print a record
OK


Indication accepting a request
KO


Indication refusing a request

Technical Specification
Here two points of view has been created of the client / server algorithms to have a global and a detailed view of the project to make.

Global algorithm

Server:
While(true):
    waitClient()
    ProcessClient()

Client:
While(true or !exit):
    ReadCommand()
    if(commandIsCorrect())
        ProcessCommand()
    Else
        printError()

Detailed algorithm

Server :
InitiateServer() /* check if the server needs to be installed (if no admin account has been found) or to be run */ 
InitiateConnection()
While(true):
    waitCommands()
/* we authorize the authentication inside the called functions, not before */
    According to(commandeReçue):
        responseCode = createDirectory()
       Ou
        responseCode = deleteDirectory()
       Ou
        responseCode = addRecord()
       Ou
        responseCode = deleteRecord()
       Ou
        responseCode = modifyRecord()
       Ou
        responseCode = printDirectory()
       Ou
        responseCode = addReader()
       Ou
        responseCode = rmReader()
       Ou
        responseCode = printRecord()
   Ou
        responseCode = addAccount()
   Ou
        responseCode = deleteAccount()
   Ou
        responseCode = modifyAccount()
   Ou
        responseCode = readDirectory()
   Ou
        responseCode = readRecord()
   Ou
        responseCode = connectionAuthorized()
    SendResponseCode()
CloseConnection()

Client:
commandIsCorrect(command):
    while(exist = false):
        if(getFirstWord(command) == listCommands[i])
            Exit = true
        Else
            i += 1
    if(i == sizeListCommands)
        return error
    else
        return isCommandSyntaxCorrect(listCommands[i], command)
        
Data structures
PDU (to complete):
Record:
name: string [15]
firstName: string [15]
phone: string [10]
address: string [30]
email: string [30]
birthDate: string [10]
comments: string [50]
Account:
username: string [15]
password: string [15]
ownedDirectories: Directory
sharedDirectories: Array of * Directory type [20]
Directory:
recordList : Array of Record type [20]

Data storage
Three types of data are to be stored, accounts, directories and records. The solution provided is to have an only file store all the accounts with one file per directory, each directory containing all of its records.
