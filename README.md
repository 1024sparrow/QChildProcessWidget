# sqlka
uuuu
Инструмент для генерирования документации по SQL-запросам, генератор с++-класса, представляющего интерфейс к реляционной БД

Необходимо в генерируемом интерфейсном классе сделать что-то вроде процедурок PostgreSQL, только универсально для БД PostgreSQL или SQlite.

Генерируемая документация - md-страница с описанием "процедур", диаграмма таблиц (картинка, сгенерированная из сгенерированного  [plantuml](http://plantuml.com/))

![dd](https://raw.githubusercontent.com/grafov/plantuml2mysql/master/database.png)
```
UML legend:

table = class
#pkey
+index

@startuml
    class user <<(U,olive)>> {
        Users table example.
        ==
        #id
        +login : varchar(16)
        +mail : varchar(64)
        docsRef : int(10) -- referenced docs for a user
        created : int(11)
        sesid : int(11)
    }

    user "1" -- "0..*" docs

    class session <<(U,olive)>> {
        Sessions table example.
        ==
        #id
        +uid : int(10) unsigned
        remoteip : int(10) unsigned
        useragent : varchar(255)
        data : longtext -- serialized session data
        +lastseen : int(11)
    }

    session "0..*" -- "1" user
    
    class docs <<(F,brown)>> {
        Documents storage example.
        ==
        #id : int(10)
        #fid : int(10) -- link to a file
        +aunthorid : int(10)
        +created : int(11)
    }
    
    class files <<(F,brown)>> {
        File storage example.
        ==
        #id
        +docId : int(10)
        title : varchar(255)
        path : varchar(255)
        hash : int(32) unsigned
    }

    files "1" -- "1..*" docs

@enduml
```

```sql
    CREATE DATABASE sampledb CHARACTER SET = utf8 COLLATE = utf8_unicode_ci;
    USE sampledb;                                                           
                                                                            
    CREATE TABLE IF NOT EXISTS `user` (                                       
      id               SERIAL,                                              
      login            VARCHAR(16),                                         
      mail             VARCHAR(64),                                         
      docsRef          INT(10) COMMENT 'referenced docs for a user',        
      created          INT(11),                                             
      sesid            INT(11),                                             
      PRIMARY KEY (id),                                                     
      INDEX (login),                                                        
      INDEX (mail)                                                          
    );                                                                      
                                                                            
    CREATE TABLE IF NOT EXISTS `session` (                                    
      id               SERIAL,                                              
      uid              INT(10) UNSIGNED,                                    
      remoteip         INT(10) UNSIGNED,                                    
      useragent        VARCHAR(255),                                        
      data             LONGTEXT COMMENT 'serialized session data',          
      lastseen         INT(11),                                             
      PRIMARY KEY (id),                                                     
      INDEX (uid),                                                          
      INDEX (lastseen)                                                      
    );                                                                      
                                                                            
    CREATE TABLE IF NOT EXISTS `docs` (                                       
      id               INT(10),                                             
      fid              INT(10) COMMENT 'link to a file',                    
      aunthorid        INT(10),                                             
      created          INT(11),                                             
      PRIMARY KEY (id, fid),                                                
      INDEX (aunthorid),                                                    
      INDEX (created)                                                       
    );                                                                      
                                                                            
    CREATE TABLE IF NOT EXISTS `files` (                                      
      id               SERIAL,                                              
      docId            INT(10),                                             
      title            VARCHAR(255),                                        
      path             VARCHAR(255),                                        
      hash             INT(32) UNSIGNED,                                    
      PRIMARY KEY (id),                                                     
      INDEX (docId)                                                         
    );                                                                      
```

**Материалы для постановки задачи взяты из описания задачи [plantuml2mysql](https://github.com/grafov/plantuml2mysql)**
