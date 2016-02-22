StringQuery v 0.1


A little query language.


WARNING: THIS SOFTWARE IS IN ALPHA AND SHOULD NOT BE USED IN PRODUCTION.  I HAVE
ONLY COMPILED USING GCC ON CENTOS 6.6 & MAC OS X 10.11.2. IF YOU ARE ACCEPTING
CODE FROM A URL IT SHOULD BE ENCODED USING THE APPLICATION/X-WWW-FORM-URLENCODED
FORMAT.  NOT DOING SO CAN LEAD TO COMMAND INJECTION.  

To compile using GCC:

    gcc main.c parser.c translators/sql.c translators/json.c validation.c extraction.c library.c -lm

To run tests, cd into the /tests directory and run following command:

    gcc main.c test_functions.c ../validation.c ../extraction.c ../library.c


Valid statement syntax:

    Resource:Type Operator Filter

Resource.Entity specifies the Resource and Entity to apply the filter against.  
In a relational database this would be a table and field name.

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

The following data types are supported:

    String
    Int8
    uInt8
    Int16
    uInt16
    Int24
    uInt24
    Int32
    uInt32
    Int64
    uInt64
    Double
    Date
    Time
    DateTime
    NULL
    @

The last type is a variable which tells the parser you are specifying a
Resource.Entity.  This data type is used when performing a simple joins.

You can concatenate multiple statements by using one of the following conjunctive
operators:

    &
    |     

If I want to search for multiple names:

    User.FirstName:String='Andrew'|User.FirstName:String='Doug'

Which can be shortened by using a List:

    User.FirstName:String=('Andrew','Doug')

Each item in a List is separated by a ','.  Valid List syntax:

     (value1[,value2,...])

StringQuery supports simple JOINS.  Let's say we have the following resources
with the following entities:

    Person Resource:

    Person.Id
    Person.FirstName
    Person.LastName

    Student Resource:

    Student.Id
    Person.Id
    Student.Major

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

You can also search for NULL values:

    Person.Dept:String=NULL

Or by DateTime which uses UTC:

    Person.CreatedDate:DateTime=2016-01-01 00:00:00


You can also send a URL encoded string to the parser:

    ./a.out --code "User.FirstName%3AString%3D%27Andrew%27%26User.Age%3AInt%3D(30%2C31%2C32)" --code-format urlencoded

This allows you to encode a StringQuery string from a URL and safely pass it to
the parser without worrying about command injection.

StringQuery supports the following system variables:

    __fetch: allows you to specify a list of fields you would like returned.
    __order: allows you to specify a list of fields you would like to order by.
    __by: allows you to specify a single sort variable: @desc|@asc.

    User.FirstName:String='Andrew'&User.LastName:String='Schools'&__fetch:@=(User.FirstName, User.LastName, User.Age)&__order:@=(User.LastName, User.FirstName)&__by:@=desc


Parser Options:

    --file          <file>                      Parse code from <file>   
    --code          <code>                      Parse code   
    --code-format   <urlencoded|nothing>        What format is the code in    Defaults to nothing
    --export        <JSON|SQL>                  Export to what format         Defaults to JSON
