# Welcome to My Ftp (FTP Server in C++)
***

Authors: 

Chris Dallat (www.github.com/chrisdallat)
Khalil Masree (www.github.com/khalilmasri)

## Task

Create a FTP server.

File Transfer Protocol (FTP) is a standard Internet protocol for transmitting files between computers on the Internet over TCP/IP connections. 
FTP is a client-server protocol where a client will ask for a file, and a local or remote server will provide it.

You will have to follow the protocol (RFC959)

The network communication will be achieved using TCP sockets.

Server will start with the port and the path of where file can be access:

Usage : ./server port path

    port is the port number on which the server socket is listening.

    path is the path to the home directory for the "Anonymous" user.

    The server MUST be able to handle several clients at the same time by using a
    thread.

This maybe be best achieved through using a Threadpool.

Your server must also have those features:

    An authentication with an "Anonymous" account and an empty password
    Data transfer MUST use "active" or "passive" mode as explained in the protocol.

## Description

We created a server which can handle multiple client connections through use of a 
thread-pool to manage as many simultaneous clients as possible. Then worked through parsing 
the inputs and response codes and messages in order to establish the communication properly 
between server and clients and then dispatch them to the correct handling methods to deal with 
the commands. We then implemented both active and passive modes to comply with the protocal.
Active, establishing a control/command channel and then the server connecting to the client and 
Passive, establishing a control/command channel and then the server telling the client where to 
connect to it and the client initialising the connection. We then implemented the relevant commands
in order for the client to see where they are in the server directories, move around and list the 
directories/contents and then retrieve a file from the server. 

## Installation

#### 1. Type "make" to compile the program

  ```sh
   make
  ```

#### 2. Inputing "make fclean" will clean up all unnecessary files and object files after use. 
```sh
   make fclean
  ```

## Usage


THE SERVER:



#### 1. You can then run the server with "./server" followed by the [PORT NUMBER] that the server will run on 
and the [PATH] of the directory the server shall run on.  
   ```sh
    ./server 2121 .
          
          will run the server on port 2121 and with the path of the current directory.
   ```
  
#### 2. Inputting "exit" will shutdown the server. 



THE FTP CLIENT:



#### 1. Type "ftp" followed by the [IP ADDRESS] followed by the [PORT NUMBER] the server can be found at.
  
   ```sh
   ftp 127.0.0.1 2121
   
        you will then be connected to the server specified in the example code above
   ```
#### 2. You will then connect to the server and be prompted for your username followed by password which can be either:
   
   ```sh
   USER "admin"
   PASS "admin" 
   
    --OR--
    
   USER "Anonymous"
   PASS ""
   ```
   
   Note: if errors are made use command "user" followed by [USERNAME] to try the authentication again
   
#### 3. You can then use the following commands as the client:
   
   ```sh
   * "pwd" - displays the filepath of the current directory the client is accessing
   
   * "cdup" - if possible moves to the parent directory and displays the filepath
   
   * "cd [PATH]" - changes directory to the specified directory, ".." will move to the parent directory if
   possible, both full pathnames and single directory names are permitted
   
   * "passive" - changes the connection to passive mode and will display the IP and port location of the 
   server before executing the next command
   
   * "ls" - will list the contents of the current directory or the information of a file if specified. 
   
   * "ls [PATH]" - will list the contents of the specified path in [PATH]
   
   * "get [FILENAME]" - will retrieve the specified file and save it to the current directory of the client,
   if the file already exists it will be overwritten. 
   
   * "get [FILENAME] [NEW FILENAME]" - will retrieve the specified file [FILENAME] and save it to the current 
   directory of the client but with the specified [NEW FILENAME]
   
   * "quit" OR "bye" - will close the client
   
   ```

---------------------------------------------------------------------------------------------------------------------------------------

### The Core Team
<span><i>Made at <a href='https://qwasar.io'>Qwasar Silicon Valley</a></i></span>
<span><img alt='Qwasar Silicon Valley Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px'></span>


   


