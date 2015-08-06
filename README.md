The purpose of this branch is to incorporate syntax which can be used to 
join resources.  I am thinking of the following syntax:

Person Resource

Id:Int
FirstName:String
LastName:String

Student Resource

Id:Int
Person.Id:Int
Major:String


Person.FirstName:String='Andrew'&Person.LastName:String='Schools'&Student.PersonId:@=Person.Id

The above query would join Person and Student on PersonId.  This will involve 
creating a new type '@' which will be a resource type.  This also includes 
allowing the entity name to also include the resource name like so: 
resourceName.entityName
