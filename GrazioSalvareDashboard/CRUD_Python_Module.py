from pymongo import MongoClient 
from bson.objectid import ObjectId 

class AnimalShelter(object): 
    """ CRUD operations for Animal collection in MongoDB """ 

    def __init__(self, username, password, database, collection): 
        # Initializing the MongoClient. This helps to access the MongoDB 
        # databases and collections. This is hard-wired to use the aac 
        # database, the animals collection, and the aac user. 
        # 
        # You must edit the password below for your environment. 
        # 
        # Connection Variables 
        # 
        USER = username 
        PASS = password
        DB = database
        COL = collection
        # URI connection string using authentication
        # Provided connection string was not working
        URI = (f"mongodb://{USER}:{PASS}"
               f"@localhost:27017/?"
               "authSource=aac"
               "&authMechanism=SCRAM-SHA-1")
        # 
        # Initialize Connection 
        # 
        self.client = MongoClient(URI)
        self.database = self.client['%s' % (DB)] 
        self.collection = self.database['%s' % (COL)] 

    # Create a method to return the next available record number for use in the create method
    def get_next_record_id(self, records_list):
        if not records_list:
            return 1  # start at 1 if empty
        else:
            # adds one index to existing indexes for new record
            return max(record["id"] for record in records_list) + 1
        
    # Complete this create method to implement the C in CRUD. 
    def create(self, data):
        if data is not None:
            insert_Valid = self.collection.insert_one(data)
            return True if insert_Valid.acknowledged else False# data should be dictionary             
        else: 
            raise Exception("Nothing to save, because data parameter is empty")
        new_id = get_next_record_id(self.database.animals)
        new_record = data
        self.database.animals.insertOne(data)

    # Create method to implement the R in CRUD.
    def read(self, query=None):
        
        if query is None:
            query = {}
            
        results = self.collection.find(query)
        
        return results
            
    # Create method to implement the U in CRUD
    def update(self, search_criteria, new_data):
        doc_count = self.collection.count_documents(search_criteria)
        # if search yields multiple documents, update all found documents
        if doc_count > 1:
            # Assigns UpdateResult Object to variable to print out update count
            update_result = self.collection.update_many(search_criteria, {'$set': new_data})
        
        # if search yields single document, update that document
        elif doc_count == 1:
            # Assigns UpdateResult Object to variable to print out update count
            update_result = self.collection.update_one(search_criteria, {'$set': new_data})
        
        # print the number of documents that were updated
        print(update_result.modified_count)
    
    # Create method to implement the D in CRUD
    def delete(self, search_criteria, docs_to_delete = 'one'):
        # if docs_to_delete is one, use delete_one
        if docs_to_delete == 'one':
            # Assigns deleteResult Object to variable to print out delete count
            delete_result = self.collection.delete_one(search_criteria)
        
        # if docs_to_delete is all, use delete_many
        elif docs_to_delete == 'all':
            # Assigns deleteResult Object to variable to print out delete count
            delete_result = self.collection.delete_many(search_criteria)
        
        # print the number of documents that were deleted
        print(delete_result.deleted_count)