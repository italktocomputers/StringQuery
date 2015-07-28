 StringQuery v 0.1
 
 A little query language that can be embedded in a URL.
 
 Copyright (c) 2015, Andrew Schools <andrewschools@me.com>
 Permission is hereby granted, free of charge, to any
 person obtaining a copy of this software and associated
 documentation files (the "Software"), to deal in the
 Software without restriction, including without
 limitation the rights to use, copy, modify, merge,
 publish, distribute, sublicense, and/or sell copies
 of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the
 following conditions:
 The above copyright notice and this permission notice
 shall be included in all copies or substantial portions
 of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 
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
