StringQuery v 0.1

A little query language that can be embedded in a URL.  Can be used for other
purposes as well.  Includes syntax and validity checking.

WARNING: THIS SOFTWARE IS IN ALPHA AND SHOULD NOT BE USED IN PRODUCTION.  I HAVE 
ONLY COMPILED USING GCC ON CENTOS 6.6.  AS OF RIGHT NOW, ASCII IS THE ONLY
SUPPORTED CHARACTER ENCODING ALTHOUGH I WILL BE LOOKING INTO SUPPORTING UTF-8.

NOTE: If using StringQuery in a URL, code will need to be encoded using the
application/x-www-form-urlencoded format.  More information on this encoding: 
http://www.w3.org/TR/html401/interact/forms.html#h-17.13.4.1
 
The MIT License (MIT)

Copyright (c) 2015 Andrew Schools

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
 
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

Parser Options:

    --file      <file>        Parse code from <file>.   
    --code      <code>        Parse code   
    --export    <JSON|SQL>    Export to what format         Defaults to JSON       

To do / issues:

    - Error indicator is off many times.
