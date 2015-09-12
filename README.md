StringQuery v 0.1


A little query language that can be embedded in a URL.  Can make querying data via 
a Restful service easier and cleaner.   Includes syntax and validity checking.


WARNING: THIS SOFTWARE IS IN ALPHA AND SHOULD NOT BE USED IN PRODUCTION.  I HAVE 
ONLY COMPILED USING GCC ON CENTOS 6.6. IF YOU ARE ACCEPTING CODE FROM A URL IT 
SHOULD BE ENCODED USING THE APPLICATION/X-WWW-FORM-URLENCODED FORMAT.  NOT DOING 
SO CAN LEAD TO COMMAND INJECTION.  

More information on this encoding: 
http://www.w3.org/TR/html401/interact/forms.html#h-17.13.4.1


To compile on Centos 6.6 using GCC 4.4.7: 

    gcc parser.c -lm

 
Valid statement syntax: 

    Resource.Entity : Type Operator Filter 
  
Resource.Entity specifies the Resource and Entity to apply the filter against.  
In a relational database this would be a table and field name.
  
Type specifies the data type of the Entity.  The following data types are 
supported:

    String
    Int
    Double
    @
    
The last type is a variable which tells the parser you are specifying a
Resource.Entity.  This data type is used when performing a simple join.
     
The following operators are supported:

    =
    !=
    >
    <
    >=
    <=
     
Filters can be any of the supported types or a collection of types known as 
a List.  Example comparing Entity against a single value:

    User.FirstName:String='Andrew'
     
You can concatenate multiple statements by using one of the following conjunctive
operators:

    &
    |     
     
If I want to search for multiple names:

    User.FirstName:String='Andrew'|User.FirstName:String='Doug'
 
Which can be shortened by using a List:

    User.FirstName:String=('Andrew','Doug')
        
Each item in a List is separated by a ','.
 
Valid List syntax:

     (value1[,value2,...])
     
StringQuery supports simple JOINS.  Let's say we have the following resources
with the following entities:

    Person Resource with the following entities:

    Id:Int
    FirstName:String
    LastName:String

    Student Resource with the following entities:

    Id:Int
    Person.Id:Int
    Major:String

Using the query below, we can join Person and Student on PersonId.

    Person.FirstName:String='Andrew'&Person.LastName:String='Schools'&Student.PersonId:@=Person.Id
    
Exporting to SQL will output:
    SELECT
      Person.*
      ,Student.*
    FROM Person, Student
    WHERE Person.FirstName = 'Andrew' 
    AND Person.LastName = 'Schools' 
    AND Student.PersonId = Person.Id
    
You can also send a URL encoded string to the parser:

    ./a.out --code "User.FirstName%3AString%3D%27Andrew%27%26User.Age%3AInt%3D(30%2C31%2C32)" --code-format urlencoded

This allows you to encode a StringQuery string from a URL and safely pass it to 
the parser without worrying about command injection.


Parser Options:

    --file          <file>                      Parse code from <file>   
    --code          <code>                      Parse code   
    --code-format   <urlencoded|nothing>        What format is the code in    Defaults to nothing
    --export        <JSON|SQL>                  Export to what format         Defaults to JSON     

To do / issues:
    
    

