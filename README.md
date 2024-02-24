

![Veridic](https://user-images.githubusercontent.com/52190352/176306430-5ead3b8f-e8ac-48bc-98cd-c89b432f673d.png)






 # prepare, build and run
 
 ```
   sudo make install
   
   make
   
   ./bin/bin
 
 ```
 ## basic struct request
 
 ```c++
 //#include "http/veridic"
 
  Veridic Url("https://URL");
  
   string data = Url.get();
   
   cout << data;
  
 ```


# curl-http-wrapper

make http requests easily in c++ using libcurl, contains two modules, raw http and Veridic that wraps the raw giving it even more functions, I recommend using Veridic but if you want you can use raw http but it has certain limitations, such as only making one request per instance which does not happen in Veridic

# modules

- **HTTP RAW (wrap curl)**
- **VERIDIC (wrap HTTP RAW)**


# examples 
  
  _in main or function etc_
 
### POST 

```C++

  Veridic tasty("https://API_name");
  
  POST fields = {
    "name: jhon",
    "lastname: doe"
  }
  
  Headers headers = {
    "data: test",
    "example: true"
  };
    
  tasty.post(fields, headers, "/user");
  tasty.post(fields, "/users");
  tasy.post(fields);
  

```
### GET

![get_dos](https://user-images.githubusercontent.com/52190352/174460062-32991239-6da1-43d4-a9ce-80ae3363f332.png)



## Methods

- POST
- GET
- PUT
- DELETE
- CUSTM

### example get

<img src="https://user-images.githubusercontent.com/52190352/174459669-46527c3c-8627-4b51-a1f5-ac9597699673.png" width="500px"/>

<img src="https://github.com/scyth3-c/curl-http-wrapper/assets/52190352/d7708690-b72d-4261-a122-870aaf44cab4.png" width="500px"/>

### example put

<img src="https://user-images.githubusercontent.com/52190352/174459721-7897687d-14b3-40b6-936b-29dec5589fd9.png" width="500px"/>

### example delete

<img src="https://user-images.githubusercontent.com/52190352/174459737-1ed37b98-23d0-46ff-bb50-48857096e166.png" width="500px"/>

### example custom

<img src="https://user-images.githubusercontent.com/52190352/174459753-6037a866-b0c3-4012-89ef-83e08099a178.png" width="500px"/>


## Project and Headers

the structure is very flexible in case you want to use directly the one I put, as long as you add the .cpp code inside the **src/sources** folder and add it in the **makefile** like this

```php
  $(DIR_OBJ)/myFile.o
  
```

### http/veridic
 
 #include "http/veridic.hpp"

### http/http
 
 #include "http/http.hpp"
 


 ## functions and arguments
 

 
 ```c++
 
  constructor:
  
    Veridic();
    Veridic(URL);
  
  void setURL(URL);
  
  
 
  [VERIDIC]
  
  int post(FIELDS,  HEADERS,  OPTIONAL endpoint,  OPTIONAL type); 
  int post(FIELDS, OPTIONAL endpoint,  OPTIONAL type);
  
  int get(string optional_endpoint);
  int get(GET& fields, string optional_endoint);
 
 optional values are the endpoint of the URL, and the request type, GET and POST are separate, but PUT and DELETE are variations of the POST type
 
 ```

<hr/>

```C++

  int put(FIELDS, HEADERS, OPTIONAL endpoint);
  int put(FIELDS, OPTIONAL endpoint);
  
  
  int Delete(FIELDS, HEADERS, OPTIONAL endpoint);
  int Delete(FIELDS, OPTIONAL endpoint);



```
 
 ### FIELDS
  
  the fields are a structure in charge of controlling and preparing the necessary parameters for the requests, they have no limit
 
 Examples
 
 ```c++
 in headers fields, DATA: VALUE
 
  Headers = {
    "data: 1",
    "data: 2",
    "data: N",
    "N: N",
  };
 
 same that post >>>
  
 in POST fields, DATa=value
 
    POST fields = {
      "uno=1",
      "dos=2",
      "tres=3",
      "N=N"
    };
    
    GET fields = {
      "uno=1",
      "dos=2",
      "tres=3",
      "N=N"
    };
    
    PUT fields = {
      "uno=1",
      "dos=2",
      "tres=3",
      "N=N"
    };
    
    DELETE fields = {
      "uno=1",
      "dos=2",
      "tres=3",
      "N=N"
    };
    
 
 ```

 ### the fields
 
 the fields are the same structure but with a typedef to make the code easier to read, the only difference is the field Headers, which has another structure
  
  ### other media
  
  
![veridic](https://user-images.githubusercontent.com/52190352/174459626-1684c6ab-8957-4a14-a7c9-f5ee421f739c.png)

 
 
 
