import pyodbc

def getConn() -> pyodbc.Connection:
    cnxn = pyodbc.connect("Driver={ODBC Driver 17 for SQL Server};" \
                          "Server=127.0.0.1;" \
                          "Database=ii24s88216;" \
                          "uid=s88216;pwd=xxxxxxx")
    return cnxn
