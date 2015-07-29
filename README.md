 StringQuery v 0.1
 
 A little query language that can be embedded in a URL.
 
 Valid statement syntax: 
 
     Entity : Type Operator Filter
  
 Entity specifies the resource to apply the filter against.
  
 Type specifies the data type of the resource.  The following data types are supported:
 
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
     ?
     
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
      
  
 NOTE: This software is incomplete.  To do list:
 
     - More syntax checks
     - Need to add support for aggregate functions:
             AVG
             COUNT
             MAX
             MIN
             SUM
     - Need to provide syntax for the retrieval of fields
