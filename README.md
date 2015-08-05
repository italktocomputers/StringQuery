StringQuery v 0.1


A little query language that can be embedded in a URL.  Can be used for other
purposes as well.  Includes syntax and validity checking.


WARNING: THIS SOFTWARE IS IN ALPHA AND SHOULD NOT BE USED IN PRODUCTION.  I HAVE 
ONLY COMPILED USING GCC ON CENTOS 6.6.  AS OF RIGHT NOW, ASCII IS THE ONLY
SUPPORTED CHARACTER ENCODING ALTHOUGH I WILL BE LOOKING INTO SUPPORTING UTF-8.
IF YOU ARE ACCEPTING CODE FROM A URL IT SHOULD BE ENCODED USING THE
APPLICATION/X-WWW-FORM-URLENCODED FORMAT.  NOT DOING SO CAN LEAD TO COMMAND 
INJECTION.  

More information on this encoding: 
http://www.w3.org/TR/html401/interact/forms.html#h-17.13.4.1


To compile on Centos 6.6 using GCC 4.4.7: 

    gcc parser.c -lm

 
Valid statement syntax: 

    Entity : Type Operator Filter 
  
Entity specifies the resource to apply the filter against.  In a relational 
database an Entity would reference a table.
  
Type specifies the data type of the resource.  The following data types are 
supported:

    String
    Int
    Double
     
The following operators are supported:

    =
    !=
    >
    <
    >=
    <=
     
Filters can be any of the supported types or a collection of types known as 
a List.  Example comparing Entity against a single value:

    FirstName:String='Andrew'
     
You can concatenate multiple statements by using one of the following logical
operators:

    &
    |     
     
If I want to search for multiple names:

    FirstName:String='Andrew'|FirstName:String='Doug'
 
Which can be shortened by using a List:

    FirstName:String=('Andrew','Doug')
        
Each item in a List is separated by a ','.
 
Valid List syntax:

     (value1[,value2,...])

Parser Options:

    --file          <file>                      Parse code from <file>   
    --code          <code>                      Parse code   
    --code-format   <urlencoded|nothing>        What format is the code in    Defaults to nothing
    --export        <JSON|SQL>                  Export to what format         Defaults to JSON     

To do / issues:
