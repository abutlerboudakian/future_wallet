# Import statements
import sqlalchemy as sa
import os

'''
    @params:    database -  Name of the database to connect to
    @requires:  Database is online and remotely accessible, database specified exists, username and password are stored in environment variables
    @modifies:  None
    @effects:   Creates a new SQL alchemy engine connected to the database specified and returns it
    @returns:   New SQL Alchemy engine
'''
def connect_engine(database):
    cstr = 'mssql+pyodbc://' + os.environ['FWUSER'] + ':' + os.environ['FWPASS'] + '@localhost:1433/' + database + '?driver=ODBC+Driver+17+for+SQL+Server'
    return sa.create_engine(cstr, execution_options=dict(stream_results=True))
